#pragma once
#include "ISegment.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class ArcSegment : public ISegment
    {
    public:
        ArcSegment(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize);
        virtual ~ArcSegment(void);
    protected:
        virtual void Create(const ID2D1GeometrySinkPtr& sink) override;
    private:
        Point           m_point;
        Size            m_size;
        float           m_rotation;
        SweepDirection  m_sweepDirection;
        ArcSize         m_arcSize;
    };
}
