#include "pch.h"
#include "RenderContext.h"
#include <dwrite_1.h>
#include "BrushTransformToBounds.h"
#include "DrawImageCommand.h"
#include "DrawEffectCommand.h"
#include "DrawTextCommand.h"

#include "..\Core\Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace std;
using namespace D2D1;

ComPtr<ID2D1Factory1>  RenderContext::m_d2dFactory;
ComPtr<IDWriteFactory> RenderContext::m_directWriteFactory;

auto RenderContext::GetDirectWriteFactory() -> const ComPtr<IDWriteFactory>
{
    if(!m_directWriteFactory)
    {
        HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                                         __uuidof(IDWriteFactory),
                                         reinterpret_cast<IUnknown**>(m_directWriteFactory.ReleaseAndGetAddressOf()));
        Check(hr);
    }

    return m_directWriteFactory;
}

auto RenderContext::GetFactory() -> const ComPtr<ID2D1Factory1>& 
{
    if(!m_d2dFactory)
    {
        D2D1_FACTORY_OPTIONS options;
#if _DEBUG
        options.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
#else
        options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,  
                                       options,
                                       m_d2dFactory.ReleaseAndGetAddressOf());

        Check(hr);
               
    }

    return m_d2dFactory;
}

RenderContext::RenderContext(const ComPtr<ID3D11Device>& device)
{
    m_device = device;

    Initialize();
}

RenderContext::~RenderContext(void)
{
}

void RenderContext::Initialize()
{
    ComPtr<IDXGIDevice> dxgiDevice;
    m_device.As(&dxgiDevice);
                            
    HRESULT hr = S_OK;
    auto factory = GetFactory();
    
    hr = m_d2dFactory->CreateDevice(dxgiDevice.Get(),
                                    m_d2dDevice.ReleaseAndGetAddressOf());

    Check(hr);

    hr = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, 
                                          &m_d2dDeviceContext);

    Check(hr);


	m_d2dDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
	m_d2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);


}

HRESULT RenderContext::GetDevice(ComPtr<ID3D11Device>& device)
{
    device = m_device;
    return S_OK;
}

HRESULT RenderContext::Get2DDevice(ComPtr<ID2D1Device>& device)
{
    device = m_d2dDevice;
    return S_OK;
}


void RenderContext::SetTransform(const Matrix3x2F& matrix)
{
    m_d2dDeviceContext->SetTransform(matrix);
}

HRESULT RenderContext::PushRenderSurface(const ComPtr<IDXGISurface>& surface)
{
    auto renderBitmap = CreateBitmap(surface);

    PushRenderTarget(renderBitmap, false);
    m_lastRenderTarget = surface.Get();
    return S_OK;
}

auto RenderContext::GetCurrentRenderTarget() -> void *
{
    return m_lastRenderTarget;
}

auto RenderContext::GetRenderBitmap() -> BitmapPtr
{
    return m_renderStateQueue.front().renderTarget;
}

auto RenderContext::Get2DDeviceContext() const -> const ID2D1DeviceContextPtr& 
{
    return m_d2dDeviceContext;
}

void RenderContext::DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const PenPtr& pen)
{
    if(brush)
    {
        BrushTransformToBounds brushTransformSwap;
        brushTransformSwap.Setup(brush.get(), geometry, pen);

        FillCommand fillCommand;
        fillCommand.SetRenderTarget(m_d2dDeviceContext.Get());
        fillCommand.SetBrush(brush.get());
        fillCommand.SetGeometry(geometry.get());
        fillCommand.Run();
    }

    if(pen)
    {
        auto strokeThickness = pen->Thickness();

        if(strokeThickness > 0)
        {
            auto strokeBrush = pen->Brush();

            BrushTransformToBounds brushTransformSwap;
            brushTransformSwap.Setup(strokeBrush.get(), geometry, pen);

            StrokeCommand strokeCommand;
            strokeCommand.SetRenderTarget(m_d2dDeviceContext.Get());
            strokeCommand.SetBrush(pen->Brush().get());
            strokeCommand.SetGeometry(geometry.get());
            strokeCommand.SetPen(pen);
            strokeCommand.Run();
        }
    }
}


void RenderContext::Clear(const Color& c)
{
    m_d2dDeviceContext->Clear(c);
}

