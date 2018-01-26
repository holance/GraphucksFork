#include "pch.h"
#include "QuadraticBezierSegment.h"

using namespace Graphucks;

QuadraticBezierSegment::QuadraticBezierSegment(const Point& point1, const Point& point2)
    : m_point1(point1), m_point2(point2)
{
}


QuadraticBezierSegment::~QuadraticBezierSegment(void)
{
}

void QuadraticBezierSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(m_point1, m_point2));
}
