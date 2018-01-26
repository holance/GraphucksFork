#pragma comment(lib, "user32.lib")
#pragma warning(disable: 4706)
#include "stdafx.h"
#include <Graphucks.h>
#include <memory>
#include "GraphucksDemo.h"

using namespace std;
using namespace Graphucks;

unique_ptr<GraphucksDemo> demo;
shared_ptr<Win32SceneRoot> sceneRoot;

int __stdcall wWinMain(HINSTANCE module, HINSTANCE, PWSTR, int)
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    WNDCLASS wc = {};
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = module;
    wc.lpszClassName = L"window";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    wc.lpfnWndProc = [] (HWND window, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        if(!sceneRoot)
        {
           sceneRoot = make_shared<Win32SceneRoot>();
           demo = unique_ptr<GraphucksDemo>(new GraphucksDemo(sceneRoot));
        }

        if((WM_SIZE == message || WM_PAINT == message) && demo)
            demo->Update();

        auto ret = sceneRoot->ProcessMessage(window, message, wparam, lparam);

        //if(ret == 0)
       //     return 0;

        if (WM_DESTROY == message)
        {
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(window, message, wparam, lparam);
    };

    RegisterClass(&wc);

    CreateWindow(wc.lpszClassName, L"Graphucks Win32 Demo", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                  nullptr, nullptr, module, nullptr);

    MSG message;
    BOOL result;

    while (result = GetMessage(&message, 0, 0, 0))
    {
        if (-1 != result) DispatchMessage(&message);
    }

#if _DEBUG
    sceneRoot = nullptr;
    demo = nullptr;
#endif

    return 0;
}
