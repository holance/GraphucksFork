#pragma once
#include "ShapeElement.h"
#include <string>

namespace Graphucks
{
    class PathElement;
    typedef std::shared_ptr<PathElement> PathElementPtr;

    class PathElement : public ShapeElement
    {
    public:
        PathElement(void);
        virtual ~PathElement(void);
    public:
        void Geometry(GeometryPtr geometry);
        auto Geometry() -> GeometryPtr;
    public:
        static GeometryPtr Parse(std::wstring& data);
    protected:
        virtual void RenderOverride(const IRenderContextPtr& renderContext) override;
        virtual auto MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size override ;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize) -> Graphucks::Size override ;
    private:
        auto GetStretchedRenderSize(Graphucks::Stretch mode, float strokeThickness, Graphucks::Size availableSize, const Graphucks::Rectangle& geometryBounds) -> Graphucks::Size ;
        void GetStretchSizeAttributes(const Graphucks::Stretch mode, const float strokeThickness, const Size& availableSize, const Graphucks::Rectangle& geometryBounds, float& xScale, float& yScale, float& dX, float& dY, Size& stretchedSize);
        auto GetRenderSizeTransformGeometry(Graphucks::Stretch mode, float strokeThickness, Graphucks::Size availableSize, const Graphucks::Rectangle& geometryBounds) -> Graphucks::Size;
    };
}
