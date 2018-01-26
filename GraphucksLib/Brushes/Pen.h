#pragma once
#include "BrushBase.h"
#include "StrokeStyle.h"

namespace Graphucks
{
    class Pen;
    typedef std::shared_ptr<Pen> PenPtr;

    class Brush;
    
    class Pen
    {
    public:
        Pen(void);
        virtual ~Pen(void);
    public:
        void Thickness(float thickness);
        auto Thickness() -> float;

        void Brush(const BrushPtr& brush);
        auto Brush() -> const BrushPtr&;

        const StrokeStylePtr& StrokeStyle() const;
        void StrokeStyle(const StrokeStylePtr& val);
    private:
        float          m_thickness;
        BrushPtr       m_brush;
        StrokeStylePtr m_strokeStyle;
    };
}
