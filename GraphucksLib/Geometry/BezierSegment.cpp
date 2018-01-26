#include "pch.h"
#include "BezierSegment.h"

using namespace Graphucks;

void BezierSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->AddBezier(D2D1::BezierSegment(m_P1, m_P2, m_P3));
    Invalidate();
}
