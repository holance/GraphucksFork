#pragma once
#include "..\Rendering\RenderContext.h"
#include "..\UI\VisualElement.h"

namespace Graphucks
{
    class SceneRoot
    {
    public:
        SceneRoot(const RenderContextPtr& renderContext);
        HRESULT Render();
        void RootElement(const VisualElementPtr& elementRoot);
        auto RootElement() const  -> const std::shared_ptr<VisualElement>{return m_elementRoot;}
        auto GetRenderContext()   -> const std::shared_ptr<RenderContext>&;
        void SetRenderContext(const std::shared_ptr<RenderContext>& context);
        void ForceDirty();
        auto IsSceneDirty() const -> bool;
    public:
        void SendOnPointerDown(const Graphucks::Point& point);
        void HitTestTree(const Graphucks::Point& point, VisualElementPtr& visualElementResult);
    private:
        bool m_forceDirty;
        RenderContextPtr m_renderContext;
        VisualElementPtr m_elementRoot;
        void *           m_lastRenderTarget;
    };
}
