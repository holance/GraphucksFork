#include "pch.h"
#include "MatrixTransform.h"

using namespace Graphucks;
 
MatrixTransform::MatrixTransform(void)
{
}

MatrixTransform::~MatrixTransform(void)
{
}

auto MatrixTransform::GetTransformMatrix() -> D2D1::Matrix3x2F&
{
    return m_matrix;
}

MatrixTransform::operator D2D1::Matrix3x2F()
{
    return m_matrix;
}

float MatrixTransform::M11()
{
    return m_matrix._11;
}

void MatrixTransform::M11( const float& val )
{
    m_matrix._11 = val;
}

float MatrixTransform::M12()
{
    return m_matrix._12;
}

void MatrixTransform::M12( const float& val )
{
    m_matrix._12 = val;
}

float MatrixTransform::M21()
{
    return m_matrix._21;
}

void MatrixTransform::M21( const float& val )
{
    m_matrix._21 = val;
}

float MatrixTransform::M22()
{
    return m_matrix._22;
}

void MatrixTransform::M22( const float& val )
{
    m_matrix._22 = val;
}

float MatrixTransform::OffsetX()
{
    return m_matrix._31;
}

void MatrixTransform::OffsetX( const float& val )
{
    m_matrix._31 = val;
}

float MatrixTransform::OffsetY()
{
    return m_matrix._32;
}

void MatrixTransform::OffsetY( const float& val )
{
    m_matrix._32 = val;
}
