#include "pch.h"
#include "ContentControlElement.h"

using namespace Graphucks;

ContentControlElement::ContentControlElement(void)
{
}


ContentControlElement::~ContentControlElement(void)
{
}

void Graphucks::ContentControlElement::UpdateOverride(LONGLONG /* time */)
{

}

Graphucks::Size ContentControlElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    int childCount = GetVisualChildCount();

    if(childCount == 0)
        return Graphucks::Size(0, 0);

    auto visualChild = GetVisualChild(0);

    visualChild->Measure(availableSize);

    return visualChild->DesiredSize();
}

Graphucks::Size ContentControlElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
    int childCount = GetVisualChildCount();

    if(childCount == 0)
        return finalSize;
 
    auto visualChild = GetVisualChild(0);

    visualChild->Arrange(Graphucks::Rectangle(0,0, finalSize.width, finalSize.height));

    return finalSize;
}

void ContentControlElement::RenderOverride(const IRenderContextPtr& /* renderContext */)
{
}

void ContentControlElement::SetContent(const VisualElementPtr& content)
{
    auto childCount = GetVisualChildCount();

    if(childCount > 0)
    {
        RemoveAllChildren();
    }

    AddVisualChild(content);
}
