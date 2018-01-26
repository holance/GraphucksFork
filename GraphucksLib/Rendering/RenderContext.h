#pragma once

#include "IRenderContext.h"
#include "FillCommand.h"
#include "StrokeCommand.h"
#include "RenderState.h"

#include "..\Brushes\RadialGradientBrush.h"
#include "..\Brushes\GradientStopCollection.h"
#include "..\Media\Bitmap.h"
#include "..\Media\MediaPlayer.h"
#include "..\Effects\EffectResource.h"
#include "..\Effects\ElementEffect.h"
#include "..\Brushes\Pen.h"
#include "..\Text\TextLayout.h"

#include <memory>
#include <wrl.h>
#include <queue>
#include <d3d11.h>
#include <dwrite_1.h>


namespace Graphucks
{
    class RenderContext;
	typedef std::shared_ptr<RenderContext> RenderContextPtr;

    class RenderContext : public IRenderContext
    {
        typedef Microsoft::WRL::ComPtr<IDXGISurface>        IDXGISurfacePtr;
        typedef Microsoft::WRL::ComPtr<ID3D11Device>        ID3D11DevicePtr;
        typedef Microsoft::WRL::ComPtr<ID2D1Device>         ID2D1DevicePtr;
        typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext>  ID2D1DeviceContextPtr;
        typedef Microsoft::WRL::ComPtr<ID2D1Factory1>       ID2D1Factory1Ptr;
        typedef Microsoft::WRL::ComPtr<IDWriteFactory>      IDWriteFactoryPtr;
    public:
        RenderContext(const ID3D11DevicePtr& device);
        ~RenderContext(void);
    public:
        auto PushRenderSurface(const IDXGISurfacePtr& surface)    -> HRESULT;
        auto GetTarget()                                          -> BitmapPtr&;
                                                                  
        auto GetDevice(ID3D11DevicePtr& device)                   -> HRESULT;
        auto Get2DDevice(ID2D1DevicePtr& device)                  -> HRESULT;
                                                                  
        auto Get2DDeviceContext() const                           -> const ID2D1DeviceContextPtr&;
        auto GetCurrentRenderTarget()                             -> void *;
        auto GetRenderBitmap()                                    -> BitmapPtr;                                                   
        static auto GetFactory()                                  -> const ID2D1Factory1Ptr& ;
        static auto GetDirectWriteFactory()                       -> const IDWriteFactoryPtr;
    public:
        virtual auto CreateBitmap(const IDXGISurfacePtr& surface) -> BitmapPtr;
    public:
        virtual void Clear(const Color& c);
        virtual void DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const PenPtr& pen);
        virtual void SetTransform(const D2D1::Matrix3x2F& matrix);
        virtual void SetRenderTarget(const BitmapPtr& renderTarget);
        virtual void DrawText(const TextLayoutPtr& textLayout, const BrushPtr& brush, const Graphucks::Point& origin);
        virtual void DrawBitmap(const BitmapPtr& bitmap, const EffectCompositeMode compositeMode, const Graphucks::Size& targetSize);
        virtual void DrawMediaPlayer(const std::shared_ptr<MediaPlayer>& mediaPlayer,  const Graphucks::Size& size );
        void BeginClip(const Graphucks::Rectangle& clipRect);
        void EndClip();

        void PushRenderTarget(const BitmapPtr& renderTarget, bool clear = true);
        void PopRenderTarget();

        void DrawEffect(const BitmapPtr& bitmap, const ElementEffectPtr& elementEffect, const EffectCompositeMode compositeMode, const Graphucks::Size& size);

        auto MaxiumImageSize() const -> const int;

    private:
        void Initialize();
    private:
        static Microsoft::WRL::ComPtr<ID2D1Factory1>  m_d2dFactory;
        static Microsoft::WRL::ComPtr<IDWriteFactory> m_directWriteFactory;
        Microsoft::WRL::ComPtr<ID3D11Device>          m_device;
        Microsoft::WRL::ComPtr<ID2D1Device>           m_d2dDevice;
        Microsoft::WRL::ComPtr<ID2D1DeviceContext>    m_d2dDeviceContext;
        std::deque<RenderState>                       m_renderStateQueue;
        void * m_lastRenderTarget;
    };
}
