#include "pch.h"
#include "BrushBase.h"
#include "Pen.h"

using namespace Graphucks;
using namespace std;
using namespace D2D1;

Pen::Pen(void) : m_thickness(0)
{
}

Pen::~Pen(void)
{
}

void Pen::Thickness(float thickness)
{
    m_thickness = thickness;
}

auto Pen::Thickness() -> float
{
    return m_thickness;
}

void Pen::Brush(const BrushPtr& brush)
{
    m_brush = brush;
}

auto Pen::Brush() -> const BrushPtr& 
{
    return m_brush;
}

const StrokeStylePtr& Pen::StrokeStyle() const
{
    return m_strokeStyle;
}

void Pen::StrokeStyle(const StrokeStylePtr& val)
{
    m_strokeStyle = val;
}
