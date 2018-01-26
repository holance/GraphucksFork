#include "pch.h"
#include "CompositeTransform.h"

using namespace Graphucks;
using namespace D2D1;

CompositeTransform::CompositeTransform(void) : m_isDirty(true),
                                                m_centerX(0),
                                                m_centerY(0),
                                                m_scaleX(1),
                                                m_scaleY(1),
                                                m_skewX(0),
                                                m_skewY(0),
                                                m_translateX(0),
                                                m_translateY(0),
                                                m_rotation(0),
                                                m_calculatedMatrix(Matrix3x2F::Identity())
{

}

CompositeTransform::~CompositeTransform(void)
{
}

auto CompositeTransform::CenterX() const -> float
{
    return m_centerX;
}

void CompositeTransform::CenterX(float val)
{
    if(val != m_centerX)
    {
        m_centerX = val; 
        m_isDirty = true;
         NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::CenterY() const -> float
{
    return m_centerY;
}

void CompositeTransform::CenterY(float val)
{
    if(val != m_centerY)
    {
        m_centerY = val; 
        m_isDirty = true;
         NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::ScaleX() const -> float
{
    return m_scaleX;
}

void CompositeTransform::ScaleX(float val)
{
    if(val != m_scaleX)
    {
        m_scaleX = val; 
        m_isDirty = true;
         NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::ScaleY() const -> float
{
    return m_scaleY;
}

void CompositeTransform::ScaleY(float val)
{
    if(m_scaleY != val)
    {
        m_scaleY = val; 
        m_isDirty = true;
         NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::SkewX() const -> float
{
    return m_skewX;
}

void CompositeTransform::SkewX(float val)
{
    if(m_skewX != val)
    {
        m_skewX = val; 
        m_isDirty = true;
     NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::SkewY() const -> float
{
    return m_skewY;
}

void CompositeTransform::SkewY(float val)
{
    if(val != m_skewY)
    {
        m_skewY = val; 
        m_isDirty = true;
         NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::TranslateX() const -> float
{
    return m_translateX;
}

void CompositeTransform::TranslateX(float val)
{
    if(val != m_translateX)
    {
        m_translateX = val; 
        m_isDirty = true;
        NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::TranslateY() const -> float
{
    return m_translateY;
}

void CompositeTransform::TranslateY(float val)
{
    if(val != m_translateY)
    {
        m_translateY = val; 
        m_isDirty = true;
        NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

auto CompositeTransform::Rotation() const -> float
{
    return m_rotation;
}

void CompositeTransform::Rotation(float val)
{
    if(val != m_rotation)
    {
        m_rotation = val;
        m_isDirty = true;
        NotifyInvalidatables(INVALIDATE_FLAG_TRANSFORM);
    }
}

CompositeTransform::operator D2D1::Matrix3x2F()
{
    Calculate();
    return m_calculatedMatrix;
}

auto CompositeTransform::GetTransformMatrix() -> D2D1::Matrix3x2F&
{
    Calculate();
    return m_calculatedMatrix;
}

void Graphucks::CompositeTransform::Calculate()
{
    if(!m_isDirty)
        return;

    m_calculatedMatrix = D2D1::Matrix3x2F::Scale(m_scaleX, m_scaleY, D2D1::Point2F(m_centerX, m_centerY));

    m_calculatedMatrix = m_calculatedMatrix * D2D1::Matrix3x2F::Skew(m_skewX, m_skewY, D2D1::Point2F(m_centerX, m_centerY));

    m_calculatedMatrix = m_calculatedMatrix * D2D1::Matrix3x2F::Rotation(m_rotation, D2D1::Point2F(m_centerX, m_centerY));

    m_calculatedMatrix = m_calculatedMatrix * D2D1::Matrix3x2F::Translation(m_translateX, m_translateY);

    m_isDirty = false;
}