void RenderContext::BeginClip(const Graphucks::Rectangle& clipRect)
{
    auto rect = clipRect;
    Matrix3x2F currentTransform;
    m_d2dDeviceContext->GetTransform(&currentTransform);

    m_d2dDeviceContext->PushAxisAlignedClip(rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    auto clipState = ClipState(clipRect, currentTransform);

    auto& currentState = m_renderStateQueue.front();
    currentState.clipQueue.push_back(clipState);
}

void RenderContext::EndClip()
{
    m_d2dDeviceContext->PopAxisAlignedClip();
    auto& currentState = m_renderStateQueue.front();
    currentState.clipQueue.pop_back();
}

auto RenderContext::CreateBitmap(const ComPtr<IDXGISurface>& surface) ->BitmapPtr 
{
     auto bitmap = make_shared<Bitmap>(surface);
     bitmap->SetDeviceContext(m_d2dDeviceContext.Get());
     return bitmap;
}

void RenderContext::PopRenderTarget()
{
    m_d2dDeviceContext->EndDraw();
    m_d2dDeviceContext->SetTarget(nullptr);
    m_renderStateQueue.pop_front();

    if(!m_renderStateQueue.empty())
    {
        auto& state = m_renderStateQueue.front();

        SetRenderTarget(state.renderTarget);
        m_d2dDeviceContext->BeginDraw();
        for(size_t i = 0; i < state.clipQueue.size(); i++)
        {
            state.clipQueue[i].PushClip(m_d2dDeviceContext);
        }
    }
}

void RenderContext::SetRenderTarget(const BitmapPtr& renderTarget)
{
    m_d2dDeviceContext->SetTarget(renderTarget->GetResource<ID2D1Bitmap>(m_d2dDeviceContext.Get()));
}

void RenderContext::PushRenderTarget(const BitmapPtr& renderTarget, bool clear)
{
    if(!m_renderStateQueue.empty())
    {
        auto& currentState = m_renderStateQueue.front();
        
        for(size_t i = currentState.clipQueue.size(); i-- > 0;)
        {
            currentState.clipQueue[i].PopClip(m_d2dDeviceContext);
        }

        m_d2dDeviceContext->EndDraw();
    }

    m_renderStateQueue.emplace_front(renderTarget);
   
    SetRenderTarget(renderTarget);
    m_d2dDeviceContext->BeginDraw();

    if(clear)
        Clear(Graphucks::Color(0,0,0,0));
}


void RenderContext::DrawBitmap(const BitmapPtr& bitmap, 
                               const EffectCompositeMode compositeMode, 
                               const Graphucks::Size& targetSize)
{
    DrawImageCommand command;

    command.Bitmap(bitmap.get());
    command.CompositeMode(compositeMode);
    command.TargetSize(targetSize);
    command.DeviceContext(m_d2dDeviceContext.Get());
    
    command.Run();
}


void RenderContext::DrawEffect(const BitmapPtr& bitmap, 
                               const ElementEffectPtr& elementEffect, 
                               const EffectCompositeMode compositeMode, 
                               const Graphucks::Size& targetSize)
{
    DrawEffectCommand command;

    command.Bitmap(bitmap);
    command.ElementEffect(elementEffect.get());
    command.CompositeMode(compositeMode);
    command.TargetSize(targetSize);
    command.DeviceContext(m_d2dDeviceContext.Get());
    
    command.Run();
}


void RenderContext::DrawText(const TextLayoutPtr& textLayout, 
                             const BrushPtr& brush, 
                             const Graphucks::Point& origin)
{
    DrawTextCommand command;
    command.DeviceContext(m_d2dDeviceContext);
    command.Brush(brush);
    command.Origin(origin);
    command.TextLayout(textLayout);

    command.Run();
}

auto RenderContext::MaxiumImageSize() const -> const int
{
    return m_d2dDeviceContext->GetMaximumBitmapSize();
}

void RenderContext::DrawMediaPlayer( const std::shared_ptr<MediaPlayer>& mediaPlayer, const Graphucks::Size& size )
{
    auto surface = mediaPlayer->GetTexture(m_device);

    if(!surface)
        return;

    ComPtr<ID2D1Bitmap1> bitmap;
    HRESULT hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(surface.Get(), nullptr, bitmap.GetAddressOf());

    if(FAILED(hr))
        return;
    D2D1_RECT_F area = {0,0,size.width, size.height};
    m_d2dDeviceContext->DrawBitmap(bitmap.Get(), &area);
}
