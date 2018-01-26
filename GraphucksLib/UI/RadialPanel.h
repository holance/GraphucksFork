#pragma once
#include "PanelElement.h"

namespace Graphucks
{
    class RadialPanel;
    typedef std::shared_ptr<RadialPanel> RadialPanelPtr;

    class RadialPanel : public PanelElement
    {
    public:
        RadialPanel(void);
        virtual ~RadialPanel(void);

        float RadiusDivisor();
        void RadiusDivisor(float radiusDivisor);
    protected:
        virtual auto MeasureOverride(const Graphucks::Size& availableSize)  -> Graphucks::Size override;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize)      -> Graphucks::Size override;
    private:
        float m_radiusDivisor;
    };

}