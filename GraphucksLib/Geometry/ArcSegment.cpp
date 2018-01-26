#include "pch.h"
#include "ArcSegment.h"

using namespace Graphucks;

ArcSegment::ArcSegment(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize) :
     m_point(point), m_size(size), m_rotation(rotation), m_sweepDirection(sweepDirection), m_arcSize(arcSize)
{
}


ArcSegment::~ArcSegment(void)
{
}

void ArcSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->AddArc(D2D1::ArcSegment(m_point, m_size, m_rotation, (D2D1_SWEEP_DIRECTION)m_sweepDirection, (D2D1_ARC_SIZE)m_arcSize));
}
