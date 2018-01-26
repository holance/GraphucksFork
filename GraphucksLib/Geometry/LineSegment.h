#pragma once
#include "..\Core\GraphicsTypes.h"
#include "GeometryBase.h"
#include "ISegment.h"

namespace Graphucks
{
    class LineSegment: public ISegment
    {
    public:
        explicit LineSegment(const Point& p): m_P(p){}
        virtual ~LineSegment(){}
    protected:
        virtual void Create(const ID2D1GeometrySinkPtr& sink) override;
    private:
        Point m_P;
    };
}
