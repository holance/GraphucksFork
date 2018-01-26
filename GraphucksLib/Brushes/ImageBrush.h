#pragma once
#include "BrushBase.h"
#include "..\Media\Image.h"
#include "..\Geometry\GeometryBase.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class ImageBrush;
    typedef std::shared_ptr<Graphucks::ImageBrush> ImageBrushPtr;

    class ImageBrush: public Brush
    {
    public:
        ImageBrush(const ImagePtr& image);
        ImageBrush();
        virtual ~ImageBrush(void);

        auto ExtentModeX() const -> ExtendMode;
        void ExtentModeX(ExtendMode extentMode);

        auto ExtentModeY() const -> ExtendMode;
        void ExtentModeY(ExtendMode extentMode);

        auto SourceRect() const -> Graphucks::Rectangle;
        void SourceRect(const Graphucks::Rectangle& rect);

        auto Image() -> ImagePtr;
        void Image(const ImagePtr image);

        auto Interpolation() const -> Graphucks::InterpolationMode;
        void Interpolation(Graphucks::InterpolationMode mode);

        virtual auto GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen) -> D2D1::Matrix3x2F override;
        virtual auto GetTransformToBounds(const Rectangle& rect) -> D2D1::Matrix3x2F override;
    protected:
        auto IsImageSet() -> bool;
        virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext) ->  Microsoft::WRL::ComPtr<IUnknown> override;
    private:
        ImagePtr             m_image;
        ExtendMode           m_extentModeX;
        ExtendMode           m_extentModeY;
        InterpolationMode    m_interpolationMode;
        mutable Graphucks::Rectangle m_sourceRect;
    };
}
