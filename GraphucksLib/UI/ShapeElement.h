#pragma once
#include "VisualElement.h"
#include "..\Core\GraphicsTypes.h"
#include "..\Rendering\IRenderContext.h"

namespace Graphucks
{
    class ShapeElement : public VisualElement
    {
    public:
         static const DependencyPropertyPtr FillBrushProperty;
         static const DependencyPropertyPtr StrokeBrushProperty;
    private:
        static void FillBrushPropertyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
        static void StrokeBrushPropertyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
    public:
        ShapeElement(void);
        virtual ~ShapeElement(void);
    public:
        void StrokeWidth(float thickness);
        auto StrokeWidth() -> float;

        void StrokeBrush(const BrushPtr& brush);
        auto StrokeBrush() -> const BrushPtr;

        void FillBrush(const BrushPtr& brush);
        auto FillBrush() -> const BrushPtr;

        void SetStretch(const Stretch stretch);
        auto GetStretch() -> const Stretch;

        void GeometryCacheEnabled(bool enabled);
        auto GeometryCacheEnabled() -> bool;
    protected:
        auto GetPen() -> const PenPtr&;
    protected:
        auto GetBounds() -> D2D1_RECT_F;
        void SetGeometry(GeometryPtr geometry);
        auto GetGeometry() -> GeometryPtr;

        virtual bool HitTest(const Point& point) override;
    private:
        GeometryPtr m_geometry;
        float       m_strokeThickness;
        BrushPtr    m_strokeBrush;
        Stretch     m_stretch;
        PenPtr      m_pen;
        bool        m_penDirty;
        bool        m_geometryCacheEnabled;
    };
}
