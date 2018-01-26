#pragma once
#include "ShapeElement.h"

namespace Graphucks
{
    class EllipseElement;
    typedef std::shared_ptr<EllipseElement> EllipseElementPtr;

    class EllipseElement :
            public ShapeElement
    {
    public:
        EllipseElement(void);
        virtual ~EllipseElement(void);
    protected:
        virtual void RenderOverride(const IRenderContextPtr& renderContext) override;
        virtual auto MeasureOverride(const Graphucks::Size& availableSize) -> Graphucks::Size override ;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize) -> Graphucks::Size override ;
    private:
        void CreateGeometry(const Graphucks::Size& size);
    private:
        EllipsePtr      m_ellipseGeometry;
        Graphucks::Size m_lastSize;
    };

}

