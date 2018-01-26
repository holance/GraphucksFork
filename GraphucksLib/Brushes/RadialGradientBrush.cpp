#include "pch.h"
#include "RadialGradientBrush.h"
#include "Utils.h"

using namespace Graphucks;
using namespace D2D1;
using namespace Microsoft::WRL;

ComPtr<IUnknown> RadialGradientBrush::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
    ComPtr<ID2D1RadialGradientBrush> brush;
    auto gsc = GetResourceFrom<ID2D1GradientStopCollection>(m_gradientStopCollection);

    Check(deviceContext->CreateRadialGradientBrush(
        D2D1::RadialGradientBrushProperties(m_center, 
                                            m_gradientOriginOffset, 
                                            m_radiusX, 
                                            m_radiusY), 
        D2D1::BrushProperties(m_Opacity, m_transform), 
        gsc, 
        &brush));
    
    return brush;
}

D2D1::Matrix3x2F RadialGradientBrush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen)
{
    D2D1_RECT_F geometryBounds;
 
    geometryBounds = geometry->GetBounds(pen, Matrix3x2F::Identity());

    D2D1_RECT_F brushRect;
    brushRect.left = m_center.x - m_radiusX;
    brushRect.right = m_center.x + m_radiusX;
    brushRect.top = m_center.y - m_radiusY;
    brushRect.bottom = m_center.y + m_radiusY;

    auto brushRectSize = SizeF(brushRect.right - brushRect.left, brushRect.bottom - brushRect.top);


    auto widthRatio = (geometryBounds.right - geometryBounds.left) / brushRectSize.width;
    auto heightRatio = (geometryBounds.bottom - geometryBounds.top) / brushRectSize.height;

    auto sourceCenterX = m_center.x;
    auto sourceCenterY =  m_center.y;

    auto destCenterX = (geometryBounds.right + geometryBounds.left) / 2;
    auto destCenterY =  (geometryBounds.bottom + geometryBounds.top) / 2;

    auto matrix = Matrix3x2F::Scale(widthRatio, heightRatio, D2D1::Point2F(sourceCenterX, sourceCenterY));
//    matrix = matrix *  Matrix3x2F::Translation((destCenterX - sourceCenterX) - widthRatio, (destCenterY - sourceCenterY) - heightRatio);
    matrix = matrix *  Matrix3x2F::Translation((destCenterX * 2) * sourceCenterX, (destCenterY * 2) * sourceCenterY);
    return matrix;
}

auto RadialGradientBrush::GetTransformToBounds(const Graphucks::Rectangle& rect) -> D2D1::Matrix3x2F
{
    auto bounds = rect;

    auto size = SizeF(1, 1);

    auto widthRatio = (bounds.right - bounds.left) / size.width;
    auto heightRatio = (bounds.bottom - bounds.top) / size.height;

    auto matrix = Matrix3x2F::Translation(bounds.left, bounds.top);
    matrix = matrix * Matrix3x2F::Scale(widthRatio, heightRatio, 
                                        D2D1::Point2F(bounds.left, bounds.top));
    return matrix;
}

auto RadialGradientBrush::GradientStopCollection() const -> const GradientStopCollectionPtr&
{
    return m_gradientStopCollection;
}

auto RadialGradientBrush::IsResourceValid() const -> bool
{
    if(!IsResourceValidFrom(m_gradientStopCollection))
        return false;

    return Brush::IsResourceValid();
}

void RadialGradientBrush::Center(Point& val)
{
    m_center = val; 

    if(IsResourceValid())
    {
        auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
        brush->SetCenter(m_center);
    }
}

Point RadialGradientBrush::Center() const
{
    return m_center;
}

void RadialGradientBrush::GradientOriginOffset(Point& val)
{
    m_gradientOriginOffset = val; 
    if(IsResourceValid())
    {
        auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
        brush->SetGradientOriginOffset(m_gradientOriginOffset);
    }
}

Point RadialGradientBrush::GradientOriginOffset() const
{
    return m_gradientOriginOffset;
}

void RadialGradientBrush::RadiusX(float val)
{
    m_radiusX = val;

    if(IsResourceValid())
    {
         auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
        brush->SetRadiusX(m_radiusX);
    }
}

float RadialGradientBrush::RadiusX() const
{
    return m_radiusX;
}

void RadialGradientBrush::RadiusY(float val)
{
    m_radiusY = val; 
    if(IsResourceValid())
    {
        auto brush = GetResourceUnsafe<ID2D1RadialGradientBrush>();
        brush->SetRadiusY(m_radiusY);
    }
}

float RadialGradientBrush::RadiusY() const
{
    return m_radiusY;
}
