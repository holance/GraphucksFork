#include "pch.h"
#include "PanelElement.h"

using namespace Graphucks;

PanelElement::PanelElement(void)
{
}

PanelElement::~PanelElement(void)
{
}

auto PanelElement::GetChildrenCount() -> int
{
    return GetVisualChildCount();
}

auto PanelElement::GetChild(int index) -> const VisualElementPtr
{
    return GetVisualChild(index);
}

void PanelElement::RemoveChild(const VisualElementPtr& child)
{
    VisualElement::RemoveVisualChild(child);
    InvalidateMeasure();
}

void Graphucks::PanelElement::AddChild(const VisualElementPtr& child)
{
    VisualElement::AddVisualChild(child);
    InvalidateMeasure();
}
