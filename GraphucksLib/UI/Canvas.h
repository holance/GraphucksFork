#pragma once
#include "PanelElement.h"
 
namespace Graphucks
{
    class Canvas;
    typedef std::shared_ptr<Canvas> CanvasPtr;

    class Canvas : public PanelElement
    {
    public:
        Canvas(void);
        virtual ~Canvas(void);
    public:
         static const DependencyPropertyPtr LeftProperty;
         static const DependencyPropertyPtr TopProperty;
         static const DependencyPropertyPtr RightProperty;
         static const DependencyPropertyPtr BottomProperty;
    public:
         static void SetTop(const VisualElementPtr& element, float top);
         static void SetLeft(const VisualElementPtr& element, float top);
         static void SetBottom(const VisualElementPtr& element, float top);
         static void SetRight(const VisualElementPtr& element, float top);

         static float GetTop(const VisualElementPtr& element);
         static float GetLeft(const VisualElementPtr& element);
         static float GetBottom(const VisualElementPtr& element);
         static float GetRight(const VisualElementPtr& element);
    private:
        static void LocationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
    protected:
        virtual auto MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size override;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize)     -> Graphucks::Size override;
    };
}
