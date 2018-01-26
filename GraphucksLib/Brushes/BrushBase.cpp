#include "pch.h"
#include "pen.h"
#include "BrushBase.h"
#include "GeometryBase.h"

using namespace Graphucks;
using namespace D2D1;

Brush::Brush() : m_Opacity(1.0f), 
                         m_transform(Matrix3x2F::Identity())
{
}

auto Brush::GetTransformToGeometryBounds(const GeometryPtr& /*geometry*/, 
                                             const PenPtr& /*pen*/) -> Matrix3x2F
{
    return Matrix3x2F::Identity();
}

auto Brush::GetTransformToBounds(const Graphucks::Rectangle& /*rect*/) -> D2D1::Matrix3x2F
{
    return Matrix3x2F::Identity();
}

void Brush::Opacity(const float o)
{
    m_Opacity = o;

    if(IsResourceValid())
    {
        GetResourceUnsafe<ID2D1Brush>()->SetOpacity(0);
    }
}

void Brush::Transform(const Matrix3x2F& transform)
{
    m_transform = transform;

    if(IsResourceValid())
    {
        GetResourceUnsafe<ID2D1Brush>()->SetTransform(transform);
    }
}

auto Brush::Transform() const -> const Matrix3x2F&
{
    return m_transform;
}

auto Brush::Opacity() const -> float
{
    return m_Opacity;
}

