#pragma once
#include <memory>

#include "..\Core\GraphicsTypes.h"
#include "..\Geometry\Ellipse.h"
#include "..\Brushes\SolidColorBrush.h"
#include "..\Brushes\GradientStopCollection.h"
#include "..\Brushes\RadialGradientBrush.h"
#include "..\Media\Bitmap.h"
#include "..\Media\MediaPlayer.h"
#include "..\Brushes\ImageBrush.h"
#include "..\Brushes\LinearGradientBrush.h"
#include "..\Effects\EffectResource.h"
#include "..\Text\TextLayout.h"

namespace Graphucks
{
    class IRenderContext;
	typedef std::shared_ptr<IRenderContext> IRenderContextPtr;

    class IRenderContext
    {
    public:
        virtual void Clear(const Color& c) = 0;
        virtual void DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const PenPtr& pen) = 0;
        virtual void DrawText(const TextLayoutPtr& textLayout, const BrushPtr& brush, const Graphucks::Point& origin) = 0;
        virtual void SetTransform(const D2D1::Matrix3x2F& matrix) = 0;
        virtual void BeginClip(const Graphucks::Rectangle& clipRect) = 0;
        virtual void EndClip() = 0;
        virtual void DrawBitmap(const BitmapPtr& bitmap, const EffectCompositeMode compositeMode, const Size& size) = 0;
        virtual void DrawMediaPlayer(const std::shared_ptr<MediaPlayer>& mediaPlayer, const Size& size) = 0;
    };
}