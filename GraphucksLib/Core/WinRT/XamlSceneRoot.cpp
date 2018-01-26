#include "pch.h"
#if defined(__cplusplus_winrt)
#include "XamlSceneRoot.h"
#include "XamlSurfaceRenderer.h"
#include <wrl.h>
#include <memory>

#include "..\UI\ShapeElement.h"
#include "..\Effects\DropShadowEffect.h"

using namespace std;
using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Markup;

HRESULT CreateDXDevice(ComPtr<ID3D11Device>& device);

XamlSceneRoot::XamlSceneRoot( ) : m_backBufferSize(Graphucks::Size(1024, 768))
{
    auto controlTemplate = safe_cast<ControlTemplate^>    
                            (XamlReader::Load(L"<ControlTemplate xmlns=\"http://schemas.microsoft.com/winfx/2006/xaml/presentation\" \
                                                xmlns:x=\"http://schemas.microsoft.com/winfx/2006/xaml\"> \
                                                    <Image x:Name=\"rendererImage\"/> \
                                                 </ControlTemplate>"));
   
    Template = controlTemplate;
    
    CreateDXDevice(m_device);
    
    m_renderContext = make_shared<RenderContext>(m_device);
    m_sceneRoot     = make_shared<SceneRoot>(m_renderContext);

    m_loadedEventToken   = Loaded   += ref new RoutedEventHandler(this, &XamlSceneRoot::OnLoaded);
    m_unloadedEventToken = Unloaded += ref new RoutedEventHandler(this, &XamlSceneRoot::OnUnloaded);
}

void XamlSceneRoot::OnPointerPressed(Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
    auto pointerPoint = e->GetCurrentPoint(this);
    auto p = pointerPoint->Position;

    auto sceneRenderSize = m_backBufferSize;

    auto newX = (float)(sceneRenderSize.width  / m_image->ActualWidth)  * p.X;
    auto newY = (float)(sceneRenderSize.height / m_image->ActualHeight) * p.Y;

    m_sceneRoot->SendOnPointerDown(Graphucks::Point(newX, newY));
}

void XamlSceneRoot::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    m_renderCallbackToken = CompositionTarget::Rendering += ref new EventHandler<Platform::Object^>(this, &XamlSceneRoot::CompositionTargetRender);
}

void XamlSceneRoot::OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    CompositionTarget::Rendering -= m_renderCallbackToken;
}

void XamlSceneRoot::CompositionTargetRender(Platform::Object^ sender, Platform::Object^ e)
{
    if(!m_image)
        return;

    if (!m_sceneRoot->IsSceneDirty())
    {
        return;
    }

    ComPtr<IDXGISurface> drawSurface;

    m_surfaceRenderer->BeginDraw(drawSurface);   

    m_renderContext->PushRenderSurface(drawSurface);

    m_sceneRoot->Render();

    m_renderContext->PopRenderTarget();

    m_surfaceRenderer->EndDraw();
}

void XamlSceneRoot::OnPointerReleased( Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e )
{}

XamlSceneRoot::~XamlSceneRoot(void)
{}

void XamlSceneRoot::SetRoot(const VisualElementPtr& rootElement)
{
    m_sceneRoot->RootElement(rootElement);
}

void XamlSceneRoot::OnApplyTemplate()
{
    m_image = safe_cast<Windows::UI::Xaml::Controls::Image^>(GetTemplateChild("rendererImage"));

    CreateBackBuffer();

    m_image->Source = m_surfaceRenderer;

    FrameworkElement::OnApplyTemplate();
}

void XamlSceneRoot::SetBackBufferSize(const Graphucks::Size& size)
{
    if(m_backBufferSize == size)
        return;

    m_backBufferSize = size;

    CreateBackBuffer();
}

void XamlSceneRoot::CreateBackBuffer()
{
    auto width  = (int)m_backBufferSize.width;
    auto height = (int)m_backBufferSize.height;

    m_surfaceRenderer = ref new XamlSurfaceRenderer(m_device, width, height, false);

    if(m_image)
        m_image->Source = m_surfaceRenderer;
}

HRESULT CreateDXDevice(ComPtr<ID3D11Device>& device)
{
    HRESULT hr;
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
 creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
    D3D_FEATURE_LEVEL m_featureLevel;

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11DeviceContext> context;

    hr = D3D11CreateDevice(nullptr, // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        0,                          // leave as 0 unless software device
        creationFlags,              // optionally set debug and Direct2D compatibility flags
        featureLevels,              // list of feature levels this app can support
        ARRAYSIZE(featureLevels),   // number of entries in above list
        D3D11_SDK_VERSION,          // always set this to D3D11_SDK_VERSION for Metro style apps
        &device,                    // returns the Direct3D device created
        &m_featureLevel,            // returns feature level of device created
        &context                    // returns the device immediate context
       );

    if(FAILED(hr))
        throw ref new Platform::Exception(hr);

    ComPtr<ID3D10Multithread> multithread;
	
    hr = device.As(&multithread);
    if(FAILED(hr))
	    return hr;

    multithread->SetMultithreadProtected(true);
    return hr;
}
#endif