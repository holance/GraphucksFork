
#include "pch.h"
#include "XamlSurfaceRenderer.h"
#if defined(__cplusplus_winrt)

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Graphucks;
namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {            
            throw Platform::Exception::CreateException(hr);
        }
    }
}

XamlSurfaceRenderer::XamlSurfaceRenderer(const Microsoft::WRL::ComPtr<ID3D11Device>& device, 
                                         int pixelWidth, 
                                         int pixelHeight, 
                                         bool isOpaque) :
    SurfaceImageSource(pixelWidth, 
                       pixelHeight, 
                       isOpaque), 
                       m_d3dDevice(device), 
                       m_width(pixelWidth), 
                       m_height(pixelHeight)
{
    CreateDeviceIndependentResources();
    CreateDeviceResources();
}

void XamlSurfaceRenderer::CreateDeviceIndependentResources()
{
    DX::ThrowIfFailed(
                        reinterpret_cast<IUnknown*>(this)->QueryInterface(IID_PPV_ARGS(&m_sisNative))
                     );
}

void XamlSurfaceRenderer::CreateDeviceResources()
{
    ComPtr<IDXGIDevice> dxgiDevice;

    DX::ThrowIfFailed(
                        m_d3dDevice.As(&dxgiDevice)
                     );

    
    DX::ThrowIfFailed(
                        m_sisNative->SetDevice(dxgiDevice.Get())
                     );
}

void XamlSurfaceRenderer::BeginDraw(Microsoft::WRL::ComPtr<IDXGISurface>& pSurface)
{
    RECT updateRectNative = {0}; 
    updateRectNative.right = m_width;
    updateRectNative.bottom = m_height;
    POINT offset;
    HRESULT beginDrawHR = m_sisNative->BeginDraw(updateRectNative, 
                                                 pSurface.ReleaseAndGetAddressOf(), 
                                                 &offset);

    //DX::ThrowIfFailed(beginDrawHR);
}

void XamlSurfaceRenderer::EndDraw()
{
     DX::ThrowIfFailed(
                        m_sisNative->EndDraw()
                      );
}

#endif