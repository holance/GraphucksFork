#pragma once
#include "ISegment.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class BezierSegment: public ISegment
    {
    public:
        BezierSegment(const Point& p1, const Point& p2, const Point& p3): m_P1(p1), m_P2(p2), m_P3(p3){}
        virtual ~BezierSegment(){}
    protected:
        virtual void Create(const ID2D1GeometrySinkPtr& sink);
    private:
        Point m_P1;
        Point m_P2;
        Point m_P3;
    };
}
