#include "pch.h"
#include "LineSegment.h"

using namespace Graphucks;

void LineSegment::Create(const ID2D1GeometrySinkPtr& sink)
{
    sink->AddLine(m_P);
    Invalidate();
}