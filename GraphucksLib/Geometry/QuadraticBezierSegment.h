#pragma once
#include "ISegment.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class QuadraticBezierSegment : public ISegment
    {
    public:
        QuadraticBezierSegment(const Point& point1, const Point& point2);
        virtual ~QuadraticBezierSegment(void);
    protected:
        virtual void Create(const ID2D1GeometrySinkPtr& sink) override;
    private:
        Point m_point1;
        Point m_point2;
    };
}


