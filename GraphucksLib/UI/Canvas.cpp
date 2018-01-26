#include "pch.h"
#include "Canvas.h"
#include "..\Core\GraphicsTypes.h"
using namespace Graphucks;
using namespace std;

Canvas::Canvas(void)
{
}


Canvas::~Canvas(void)
{
}

const DependencyPropertyPtr Canvas::TopProperty = DependencyProperty::Register<Canvas/*ownerType*/>(L"Top", 
                                                                                              FrameworkPropertyMetadata::Create<float/*dp type*/>(
                                                                                                  Infinity(), 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsMeasure,
                                                                                                  &LocationChanged)
                                                                                               );


const DependencyPropertyPtr Canvas::RightProperty = DependencyProperty::Register<Canvas/*ownerType*/>(L"Right", 
                                                                                              FrameworkPropertyMetadata::Create<float/*dp type*/>(
                                                                                                  Infinity(), 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsMeasure,
                                                                                                  &LocationChanged)
                                                                                               );

const DependencyPropertyPtr Canvas::BottomProperty = DependencyProperty::Register<Canvas/*ownerType*/>(L"Bottom", 
                                                                                              FrameworkPropertyMetadata::Create<float/*dp type*/>(
                                                                                                  Infinity(), 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsMeasure,
                                                                                                  &LocationChanged)
                                                                                               );

const DependencyPropertyPtr Canvas::LeftProperty = DependencyProperty::Register<Canvas/*ownerType*/>(L"Left", 
                                                                                              FrameworkPropertyMetadata::Create<float/*dp type*/>(
                                                                                                  Infinity(), 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsMeasure,
                                                                                                  &LocationChanged)
                                                                                               );

auto Canvas::MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size 
{
    auto childConstraint = Size(GRAPHUCKS_INFINITY, GRAPHUCKS_INFINITY); 

    for (int i = 0; i < GetVisualChildCount(); i++)
    {
        auto child = GetVisualChild(i);
        child->Measure(childConstraint); 
    }

    return Size();
}

auto Canvas::ArrangeOverride(const Graphucks::Size& finalSize) -> Graphucks::Size 
{
    for (int i = 0; i < GetVisualChildCount(); i++)
    {
        auto child = GetVisualChild(i);

        float xPosition = 0; 
        float yPosition = 0; 
 
        float left = GetLeft(child);

        auto childDesiredSize = child->DesiredSize();

        if(left != GRAPHUCKS_INFINITY) 
            xPosition = left;
        else
        {
            float right = GetRight(child);
 
            if(right != GRAPHUCKS_INFINITY)
                xPosition = finalSize.width - childDesiredSize.width - right;
        } 

        float top = GetTop(child);

        if(top != GRAPHUCKS_INFINITY)
            yPosition = top;
        else 
        {
            float bottom = GetBottom(child); 

            if(bottom != GRAPHUCKS_INFINITY)
                yPosition = finalSize.height - childDesiredSize.height - bottom; 
        } 
 
        child->Arrange(Rectangle(xPosition, 
                                 yPosition, 
                                 childDesiredSize.width + xPosition, 
                                 childDesiredSize.height + yPosition));
    } 

    return finalSize;
}

void Canvas::LocationChanged( DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs )
{
     
}

void Canvas::SetTop(const VisualElementPtr& element, float top )
{
    element->SetValue(TopProperty, top);
}

void Canvas::SetLeft(const VisualElementPtr& element, float top )
{
    element->SetValue(LeftProperty, top);
}

void Canvas::SetBottom(const  VisualElementPtr& element, float top )
{
    element->SetValue(BottomProperty, top);
}

void Canvas::SetRight(const VisualElementPtr& element, float top )
{
    element->SetValue(RightProperty, top);
}

float Canvas::GetTop(const VisualElementPtr& element )
{
    return element->GetValue<float>(TopProperty);
}

float Canvas::GetLeft(const VisualElementPtr& element )
{
    return element->GetValue<float>(LeftProperty);
}

float Canvas::GetBottom(const VisualElementPtr& element )
{
    return element->GetValue<float>(BottomProperty);
}

float Canvas::GetRight(const VisualElementPtr& element )
{
    return element->GetValue<float>(RightProperty);
}
