#include "pch.h"
#include "Bitmap.h"
#include "Utils.h"
#include <shcore.h>
#include <memory>

using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace Concurrency;
using namespace std;

ComPtr<IUnknown> Bitmap::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
    ComPtr<ID2D1Bitmap1> bitmap;

    if(m_bitmapSource)
    {
        Check(deviceContext->CreateBitmapFromWicBitmap(m_bitmapSource.Get(), &bitmap));
    }
    else if(m_surface)
    {
        D2D1_BITMAP_PROPERTIES1 props;
        float dpiX, dpiY;
        deviceContext->GetDpi(&dpiX, &dpiY);
        
        props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, 
                                        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, 
                                                          D2D1_ALPHA_MODE_PREMULTIPLIED), 
                                        dpiX, 
                                        dpiY, 
                                        nullptr);

        HRESULT hr = deviceContext->CreateBitmapFromDxgiSurface(m_surface.Get(), &props, &bitmap);
        
        Check(hr);
        
        DXGI_SURFACE_DESC desc;
        m_surface->GetDesc(&desc);
        m_size.width = (float)desc.Width;
        m_size.height = (float)desc.Height;
    }
    else
    {
        D2D1_SIZE_U size = {(int)m_size.width, (int)m_size.height};
        D2D1_BITMAP_PROPERTIES1 props;
        float dpiX, dpiY;
        deviceContext->GetDpi(&dpiX, &dpiY);
        
        props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, 
                                        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, 
                                                          D2D1_ALPHA_MODE_PREMULTIPLIED), 
                                        dpiX, 
                                        dpiY, 
                                        nullptr);

        ComPtr<ID2D1Bitmap1> bmp = nullptr;
        HRESULT hr = deviceContext->CreateBitmap(size, nullptr, 0, props, &bmp);

        Check(hr);
        return bmp;
    }

    return bitmap;
}

Bitmap::Bitmap(const WICBitmapSourcePtr& bitmapSource)
{
    m_bitmapSource = bitmapSource;

    D2D1_SIZE_U size;

    m_bitmapSource->GetSize(&size.width, &size.height);

    m_size.width = (float)size.width;
    m_size.height = (float)size.height;
}

Bitmap::Bitmap(const ComPtr<IDXGISurface>& surface)
{
    m_surface = surface;
}

Bitmap::Bitmap(const Graphucks::Size& size)
{
    m_size = size;
}

const Graphucks::Size& Bitmap::GetSize() const
{
    return m_size;
}

#if defined(__cplusplus_winrt)
Concurrency::task<BitmapPtr> Bitmap::LoadFromApplicationUrl( Windows::Foundation::Uri^ uri )
{
    return Concurrency::task<BitmapPtr>([=]
    {
        using namespace Platform;
        using namespace Windows::Storage;
        using namespace Windows::Storage::Streams;

        return create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([=](task<StorageFile^> task) 
	    {
		    auto storageFile = task.get();
		    return storageFile->OpenAsync(FileAccessMode::Read);

	    }, task_continuation_context::use_arbitrary()).then([=](task<IRandomAccessStream^> openAsyncTask) mutable
        {
            ComPtr<IWICImagingFactory> pFactory;
            ComPtr<IWICBitmapDecoder>  pDecoder; 
            ComPtr<IStream>            istream;

            HRESULT hr = CoCreateInstance(
                CLSID_WICImagingFactory,
                NULL,
                CLSCTX_INPROC_SERVER,
                IID_IWICImagingFactory,
                (LPVOID*)&pFactory);

            if(FAILED(hr))
                throw std::exception("Could not create WIC factory");
            
            auto fileStream = openAsyncTask.get();
            hr = CreateStreamOverRandomAccessStream(reinterpret_cast<IUnknown*>(fileStream),
                                                    IID_PPV_ARGS(&istream));

        
            if(FAILED(hr))
               throw std::exception("Could not CreateStreamOverRandomAccessStream");

            ComPtr<IWICBitmapDecoder> decoder;
            pFactory->CreateDecoderFromStream(
                    istream.Get(),
                    nullptr,
                    WICDecodeMetadataCacheOnDemand,
                    &decoder
                    );

            ComPtr<IWICBitmapFrameDecode> frame;
            hr = decoder->GetFrame(0, &frame);

            if(FAILED(hr))
               throw std::exception("Could not CreateStreamOverRandomAccessStream");

            ComPtr<IWICFormatConverter> converter;
            hr = pFactory->CreateFormatConverter(&converter);
            
            if(FAILED(hr))
               throw std::exception("Could not CreateFormatConverter");

            hr = converter->Initialize(
                frame.Get(),
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                nullptr,
                0.0f,
                WICBitmapPaletteTypeCustom
                );

             if(FAILED(hr))
               throw std::exception("Could not initialize WIC converter");

            auto bitmap = make_shared<Bitmap>(converter);

            return bitmap;
        });
    });
}
#else
Concurrency::task<BitmapPtr> Graphucks::Bitmap::LoadFromPathAsync( std::wstring& path )
{
    return Concurrency::task<BitmapPtr>([=]
    {
        ComPtr<IWICImagingFactory> pFactory;
        ComPtr<IWICBitmapDecoder>  pDecoder; 
        ComPtr<IStream>            istream;

        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            (LPVOID*)&pFactory);

        if(FAILED(hr))
            throw std::exception("Could not create WIC factory");
   
        ComPtr<IWICBitmapDecoder> decoder;
        hr = pFactory->CreateDecoderFromFilename(
                path.c_str(),
                nullptr,
                GENERIC_READ,
                WICDecodeMetadataCacheOnDemand,
                &decoder
                );

        ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);

        if(FAILED(hr))
            throw std::exception("Could not CreateStreamOverRandomAccessStream");

        ComPtr<IWICFormatConverter> converter;
        hr = pFactory->CreateFormatConverter(&converter);
            
        if(FAILED(hr))
            throw std::exception("Could not CreateFormatConverter");

        hr = converter->Initialize(
            frame.Get(),
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0f,
            WICBitmapPaletteTypeCustom
            );

            if(FAILED(hr))
            throw std::exception("Could not initialize WIC converter");

        auto bitmap = make_shared<Bitmap>(converter);

        return bitmap;
    });
}

BitmapPtr Bitmap::LoadFromPath( std::wstring& path )
{
    ComPtr<IWICImagingFactory> pFactory;
    ComPtr<IWICBitmapDecoder>  pDecoder; 
    ComPtr<IStream>            istream;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        (LPVOID*)&pFactory);

    if(FAILED(hr))
        throw std::exception("Could not create WIC factory");
   
    ComPtr<IWICBitmapDecoder> decoder;
    hr = pFactory->CreateDecoderFromFilename(
            path.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &decoder
            );

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);

    if(FAILED(hr))
        throw std::exception("Could not CreateStreamOverRandomAccessStream");

    ComPtr<IWICFormatConverter> converter;
    hr = pFactory->CreateFormatConverter(&converter);
            
    if(FAILED(hr))
        throw std::exception("Could not CreateFormatConverter");

    hr = converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeCustom
        );

        if(FAILED(hr))
        throw std::exception("Could not initialize WIC converter");

    auto bitmap = make_shared<Bitmap>(converter);

    return bitmap;
}
#endif
