#include "pch.h"
#include "SceneRoot.h"

using namespace Graphucks;

Graphucks::SceneRoot::SceneRoot(const RenderContextPtr& renderContext) : m_renderContext(renderContext), m_forceDirty(false)
{
}

auto SceneRoot::Render() -> HRESULT
{
    if(!m_elementRoot)
        return E_UNEXPECTED;

    Graphucks::Rectangle rect(0,0,0,0);

    //m_elementRoot->Measure(Graphucks::Size(Graphucks::INFINITY, Graphucks::INFINITY));

    auto size = m_renderContext->GetRenderBitmap()->GetSize();

    rect.right = size.width;
    rect.bottom = size.height;

    m_elementRoot->Measure(size);
    m_elementRoot->Arrange(rect);
    m_elementRoot->UpdateImpl(0);

   // m_elementRoot->RenderingImpl(); /* to drive frame based animations */

    bool needsRender = m_forceDirty; 

    void * targetPtr =  m_renderContext->GetCurrentRenderTarget();
   
    if(targetPtr != m_lastRenderTarget)
        needsRender = true;

    needsRender |= m_elementRoot->m_renderInvalid;

    if(needsRender)
    {
        m_renderContext->Clear(Color(float(1),float(1),float(1),float(0)));
        m_elementRoot->RenderImpl(m_renderContext);
    }

    m_lastRenderTarget = targetPtr;
    m_forceDirty = false;
    return S_OK;
}

void SceneRoot::RootElement(const VisualElementPtr& elementRoot)
{
    if(m_elementRoot)
    {
        m_elementRoot->InvokeUnloaded();
        m_elementRoot->m_isRoot = false;
    }

    m_elementRoot = elementRoot;

    if(m_elementRoot)
    {
        m_elementRoot->InvokeLoaded();
        m_elementRoot->m_isRoot = true;
    }
}

const std::shared_ptr<RenderContext>& Graphucks::SceneRoot::GetRenderContext()
{
    return m_renderContext;
}

bool Graphucks::SceneRoot::IsSceneDirty() const
{
    if(m_forceDirty)
        return true;

    if (!m_elementRoot)
    {
        return false;
    }

    m_elementRoot->RenderingImpl(); /* to drive frame based animations */

    return m_elementRoot->m_renderInvalid || 
           m_elementRoot->m_arrangeDirty  || 
           m_elementRoot->m_measureDirty  || 
           m_elementRoot->m_transformDirty;
}

void Graphucks::SceneRoot::SendOnPointerDown( const Graphucks::Point& point )
{
    VisualElementPtr elem;
    if(!m_elementRoot)
        return;

    m_elementRoot->HitTestTree(point, elem);

    if(elem)
    {
        auto xform = elem->m_realSceneTransform;
        xform.Invert();
        auto translatedPoint = xform.TransformPoint(point);
        elem->m_pointerDownEvent.Invoke(elem, Point(translatedPoint.x, translatedPoint.y));
    }
}

void Graphucks::SceneRoot::ForceDirty()
{
    m_forceDirty = true;

    if(m_elementRoot)
    {
        m_elementRoot->InvalidateMeasure();
        m_elementRoot->InvalidateArrange();
    }
}

void SceneRoot::SetRenderContext( const std::shared_ptr<RenderContext>& context )
{
    m_renderContext = context;
}

void SceneRoot::HitTestTree(const Graphucks::Point& point, VisualElementPtr& visualElementResult)
{
    if(!m_elementRoot)
        return;

    m_elementRoot->HitTestTree(point, visualElementResult);
}
