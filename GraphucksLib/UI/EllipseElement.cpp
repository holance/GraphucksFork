#include "pch.h"
#include "EllipseElement.h"
#include "Ellipse.h"

using namespace Graphucks;
using namespace D2D1;
using namespace std;

EllipseElement::EllipseElement(void)
{
   
}

EllipseElement::~EllipseElement(void)
{
}

void EllipseElement::RenderOverride(const IRenderContextPtr& renderContext)
{
    auto renderSize = GetRenderSize();

    if(!m_ellipseGeometry)
    {
	    CreateGeometry(m_lastSize);
    }

    auto fillBrush = FillBrush();
   
    renderContext->DrawGeometry(m_ellipseGeometry, fillBrush, GetPen());

    return;
}

Graphucks::Size EllipseElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    if(GetStretch() == Stretch::UniformToFill)
    {
        float width = availableSize.width;
        float height = availableSize.height;

        if(width == Graphucks::GRAPHUCKS_INFINITY && height == Graphucks::GRAPHUCKS_INFINITY)
        {
            return Graphucks::Size(StrokeWidth(), StrokeWidth());
        }
        else if(width == Graphucks::GRAPHUCKS_INFINITY || height == Graphucks::GRAPHUCKS_INFINITY)
        {
            width = min(width, height);
        }
        else
        {
            width = max(width, height);
        }

        return Graphucks::Size(width, width);
    }

    return Graphucks::Size(StrokeWidth(), StrokeWidth());
}

Graphucks::Size EllipseElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
    Graphucks::Size size;
	
    size = finalSize;
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

    if(size.width  != m_lastSize.width && 
       size.height != m_lastSize.height)
    {
        m_ellipseGeometry = nullptr;
        m_lastSize = size;
    }

	return finalSize;
}

void EllipseElement::CreateGeometry(const Graphucks::Size& size)
{
    float strokeThickness = 0;
    
    if(StrokeBrush())
        strokeThickness = StrokeWidth() / 2 ;
    
    float centerX = (size.width  / 2);
    float centerY = (size.height / 2);

    m_ellipseGeometry = make_shared<Ellipse>(Point(centerX, centerY), 
                                            (size.width/2) - strokeThickness, 
                                            (size.height/2) - strokeThickness);
    m_lastSize = size;
    SetGeometry(m_ellipseGeometry);
}
