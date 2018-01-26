#include "pch.h"
#include "Ellipse.h"
#include "Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1Geometry> Ellipse::OnCreateGeometry()
{
    ComPtr<ID2D1EllipseGeometry> ellipse;
    Check(m_Factory->CreateEllipseGeometry(D2D1::Ellipse(m_Center, m_RX, m_RY), &ellipse));
    return ellipse;
}

void Ellipse::RadiusX(float rx)
{
    m_RX = rx;
    Invalidate();
}

auto Ellipse::RadiusX() const -> float
{
    return m_RX;
}

auto Ellipse::RadiusY() const -> float
{
    return m_RY;
}

void Ellipse::RadiusY(float ry)
{
    m_RY = ry;
    Invalidate();
}

void Ellipse::Center(const Graphucks::Point& center)
{
    m_Center = center;
    Invalidate();
}

auto Ellipse::Center() const -> Graphucks::Point
{
    return m_Center;
}
