#pragma once
#include "VisualElement.h"
#include "PanelElement.h"

namespace Graphucks
{
    enum class Orientation
    {
        Horizontal,
        Vertical
    };

    class StackPanel;
    typedef std::shared_ptr<StackPanel> StackPanelPtr;

    class StackPanel : public PanelElement
    {
    public:
        StackPanel(void);
        virtual ~StackPanel(void);
    protected:
        virtual auto MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size override;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize)     -> Graphucks::Size override;
    public:
        Graphucks::Orientation GetOrientation() const;
        void SetOrientation(Graphucks::Orientation val);
    private:
        Orientation m_orientation;
    };
}
