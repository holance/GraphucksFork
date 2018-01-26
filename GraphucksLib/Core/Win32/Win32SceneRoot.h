#pragma once
#include "..\..\Rendering\DX.h"
#include "..\SceneRoot.h"

#if !defined(__cplusplus_winrt)
namespace Graphucks
{
    class Win32SceneRoot
    {
    public:
        Win32SceneRoot(void);
        virtual ~Win32SceneRoot(void);

        void SetRoot(const VisualElementPtr& rootElement);
        int ProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
    private:
        void SetHwnd(HWND hwnd);
        void Present();

    private:
        static void  __stdcall TimerCallback(HWND hwnd, UINT p1, UINT_PTR p2, DWORD p3);
        std::shared_ptr<SceneRoot> m_sceneRoot;
        std::shared_ptr<RenderContext>                  m_renderContext;
        HWND m_hwnd;
        KennyKerr::Dxgi::SwapChain m_swapChain;
        KennyKerr::Direct3D::Device m_device;
        bool m_resizeHappend;
    };
}
#endif