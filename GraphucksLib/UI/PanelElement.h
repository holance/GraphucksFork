#pragma once
#include "VisualElement.h"

namespace Graphucks
{
    class PanelElement : public VisualElement
    {
    public:
        PanelElement(void);
        virtual ~PanelElement(void);
    public:
        auto GetChildrenCount() -> int ;
        auto GetChild(int index) -> const VisualElementPtr;
        void RemoveChild(const VisualElementPtr& child);
        void AddChild(const VisualElementPtr& child);
    protected:
         virtual void RenderOverride(const IRenderContextPtr& ) final
         {}
    };
}


