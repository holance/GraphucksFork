#include "pch.h"
#include "RadialPanel.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Graphucks;
using namespace std;

RadialPanel::RadialPanel(void) : m_radiusDivisor(3)
{
}


RadialPanel::~RadialPanel(void)
{
}

auto RadialPanel::MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size 
{
    for (int i = 0 ; i < GetVisualChildCount(); i++)
    {
        auto elem = GetVisualChild(i);
        elem->Measure(availableSize);//Size(INFINITY, INFINITY));
    }

    return availableSize;
}

auto RadialPanel::ArrangeOverride(const Graphucks::Size& finalSize) -> Graphucks::Size 
{
    auto childCount = GetVisualChildCount();

    if(childCount== 0)
        return finalSize;

    float angle = 0;

    float incrementalAngularSpace = (360.0f /(float) childCount) *  (float)(M_PI/180.0f);

     
    float radiusX = finalSize.width / m_radiusDivisor;

    float radiusY = finalSize.height / m_radiusDivisor;

    for (int i = 0 ; i < GetVisualChildCount(); i++)
    {
        auto element = GetVisualChild(i);
        auto desiredSize = element->DesiredSize();
        auto childPoint = Graphucks::Point(cos(angle) * radiusX, -sin(angle) * radiusY);
        auto actualChildPoint = Graphucks::Point(finalSize.width /2 + childPoint.x - desiredSize.width /2, finalSize.height /2 + childPoint.y - desiredSize.height / 2);

        element->Arrange(Rectangle(actualChildPoint.x, actualChildPoint.y, desiredSize.width + actualChildPoint.x, desiredSize.height + actualChildPoint.y));

        angle += incrementalAngularSpace;
    }

    return finalSize;
}

float Graphucks::RadialPanel::RadiusDivisor()
{
    return m_radiusDivisor;
}

void Graphucks::RadialPanel::RadiusDivisor( float radiusDivisor )
{
    if(radiusDivisor == 0 || radiusDivisor == m_radiusDivisor)
        return;

    m_radiusDivisor = radiusDivisor;

    InvalidateMeasure();
}
