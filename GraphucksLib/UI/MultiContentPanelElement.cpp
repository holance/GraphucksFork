#include "pch.h"
#include "MultiContentPanelElement.h"
using namespace Graphucks;

MultiContentPanelElement::MultiContentPanelElement(void) 
{
}


MultiContentPanelElement::~MultiContentPanelElement(void)
{
}

auto MultiContentPanelElement::MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size 
{
    auto available = availableSize;
    auto constraint = available;

    auto size = Graphucks::Size(0,0);

    for (int i = 0; i < GetVisualChildCount(); i++)
    {
        auto child = GetVisualChild(i);
        child->Measure(available);
        auto desiredSize = child->DesiredSize();
       
        size.width = max(desiredSize.width, size.width);
        size.height = max(desiredSize.height, size.height);
    }

    return size;
}

auto MultiContentPanelElement::ArrangeOverride(const Graphucks::Size& finalSize) -> Graphucks::Size 
{
    auto finalRect = Graphucks::Rectangle(0, 0, finalSize.width, finalSize.height);

    for (int i = 0; i < GetVisualChildCount(); i++)
    {
        auto element = GetVisualChild(i);
        auto desiredSize = element->DesiredSize();

        element->Arrange(finalRect);
    }

    return finalSize;
}
