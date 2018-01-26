#include "pch.h"
#include "StackPanel.h"
using namespace Graphucks;

StackPanel::StackPanel(void) : m_orientation(Orientation::Vertical)
{
}


StackPanel::~StackPanel(void)
{
}

auto StackPanel::MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size 
{
    auto available = availableSize;
    auto constraint = available;

    auto size = Graphucks::Size(0,0);

    if(m_orientation == Orientation::Horizontal)
    {
        available.width = Graphucks::GRAPHUCKS_INFINITY;
    }
    else
    {
        available.height = Graphucks::GRAPHUCKS_INFINITY;
    }

    for (int i = 0; i < GetVisualChildCount(); i++)
    {
        auto child = GetVisualChild(i);
        child->Measure(available);

        if (m_orientation == Orientation::Horizontal)
        {
            auto desiredSize = child->DesiredSize();
            size.width += desiredSize.width;
            size.height = max(size.height, desiredSize.height);
        }
        else
        {
            auto desiredSize = child->DesiredSize();
            size.height += desiredSize.height;
            size.width = max(size.width, desiredSize.width);
        }
    }

    return size;
}

auto StackPanel::ArrangeOverride(const Graphucks::Size& finalSize) -> Graphucks::Size 
{
    auto finalRect = Graphucks::Rectangle(0, 0, finalSize.width, finalSize.height);
    float lastSize = 0;

    for (int i = 0; i < GetVisualChildCount(); i++)
    {
        auto element = GetVisualChild(i);

        if (m_orientation == Orientation::Horizontal)
        {
            finalRect.left += lastSize;
            auto desiredSize = element->DesiredSize();
            lastSize = desiredSize.width;
            finalRect.right = finalRect.left + lastSize;
            finalRect.bottom = finalRect.top + min(finalSize.height, desiredSize.height);
        }
        else
        {
            finalRect.top += lastSize;
            auto desiredSize = element->DesiredSize();
            lastSize = desiredSize.height;
            finalRect.bottom = finalRect.top + lastSize;
            finalRect.right = finalRect.left + min(finalSize.width, desiredSize.width);
        }

        element->Arrange(finalRect);
    }

    return finalSize;
}

Orientation StackPanel::GetOrientation() const
{
    return m_orientation;
}

void StackPanel::SetOrientation(Graphucks::Orientation val)
{
    m_orientation = val;
}
