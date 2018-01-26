#include "pch.h"
#include "ShapeElement.h"
#include "GeometryBase.h"
#include "VisualElement.h"

using namespace Graphucks;
using namespace D2D1;

void ShapeElement::FillBrushPropertyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
    auto shapeElement = dynamic_cast<ShapeElement*>(ptr);
}

const DependencyPropertyPtr ShapeElement::FillBrushProperty = DependencyProperty::Register<ShapeElement/*ownerType*/>(L"FillBrush", 
                                                                                              FrameworkPropertyMetadata::Create<BrushPtr/*dp type*/>(
                                                                                                  nullptr, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender,
                                                                                                  &FillBrushPropertyChanged)
                                                                                               );

void ShapeElement::FillBrush(const BrushPtr& brush)
{
    SetValue(FillBrushProperty, brush);
}

const BrushPtr ShapeElement::FillBrush()
{
    return GetValue<Brush>(FillBrushProperty);
}

void ShapeElement::StrokeBrushPropertyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
    auto shapeElement = dynamic_cast<ShapeElement*>(ptr);
    shapeElement->m_penDirty = true;
}

const DependencyPropertyPtr ShapeElement::StrokeBrushProperty = DependencyProperty::Register<ShapeElement/*ownerType*/>(L"StrokeBrush", 
                                                                                              FrameworkPropertyMetadata::Create<BrushPtr/*dp type*/>(
                                                                                                  nullptr, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender,
                                                                                                  &StrokeBrushPropertyChanged)
                                                                                               );

void ShapeElement::StrokeBrush(const BrushPtr& brush)
{
    SetValue(StrokeBrushProperty, brush);
}

const BrushPtr ShapeElement::StrokeBrush()
{
    return  GetValue<Brush>(StrokeBrushProperty);
}


ShapeElement::ShapeElement(void) : m_strokeThickness(0), m_stretch(Stretch::Fill), m_geometryCacheEnabled(false)
{
}


ShapeElement::~ShapeElement(void)
{
}

void ShapeElement::SetGeometry(GeometryPtr geometry)
{
    m_geometry = geometry;

    if(geometry)
    {
        m_geometry->GeometryCacheEnabled(m_geometryCacheEnabled);
    }
    InvalidateMeasure();
}

GeometryPtr ShapeElement::GetGeometry()
{
    return m_geometry;
}

D2D1_RECT_F ShapeElement::GetBounds()
{
    return m_geometry->GetBounds(GetPen(), Matrix3x2F::Identity());
}

void ShapeElement::StrokeWidth(float thickness)
{
    m_strokeThickness = thickness;
    m_penDirty = true;
    InvalidateMeasure();
}

float ShapeElement::StrokeWidth()
{
    return m_strokeThickness;
}

const PenPtr& ShapeElement::GetPen()
{
    auto strokeBrush = GetValue<Brush>(StrokeBrushProperty);

    if(!m_pen || m_penDirty)
    {
        if(!strokeBrush)
        {
            m_pen = nullptr;
        }
        else
        {
            m_pen = std::make_shared<Pen>();
            m_pen->Thickness(m_strokeThickness);
            m_pen->Brush(strokeBrush);
            m_penDirty = false;
        }
    }

    return m_pen;
}

void ShapeElement::SetStretch(const Stretch stretch)
{
    m_stretch = stretch;
    InvalidateMeasure();
}

const Stretch ShapeElement::GetStretch()
{
    return m_stretch;
}

bool ShapeElement::HitTest(const Point& point)
{
    bool result = false;

    if(!m_geometry)
        return false;

    if(FillBrush())
        result = m_geometry->HitTest(point);

    if(result)
        return true;

    if(m_strokeBrush && m_strokeThickness > 0)
        result = m_geometry->HitTestStroke(point, GetPen());

    return result;
}

void ShapeElement::GeometryCacheEnabled( bool enabled )
{
    m_geometryCacheEnabled = enabled;

    if(m_geometry)
    {
        m_geometry->GeometryCacheEnabled(enabled);
    }
}

auto ShapeElement::GeometryCacheEnabled() -> bool 
{
    return m_geometryCacheEnabled;
}
