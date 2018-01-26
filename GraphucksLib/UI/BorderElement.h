#pragma once
#include <vector>
#include "VisualElement.h"
#include "ContentControlElement.h"
#include "..\Geometry\PathGeometry.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    struct CornerRadius 
    {
        CornerRadius() : BottomLeft(0), TopLeft(0), BottomRight(0), TopRight(0)
        { }

        CornerRadius(float bottomLeft, float topLeft, float bottomRight, float topRight) : 
            BottomLeft(bottomLeft), TopLeft(topLeft), BottomRight(bottomRight), TopRight(topRight)
        { }

        float BottomLeft;
        float BottomRight;
        float TopLeft;
        float TopRight;
    };

    class BorderElement : public VisualElement
    {
        typedef std::vector<GeometryPtr> GeometryBaseVector;

    public:
        static const DependencyPropertyPtr BorderBrushProperty;
        static const DependencyPropertyPtr BackgroundProperty;
    public:
        BorderElement(void);
        virtual ~BorderElement(void);

        void BorderBrush(const BrushPtr& borderBrush);
        const BrushPtr BorderBrush();

        void Background(const BrushPtr& backgroundBrush);
        const BrushPtr Background();

        void Padding(const Graphucks::Thickness& thickness);
        const Thickness Padding();

        auto CornerRadii() const -> Graphucks::CornerRadius;
        void CornerRadii(Graphucks::CornerRadius val);

        float BorderThickness() const;
        void BorderThickness(float thickness);

    public:
        auto GetContent() -> const VisualElementPtr&;
        void SetContent(const VisualElementPtr& content);
    protected:
		virtual void RenderOverride(const IRenderContextPtr& renderContext) override;
 protected:
        virtual Graphucks::Size MeasureOverride(const Graphucks::Size& availableSize) override;
        virtual Graphucks::Size ArrangeOverride(const Graphucks::Size& finalSize) override;
    private:
        void CreateGeometry();
    private:
        CornerRadius        m_cornerRadius;
        PathGeometryPtr     m_pathGeometry;
        Thickness           m_borderThickness;
        Thickness           m_padding;
        PenPtr              m_pen;
        Size                m_lastSize;
        GeometryBaseVector  m_geometries;
    };
}

