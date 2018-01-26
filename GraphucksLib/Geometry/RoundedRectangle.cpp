#include "pch.h"
#include "RoundedRectangle.h"
#include "..\Core\GraphicsTypes.h"
#include "Utils.h"

using namespace Graphucks;
using namespace D2D1;
using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1Geometry> RoundedRectangle::OnCreateGeometry()
{
    ComPtr<ID2D1RoundedRectangleGeometry> geometry;

    D2D1_ROUNDED_RECT rect;
    rect.radiusX    = m_RX;
    rect.radiusY    = m_RY;
    rect.rect       = m_rect;

    m_Factory->CreateRoundedRectangleGeometry(rect, &geometry);
    
    return geometry;
}

void RoundedRectangle::RadiusX(float rx)
{
    m_RX = rx;
    Invalidate();
}

auto RoundedRectangle::RadiusX() const -> float
{
    return m_RX;
}

auto RoundedRectangle::RadiusY() const -> float
{
    return m_RY;
}

void RoundedRectangle::RadiusY(float ry)
{
    m_RY = ry;
    Invalidate();
}

void RoundedRectangle::Rect(Graphucks::Rectangle rect)
{
    m_rect = rect;
    Invalidate();
}

auto RoundedRectangle::Rect() const -> Graphucks::Rectangle 
{
    return m_rect;
}
