#pragma once
#include "VisualElement.h"

namespace Graphucks
{
    class ContentControlElement : public VisualElement
    {
    public:
        ContentControlElement(void);
        virtual ~ContentControlElement(void);
    public:
        auto GetContent() -> const VisualElementPtr&;
        void SetContent(const VisualElementPtr& content);
    protected:
        virtual void UpdateOverride(LONGLONG time);
		virtual void RenderOverride(const IRenderContextPtr& renderContext) override;
    protected:
        virtual Graphucks::Size MeasureOverride(const Graphucks::Size& availableSize) override;
        virtual Graphucks::Size ArrangeOverride(const Graphucks::Size& finalSize)     override;
    };
}
