#pragma once
#include <ppltasks.h>
#include <wincodec.h>

#include "Image.h"
#include "..\Core\GraphicsTypes.h"
namespace Graphucks
{
    class Bitmap;
    typedef std::shared_ptr<Graphucks::Bitmap>       BitmapPtr;

    typedef Microsoft::WRL::ComPtr<IWICBitmapSource> WICBitmapSourcePtr;

    class Bitmap: public Image
    {
    public:
        Bitmap(const WICBitmapSourcePtr& bitmapSource);
        Bitmap(const Microsoft::WRL::ComPtr<IDXGISurface>& surface);
        Bitmap(const Graphucks::Size& size);
        virtual ~Bitmap(){}
    public:
#if defined(__cplusplus_winrt)
        static Concurrency::task<BitmapPtr> LoadFromApplicationUrl(Windows::Foundation::Uri^ uri);
#else
        static Concurrency::task<BitmapPtr> LoadFromPathAsync(std::wstring& path);
        static BitmapPtr Bitmap::LoadFromPath( std::wstring& path );
#endif
    public:
        auto GetSize() const -> const Graphucks::Size&;
    private:
        virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext) -> Microsoft::WRL::ComPtr<IUnknown>;
    private:
        mutable Graphucks::Size              m_size;
        WICBitmapSourcePtr                   m_bitmapSource;
        Microsoft::WRL::ComPtr<IDXGISurface> m_surface;
    };
}