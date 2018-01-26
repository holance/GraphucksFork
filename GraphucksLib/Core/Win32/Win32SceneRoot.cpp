#include "pch.h"
#include "Win32SceneRoot.h"
#include <wrl.h>
#if !defined(__cplusplus_winrt)
#include <Windowsx.h>

using namespace std;
using namespace KennyKerr;
using namespace Graphucks;
using namespace Microsoft::WRL;

Win32SceneRoot::Win32SceneRoot(void) : m_hwnd(nullptr)
{
    m_device = Direct3D::CreateDevice();
    m_renderContext = make_shared<RenderContext>(m_device.Get());
    m_sceneRoot     = make_shared<SceneRoot>(m_renderContext);
}

Win32SceneRoot::~Win32SceneRoot(void)
{
    if(m_hwnd)
        KillTimer(m_hwnd, (UINT_PTR)this);
}

void  __stdcall Win32SceneRoot::TimerCallback(HWND hwnd, UINT p1, UINT_PTR p2, DWORD p3)
{
    auto instance = static_cast<Win32SceneRoot*>((void*)p2);
    
    if (!instance->m_sceneRoot->IsSceneDirty())
    {
        return;
    }

    InvalidateRect(instance->m_hwnd, nullptr, false);
}

void Win32SceneRoot::SetHwnd( HWND hwnd )
{
    if(m_hwnd)
    {
        KillTimer(m_hwnd, (UINT_PTR)this);
    }

    m_hwnd = hwnd;
    
    auto dxgi = m_device.GetDxgiFactory();

    Dxgi::SwapChainDescription1 description;

    m_swapChain = dxgi.CreateSwapChainForHwnd(m_device,
                                              m_hwnd,
                                              description);

    SetTimer(m_hwnd, (UINT_PTR)this, 16, TimerCallback);
}

void Win32SceneRoot::Present()
{
    if(!m_swapChain)
        return;

    if (!m_sceneRoot->IsSceneDirty())
    {
        m_swapChain.Present();
        return;
    }

    auto drawSurface = m_swapChain.GetBuffer(0);

    DXGI_SURFACE_DESC desc;
    drawSurface->GetDesc(&desc);

    m_renderContext->PushRenderSurface(drawSurface.Get());

    m_sceneRoot->Render();

    m_renderContext->PopRenderTarget();

    HRESULT hr = m_swapChain.Present();
}

void Win32SceneRoot::SetRoot( const VisualElementPtr& rootElement )
{
    m_sceneRoot->RootElement(rootElement);
}

int Win32SceneRoot::ProcessMessage( HWND window, UINT message, WPARAM wparam, LPARAM lparam )
{
    if(window != m_hwnd)
        SetHwnd(window);

    if (WM_PAINT == message)
    {
        PAINTSTRUCT ps;
        VERIFY(BeginPaint(window, &ps));
        m_sceneRoot->ForceDirty();
        Present();
        EndPaint(window, &ps);
        return 0;
    }

    if (WM_SIZE == message)
    {
        if (SIZE_MINIMIZED != wparam)
        {
            m_sceneRoot->ForceDirty();
            m_swapChain.ResizeBuffers();
            //Present();
        }
            
        return 0;
    }

    if(WM_ERASEBKGND == message)
    {
        return 0;
    }

    if (WM_DISPLAYCHANGE == message)
    {
        Present();
        return 0;
    }

    if(WM_LBUTTONUP == message)
    {
        auto x = GET_X_LPARAM(lparam);
        auto y = GET_Y_LPARAM(lparam);
        m_sceneRoot->SendOnPointerDown(Point(x, y));
        return -1;
    }

    return -1;
}

#endif