#pragma once

#if defined(__cplusplus_winrt)

#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include "windows.ui.xaml.media.dxinterop.h"

namespace Graphucks
{
    ref class XamlSurfaceRenderer sealed : Windows::UI::Xaml::Media::Imaging::SurfaceImageSource
    {
    internal:
        XamlSurfaceRenderer(const Microsoft::WRL::ComPtr<ID3D11Device>& device, int pixelWidth, int pixelHeight, bool isOpaque);
    internal:
        void BeginDraw(Microsoft::WRL::ComPtr<IDXGISurface>& pSurface);
        void EndDraw(void);
    private:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();
    private:
        Microsoft::WRL::ComPtr<ISurfaceImageSourceNative>   m_sisNative;
        Microsoft::WRL::ComPtr<ID3D11Device>                m_d3dDevice;
        int                                                 m_width;
        int                                                 m_height;
    };
}
#endif