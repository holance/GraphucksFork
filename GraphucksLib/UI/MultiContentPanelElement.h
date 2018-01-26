#pragma once
#include "VisualElement.h"
#include "PanelElement.h"

namespace Graphucks
{
    class MultiContentPanelElement;
    typedef std::shared_ptr<MultiContentPanelElement> MultiContentPanelElementPtr;

    class MultiContentPanelElement : public PanelElement
    {
    public:
        MultiContentPanelElement(void);
        virtual ~MultiContentPanelElement(void);
    protected:
        virtual auto MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size override;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize)      -> Graphucks::Size override;

    };
}
