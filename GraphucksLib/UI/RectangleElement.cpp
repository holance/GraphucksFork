#include "pch.h"
#include "RectangleElement.h"
#include "RoundedRectangle.h"
#include "TextLayout.h"

using namespace Graphucks;
using namespace D2D1;
using namespace std;

RectangleElement::RectangleElement(void) : m_radiusX(0), m_radiusY(0)
{
    
}

RectangleElement::~RectangleElement(void)
{
}

void RectangleElement::RenderOverride(const IRenderContextPtr& renderContext)
{
    if(!m_geometry)
    {
	    CreateGeometry(m_lastSize);
    }

    auto fillBrush = FillBrush();
   
    renderContext->DrawGeometry(m_geometry, fillBrush, GetPen());
}

Graphucks::Size RectangleElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    if (GetStretch() == Stretch::UniformToFill) 
    { 
        float width = availableSize.width;
        float height = availableSize.height; 

        if(width == GRAPHUCKS_INFINITY && height == GRAPHUCKS_INFINITY)
        {
            return Size(StrokeWidth(), StrokeWidth()); 
        }
        else if (width == GRAPHUCKS_INFINITY || height == GRAPHUCKS_INFINITY) 
        { 
            width = min(width, height);
        } 
        else
        {
            width = max(width, height);
        } 

        return Size(width, width); 
    } 

    return Size(StrokeWidth(), StrokeWidth()); 
}

Graphucks::Size RectangleElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
	Size size;

    float penThickness = StrokeWidth(); 
     
    size.width  = max(0, finalSize.width  - penThickness);
    size.height = max(0, finalSize.height - penThickness);

    switch (GetStretch()) 
    {
    case Stretch::None: 
           size.width = size.height = 0;
            break; 
    case Stretch::Fill:
            break;
    case Stretch::Uniform: 
            if (size.width > size.height) 
            {
                size.width = size.height;
            }
            else 
            {
                size.height = size.width; 
            } 
            break;
    case Stretch::UniformToFill:
            if (size.width < size.height) 
            {
                size.width = size.height; 
            } 
            else 
            { 
                size.height = size.width;
            }
            break;
    } 

    if(size != m_lastSize)
    {
        m_geometry = nullptr;
        m_lastSize = size;
    }

	return finalSize;
}

float RectangleElement::RadiusX()
{
    return m_radiusX;
}

void RectangleElement::RadiusX(float rx)
{
    m_radiusX = rx;
    InvalidateMeasure();
}

float RectangleElement::RadiusY()
{
    return m_radiusY;
}

void RectangleElement::RadiusY(float ry)
{
    m_radiusY = ry;
    InvalidateMeasure();
}

void RectangleElement::CreateGeometry(const Graphucks::Size& size)
{
    float strokeThickness = 0;
    
    if(StrokeBrush())
        strokeThickness = StrokeWidth() /2 ;
    
    float left = strokeThickness;
    float top =  strokeThickness;

    m_geometry = make_shared<Graphucks::RoundedRectangle>(Rectangle(left, top, size.width, size.height),
                                                          m_radiusX, 
                                                          m_radiusY);
    SetGeometry(m_geometry);
}
