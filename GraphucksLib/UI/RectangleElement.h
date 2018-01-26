#pragma once
#include "ShapeElement.h"
#include "..\Geometry\RoundedRectangle.h"

namespace Graphucks
{
    class RectangleElement : public ShapeElement
    {
    public:
        RectangleElement(void);
        virtual ~RectangleElement(void);
    public:
        auto RadiusX() -> float;
        void RadiusX(float rx);

        auto RadiusY() -> float;
        void RadiusY(float ry);
    protected:
        virtual void RenderOverride(const IRenderContextPtr& renderContext)                    override;
        virtual auto MeasureOverride(const Graphucks::Size& availableSize)  -> Graphucks::Size override;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize)     -> Graphucks::Size override;
    private:
        void CreateGeometry(const Graphucks::Size& size);
        RoundedRectanglePtr m_geometry;
        Graphucks::Size     m_lastSize;
        float               m_radiusX;
        float               m_radiusY;
    };
}

