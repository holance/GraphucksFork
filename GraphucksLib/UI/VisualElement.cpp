#include "pch.h"
#include "VisualElement.h"
#include <string>
#include <sstream>
#include "RenderContext.h"
#include "ClipCommand.h"
#include "CompositeTransform.h"

using namespace Graphucks;
using namespace std;
using namespace D2D1;


DP_PROPERTY_IMPL(RenderTransform, VisualElement, TransformPtr,     nullptr, (int)FrameworkPropertyMetadataOptions::AffectsRender)
DP_PROPERTY_IMPL(ElementEffect,   VisualElement, ElementEffectPtr, nullptr, (int)FrameworkPropertyMetadataOptions::AffectsRender)

VisualElement::VisualElement() : m_transform                (Matrix3x2F::Identity()), 
                                 m_realSceneTransform       (Matrix3x2F::Identity()),
                                 m_clipRect                 (Rectangle(0,0,0,0)),
                                 m_desiredSize              (Size(0,0)),
                                 m_size                     (Size(GRAPHUCKS_INFINITY, GRAPHUCKS_INFINITY)),
                                 m_minSize                  (0, 0),
                                 m_maxSize                  (Graphucks::Size(GRAPHUCKS_INFINITY, GRAPHUCKS_INFINITY)),
                                 m_unclippedDesiredSize     (-1, -1),
                                 m_clipEnabled              (false),
                                 m_horizontalAlignment      (HorizontalAlignment::Stretch),
                                 m_verticalAlignment        (VerticalAlignment::Stretch),
                                 m_renderTransformOrigin    (Point(0.5f, 0.5f)),
                                 m_measureDirty             (true),
                                 m_arrangeDirty             (true),
                                 m_transformDirty           (true),
                                 m_bitmapCacheEnabled       (false),
                                 m_bitmapCachedInvalid      (true),
                                 m_bitmapCacheRenderAtScale (1.0f),
                                 m_renderInvalid            (true),
                                 m_clipToBounds             (false),
                                 m_parentElement            (nullptr),
                                 m_compositeMode            (EffectCompositeMode::SourceOver),
                                 m_loaded                   (false),
                                 m_isRoot                   (false)
{
}

void VisualElement::AddVisualChild(const VisualElementPtr& element)
{
    if(element->GetParent() != nullptr)
        throw invalid_argument("VisualElement is already connected to a parent");

    if(!element)
        throw invalid_argument("VisualElement is null");

    element->SetParent(this);

    m_children.push_back(element);

    element->InvalidateVisual();

    if(IsRooted())
        element->InvokeLoaded();
}

VisualElement * VisualElement::GetParent()
{
    return m_parentElement;
}

HRESULT VisualElement::SetParent(VisualElement* parent)
{
    auto oldParent = m_parentElement;
    m_parentElement = parent;

    InvalidateVisual();
    OnParentChanged(oldParent, parent);

    return S_OK;
}

void VisualElement::RemoveVisualChild(const VisualElementPtr& element)
{
    auto findIter = find(begin(m_children), end(m_children), element);

    if(*findIter == nullptr)
        return;
   
    m_children.erase(findIter);
	
    if(element->m_loaded)
		element->InvokeUnloaded();

    element->SetParent(nullptr);
}

VisualElementPtr VisualElement::GetVisualChild(int index)
{
    auto size = m_children.size();

    if((size_t)index >= size)
        return nullptr;

    return m_children[index];
}

int VisualElement::GetVisualChildCount()
{
    return (int)m_children.size();
}

void VisualElement::RenderImpl(const IRenderContextPtr& renderContext)
{
     /* dirty */
    auto contextImpl = static_cast<RenderContext*>(renderContext.get());

    EnsureBitmapCache(m_renderSize, contextImpl->MaxiumImageSize());
    
    if(m_bitmapCache)
    {
        CalculateTransform();
        if(m_bitmapCachedInvalid || m_renderInvalid)
        {
            auto oldTransform = m_transform;
           
            contextImpl->PushRenderTarget(m_bitmapCache);
            
            {
                auto cacheDimensions = m_bitmapCache->GetSize();

                m_transform = Matrix3x2F::Scale(cacheDimensions.width / m_renderSize.width, 
                                                cacheDimensions.height/ m_renderSize.height);

                renderContext->SetTransform(m_transform);

                
                ClipCommand clipCommand(m_clipEnabled, &m_clipRect, renderContext.get());
            
                RenderOverride(renderContext);
                m_renderInvalid = false;
                m_bitmapCachedInvalid = false;

                for(VisualElementPtr& element : m_children)
                {
                    element->RenderImpl(renderContext);
                }
            }

            contextImpl->PopRenderTarget();
            m_transform = oldTransform;
        }
           
        
        contextImpl->SetTransform(m_transform);
        ClipCommand clipCommand(m_clipEnabled, &m_clipRect, renderContext.get());

        auto bitmapSize = m_bitmapCache->GetSize();

        auto elementEffect = ElementEffect();

        if(elementEffect)
            contextImpl->DrawEffect(m_bitmapCache, elementEffect, m_compositeMode, m_renderSize);
        else
            contextImpl->DrawBitmap(m_bitmapCache, m_compositeMode, m_renderSize);
    }
    else
    {
        CalculateTransform();
        m_realSceneTransform = m_transform;
        contextImpl->SetTransform(m_transform);

        ClipCommand clipCommand(m_clipEnabled, &m_clipRect, renderContext.get());

        RenderOverride(renderContext);
        m_renderInvalid = false;

        for(VisualElementPtr& element : m_children)
        {
             element->RenderImpl(renderContext);
        }
    }    
}

void VisualElement::RenderingImpl()
{
    Rendering();
    for(auto& element : m_children)
    {
        element->RenderingImpl();
    }
}

void VisualElement::Rendering()
{
         
}

void VisualElement::UpdateImpl(LONGLONG time)
{
    auto renderTransform = RenderTransform();
    if(renderTransform && !m_transformDirty)
    {
        if(!memcmp(&m_lastRenderTransform, 
                   &renderTransform->GetTransformMatrix(), 
                   sizeof(Matrix3x2F)))
        {
            m_transformDirty = true;
        }
    }

    for(auto& element : m_children)
    {
        element->UpdateImpl(time);
    }

    if(m_transformDirty && !m_renderInvalid)
    {
        auto parent = m_parentElement;
        if(parent)
            parent->InvalidateVisual();
    }
}


void VisualElement::ClipRect(const Graphucks::Rectangle& clipRect)
{
    m_clipRect = clipRect;
}

void VisualElement::ClipEnable(bool enabled)
{
    m_clipEnabled = enabled;
}

const Graphucks::Size& VisualElement::DesiredSize()
{
    return m_desiredSize;
}

const Graphucks::Size& VisualElement::GetSize() const
{
    return m_size;
}

void VisualElement::SetSize(Graphucks::Size& size)
{
    m_size = size;
    InvalidateMeasure();
}

void VisualElement::SetDesiredSize(Graphucks::Size& size)
{
    m_desiredSize = size;
}

Graphucks::Size VisualElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    auto size = GetSize();//SizeF(0, 0);

    if(size.width == Graphucks::GRAPHUCKS_INFINITY)
    {
        size.width = availableSize.width;
    }

    if(size.height == Graphucks::GRAPHUCKS_INFINITY)
    {
        size.height = availableSize.height;
    }
    
    for(VisualElementPtr& element : m_children)
    {
        element->Measure(availableSize);
    }

    return availableSize;
}

Graphucks::Size VisualElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
    for (int i = 0 ; i < GetVisualChildCount() ; i++)
    {
       GetVisualChild(i)->Arrange(Rectangle(0,0, finalSize.width, finalSize.height));
    }
    return finalSize;
}

void VisualElement::CalculateMinMax(Graphucks::Size& minSize, Graphucks::Size& maxSize)
{
    maxSize.height = m_maxSize.height;
    minSize.height = m_minSize.height;

    auto dimensionLength = m_size.height;
 
    float height = (Graphucks::GRAPHUCKS_INFINITY == dimensionLength ? Graphucks::GRAPHUCKS_INFINITY : dimensionLength);

    maxSize.height = max<float>(min<float>(height, maxSize.height), minSize.height); 

    height = (Graphucks::GRAPHUCKS_INFINITY == dimensionLength ? 0 : dimensionLength);

    minSize.height = max<float>(min<float>(maxSize.height, height), minSize.height);
 
    maxSize.width = m_maxSize.width;
    minSize.width = m_minSize.width;

    dimensionLength = m_size.width; 

    float width = (Graphucks::GRAPHUCKS_INFINITY == dimensionLength ? Graphucks::GRAPHUCKS_INFINITY : dimensionLength); 

    maxSize.width = max<float>(min<float>(width, maxSize.width), minSize.width);

    width = (Graphucks::GRAPHUCKS_INFINITY == dimensionLength ? 0 : dimensionLength);

    minSize.width = max<float>(min<float>(maxSize.width, width), minSize.width); 
}

void VisualElement::Measure(const Graphucks::Size& availableSize)
{
    if(!m_measureDirty)
        return;

    auto margin = Margin(); 

    float marginWidth = margin.Left + margin.Right;
    float marginHeight = margin.Top + margin.Bottom; 

    auto frameworkAvailableSize = Graphucks::Size(max(availableSize.width  - marginWidth,  0),
                                                  max(availableSize.height - marginHeight, 0));

    Size maxSize, minSize;
    CalculateMinMax(minSize, maxSize);

    frameworkAvailableSize.width  = max(minSize.width,  min(frameworkAvailableSize.width, maxSize.width));
    frameworkAvailableSize.height = max(minSize.height, min(frameworkAvailableSize.height, maxSize.height));

    Graphucks::Size desiredSize = MeasureOverride(frameworkAvailableSize);

    desiredSize = Graphucks::Size(max(desiredSize.width, minSize.width),
                                  max(desiredSize.height, minSize.height));

    Size unclippedDesiredSize = desiredSize;

    bool clipped = false;

    if(desiredSize.width > maxSize.width) 
    {
        desiredSize.width =  maxSize.width; 
        clipped = true;
    }

    if(desiredSize.height > maxSize.height) 
    {
        desiredSize.height = maxSize.height; 
        clipped = true; 
    }
 
    float clippedDesiredWidth  = desiredSize.width  + marginWidth;
    float clippedDesiredHeight = desiredSize.height + marginHeight; 

    if (clippedDesiredWidth > availableSize.width) 
    {
        clippedDesiredWidth = availableSize.width;
        clipped = true;
    } 

    if (clippedDesiredHeight > availableSize.width) 
    { 
        clippedDesiredHeight = availableSize.width;
        clipped = true; 
    }

     if (clipped                  ||
         clippedDesiredWidth  < 0 ||
         clippedDesiredHeight < 0  )
     { 
        m_unclippedDesiredSize = unclippedDesiredSize;
     }
     else
     {
         m_unclippedDesiredSize.width  = -1;
         m_unclippedDesiredSize.height = -1;
     }

    m_desiredSize = Size(clippedDesiredWidth, clippedDesiredHeight);

    m_measureDirty = false;
}

void VisualElement::Arrange(const Graphucks::Rectangle& finalRect)
{
    auto ancestor = m_parentElement;

    bool ancestorDirty = false;

    while(ancestor)
    {
        if(ancestor->m_arrangeDirty)
        {
            ancestorDirty = true;
            break;
        }

        ancestor = ancestor->m_parentElement;
    }
    
    if(!m_arrangeDirty && !ancestorDirty)
        return;

    m_clipEnabled = false;

    auto arrangeSize = finalRect.Size(); 
 
    auto margin = Margin();

    Size marginSize;

    marginSize.width  = margin.Left + margin.Right;
    marginSize.height = margin.Top  + margin.Bottom;

    arrangeSize.width  =  max(0, arrangeSize.width  - marginSize.width);
    arrangeSize.height =  max(0, arrangeSize.height - marginSize.height); 

    auto unclippedDesiredSize = m_unclippedDesiredSize; 

    if(unclippedDesiredSize.width  == -1 || 
       unclippedDesiredSize.height == -1)
    {
        if(m_desiredSize.width == GRAPHUCKS_INFINITY || m_desiredSize.height == GRAPHUCKS_INFINITY)
        {
            unclippedDesiredSize.width = arrangeSize.width - marginSize.width;
            unclippedDesiredSize.height = arrangeSize.height - marginSize.height;
        }
        else
        {
            unclippedDesiredSize.width  =  m_desiredSize.width  -  marginSize.width,
            unclippedDesiredSize.height =  m_desiredSize.height -  marginSize.height;
        }
    }
   
    if (arrangeSize.width < unclippedDesiredSize.width)
    {
        m_clipEnabled = true;
        arrangeSize.width = unclippedDesiredSize.width;
    } 

    if (arrangeSize.height < unclippedDesiredSize.height) 
    { 
        m_clipEnabled = true;
        arrangeSize.height = unclippedDesiredSize.height; 
    }

    if (m_horizontalAlignment != HorizontalAlignment::Stretch)
    { 
        arrangeSize.width = unclippedDesiredSize.width; 
    }
 
    if (m_verticalAlignment != VerticalAlignment::Stretch)
    {
        arrangeSize.height = unclippedDesiredSize.height;
    } 
    
    Size minSize, maxSize;
    CalculateMinMax(minSize, maxSize);

    float calcedMaxWidth = max(unclippedDesiredSize.width, maxSize.width);
    if (calcedMaxWidth < arrangeSize.width)
    {
        m_clipEnabled = true;
        arrangeSize.width = calcedMaxWidth;
    }

    float calcedMaxHeight =  max(unclippedDesiredSize.height, maxSize.height); 
    if (calcedMaxHeight < arrangeSize.height) 
    {
        m_clipEnabled = true; 
        arrangeSize.height = calcedMaxHeight;
    }

    Size oldRenderSize     = m_renderSize; 
    Size arrangeResultSize = ArrangeOverride(arrangeSize);

    bool arrangeSizeChanged = arrangeResultSize != m_renderSize;
    if(arrangeSizeChanged)
    {
       RenderSizeChanged(arrangeResultSize);
    }

    m_renderSize = arrangeResultSize;
     
    Size clippedArrangeResultSize = Size(min(arrangeResultSize.width, maxSize.width),
                                         min(arrangeResultSize.height, maxSize.height)); 

    if(!m_clipEnabled)
    {
        m_clipEnabled = clippedArrangeResultSize.width  < arrangeResultSize.width ||
                        clippedArrangeResultSize.height < arrangeResultSize.height;
    }

    auto clientSize = Graphucks::Size(max(0, finalRect.Width() - marginSize.width), 
                                      max(0, finalRect.Height() - marginSize.height));

    if(!m_clipEnabled)
    {
         m_clipEnabled = clientSize.width  < clippedArrangeResultSize.width ||
                         clientSize.height < clippedArrangeResultSize.height;

    }
    
    Graphucks::Point layoutOffset;
    layoutOffset.x = 0;
    layoutOffset.y = 0;

    HorizontalAlignment tempHorizontalAlignment = m_horizontalAlignment;
    VerticalAlignment   tempVerticalAlignment   = m_verticalAlignment;

    if(tempHorizontalAlignment == HorizontalAlignment::Stretch && 
       clippedArrangeResultSize.width > clientSize.width)
    { 
        tempHorizontalAlignment = HorizontalAlignment::Left; 
    }
 
    if(tempVerticalAlignment == VerticalAlignment::Stretch &&
      clippedArrangeResultSize.height > clientSize.height)
    {
        tempVerticalAlignment = VerticalAlignment::Top; 
    }
 
    if (tempHorizontalAlignment == HorizontalAlignment::Center ||  
        tempHorizontalAlignment == HorizontalAlignment::Stretch ) 
    {
        layoutOffset.x = (clientSize.width - clippedArrangeResultSize.width) / 2.0f;
    }
    else if (tempHorizontalAlignment == HorizontalAlignment::Right) 
    {
        layoutOffset.x = clientSize.width - clippedArrangeResultSize.width; 
    } 
    else
    { 
        layoutOffset.x = 0;
    }

    if (tempVerticalAlignment == VerticalAlignment::Center 
    ||  tempVerticalAlignment == VerticalAlignment::Stretch )
    { 
        layoutOffset.y = (clientSize.height - clippedArrangeResultSize.height) / 2.0f; 
    }
    else if (m_verticalAlignment == VerticalAlignment::Bottom) 
    {
        layoutOffset.y = clientSize.height - clippedArrangeResultSize.height;
    }
    else 
    {
        layoutOffset.y = 0; 
    } 

    layoutOffset.x += finalRect.left + margin.Left; 
    layoutOffset.y += finalRect.top + margin.Top; 

    if(m_clipEnabled || m_clipToBounds)
        m_clipRect = Rectangle(0, 0, clientSize.width, clientSize.height);

    if(layoutOffset.x != m_layoutOffset.x ||
       layoutOffset.y != m_layoutOffset.y || arrangeSizeChanged)
    {
        m_transformDirty = true;
    }

    m_arrangeDirty = false;

    m_layoutOffset = layoutOffset;
}

void VisualElement::RenderTransform(const TransformPtr& transform)
{
    SetValue(RenderTransformProperty, transform);
    
}

auto VisualElement::RenderTransform() -> const TransformPtr
{
    return GetValue<Transform>(RenderTransformProperty);
}

const VerticalAlignment VisualElement::GetVerticalAlignment()
{
    return m_verticalAlignment;
}

void VisualElement::SetVerticalAlignment(VerticalAlignment alignment)
{
    m_verticalAlignment = alignment;
}

void VisualElement::SetHorizontalAlignment(HorizontalAlignment alignment)
{
    m_horizontalAlignment = alignment;
}

const HorizontalAlignment VisualElement::GetHorizontalAlignment()
{
    return m_horizontalAlignment;
}

void VisualElement::OnParentChanged(const VisualElement* /*oldParent*/, const VisualElement* /*newParent*/)
{

}

const Graphucks::Size& VisualElement::GetRenderSize()
{
    return m_renderSize;
}

const Thickness& VisualElement::Margin()
{
    return m_margin;
}

void VisualElement::Margin(const Thickness& margin)
{
    m_margin = margin;
    InvalidateMeasure();
}

void VisualElement::RemoveAllChildren()
{
    vector<VisualElementPtr> removeList;

    for(const VisualElementPtr& child : m_children)
    {
        removeList.push_back(child);
    }

    for(const VisualElementPtr& child : removeList)
    {
        RemoveVisualChild(child);
    }
}

const Graphucks::Size& Graphucks::VisualElement::MaximumSize()
{
    return m_maxSize;
}

bool VisualElement::IsRooted()
{
    if(m_isRoot)
        return true;

    auto ancestor = m_parentElement;

    while(ancestor)
    {
        if(ancestor->m_isRoot)
            return true;

        ancestor = ancestor->m_parentElement;
    }

    return false;
}

void VisualElement::RenderOverride( const IRenderContextPtr& /* renderContext */ )
{

}

void VisualElement::InvokeUnloaded()
{
    m_loaded = false;

    Unloaded();
    for(const VisualElementPtr& child : m_children)
    {
        child->InvokeUnloaded();
    }
}

void VisualElement::InvokeLoaded()
{
    m_loaded = true;

    SetInheritanceContext(m_parentElement);
    Loaded();
    for(const VisualElementPtr& child : m_children)
    {
        child->InvokeLoaded();
        
    }
}

void VisualElement::Loaded()
{
    
}

void VisualElement::Unloaded()
{

}

const PointerDownEvent& VisualElement::OnPointerDown()
{
    return m_pointerDownEvent;
}

const D2D1::Matrix3x2F& VisualElement::RealSceneTransform()
{
    return m_realSceneTransform;
}

void VisualElement::Invalidate(int flags)  /* IInvalidatable */
{
    if(flags & INVALIDATE_FLAG_VISUAL)
        InvalidateVisual();

    if(flags & INVALIDATE_FLAG_ARRANGE)
        InvalidateArrange();

    if(flags & INVALIDATE_FLAG_MEASURE)
        InvalidateMeasure();

    if(flags & INVALIDATE_FLAG_TRANSFORM)
    {
        m_transformDirty = true;

        auto ancestor = m_parentElement;

        while(ancestor)
        {
            ancestor->m_transformDirty = true;
            ancestor = ancestor->m_parentElement;
        }
    }
}

bool VisualElement::HitTest(const Point& /*point*/)
{
    return false;
}

void VisualElement::HitTestTree(const Point& point, VisualElementPtr& hitElement)
{
    bool hitResult = false;
    hitElement = nullptr;

    Matrix3x2F inverted = RealSceneTransform();

    inverted.Invert();

    auto transformedPoint = inverted.TransformPoint(point);

    hitResult = HitTest(Point(transformedPoint.x, transformedPoint.y));

    if(hitResult)
    {
        hitElement = shared_from_this();
        return;
    }

    for(auto it = m_children.rbegin(); it != m_children.rend(); ++it) 
    {
        auto& child = *it;

        child->HitTestTree(point, hitElement);
        if(hitElement)
        {
            return;
        }
    }
}

void VisualElement::ClipToBounds(bool clipToBounds)
{
    m_clipToBounds = clipToBounds;
    InvalidateVisual();
}

auto VisualElement::ClipToBounds() const -> bool
{
    return m_clipToBounds;
}

void VisualElement::ElementEffect(const ElementEffectPtr& elementEffect)
{
    SetValue(ElementEffectProperty, elementEffect);
}

ElementEffectPtr VisualElement::ElementEffect()
{
    return GetValue<Graphucks::ElementEffect>(ElementEffectProperty);
}

void VisualElement::EnableBitmapCache(bool enable)
{
    m_bitmapCacheEnabled = enable;

    InvalidateVisual();
}

void VisualElement::SetBitmapCacheRenderScale(float scale)
{
    if(scale < 0)
        scale = 0;

    if(scale == m_bitmapCacheRenderAtScale)
        return;

    m_bitmapCachedInvalid = true;
    m_bitmapCacheRenderAtScale = scale;
}

void VisualElement::CalculateTransform()
{
    m_transform = Matrix3x2F::Translation(m_layoutOffset.x, m_layoutOffset.y);

    Matrix3x2F transform;
    Matrix3x2F realTransform;
    if(GetParent() == nullptr)
    {
        transform     = m_transform * Matrix3x2F::Identity();
        realTransform = m_transform * Matrix3x2F::Identity();
    }
    else
    {
        transform     = m_transform * GetParent()->m_transform;
        realTransform = m_transform * GetParent()->m_realSceneTransform;
    }

    auto renderTransform = RenderTransform();
    if(renderTransform)
    {
        m_transform =  Matrix3x2F::Translation(-(m_renderSize.width  *  m_renderTransformOrigin.x), 
                                               -(m_renderSize.height *  m_renderTransformOrigin.y));

        m_transform = m_transform * (Matrix3x2F)renderTransform->GetTransformMatrix();

        m_transform = m_transform * Matrix3x2F::Translation((m_renderSize.width  * m_renderTransformOrigin.x), 
                                                            (m_renderSize.height * m_renderTransformOrigin.y)) ;
    }
    else
    {
        m_transform = Matrix3x2F::Identity();
    }
     
    m_realSceneTransform = m_transform * realTransform;
    m_transform = m_transform * transform;

    m_transformDirty = false;
}

void VisualElement::InvalidateVisual()
{
    m_renderInvalid = true;
    m_bitmapCachedInvalid = true;
    m_transformDirty = true;
    m_measureDirty = true;
    m_arrangeDirty = true;

    auto ancestor = m_parentElement;
    while(ancestor)
    {
        /* Optimization.  If parent is already invalid, then
         * the rest of the ancestors should be invalid also */
        if(ancestor->m_bitmapCachedInvalid && 
           ancestor->m_renderInvalid && 
           ancestor->m_transformDirty && 
           ancestor->m_measureDirty && 
           ancestor->m_arrangeDirty && 
           ancestor->m_arrangeDirty)
            return;

        ancestor->m_transformDirty = true;
        ancestor->m_bitmapCachedInvalid = true;
        ancestor->m_renderInvalid = true;
        ancestor->m_measureDirty = true;
        ancestor->m_arrangeDirty = true;
        ancestor = ancestor->m_parentElement;
    }
}

void VisualElement::RenderSizeChanged(const Graphucks::Size& /* newSize */)
{
    InvalidateVisual();
}

void VisualElement::RenderTransformOrigin(const Point& point)
{
    m_renderTransformOrigin = point;
}

void VisualElement::EnsureBitmapCache(const Graphucks::Size& size, const int maxBitmapSize)
{
    if(size.IsOneDimensionalOrEmpty() || (!m_bitmapCacheEnabled && !ElementEffect() && m_compositeMode == EffectCompositeMode::SourceOver))
    {
        m_bitmapCache = nullptr;
        return;
    }
    
    if(size.width > maxBitmapSize || size.height > maxBitmapSize)
        return;

    if(m_bitmapCache == nullptr)
    {
        auto scaleSize = size.Scale(m_bitmapCacheRenderAtScale);
        
        if(scaleSize.width > (float)maxBitmapSize)
           scaleSize.width = (float)maxBitmapSize;

        if(scaleSize.height > (float)maxBitmapSize)
           scaleSize.height = (float)maxBitmapSize;

        m_bitmapCache = make_shared<Bitmap>(scaleSize);
        return;
    }

    auto cacheNaturalSize = m_bitmapCache->GetSize();

    if(size.width > cacheNaturalSize.width * m_bitmapCacheRenderAtScale ||
       size.height > cacheNaturalSize.height * m_bitmapCacheRenderAtScale)
    {
        auto scaleSize = size.Scale(m_bitmapCacheRenderAtScale);
        
        if(scaleSize.width > (float)maxBitmapSize)
           scaleSize.width = (float)maxBitmapSize;

        if(scaleSize.height > (float)maxBitmapSize)
           scaleSize.height = (float)maxBitmapSize;

        m_bitmapCache = make_shared<Bitmap>(scaleSize);
        m_bitmapCachedInvalid = true;
    }
}

void VisualElement::MaximumSize(const Graphucks::Size& size)
{
    m_maxSize = size;
}

void VisualElement::InvalidateArrange()
{
    m_arrangeDirty = true;

    auto ancestor = GetParent();
    while(ancestor)
    {
        ancestor->m_arrangeDirty = true;
        ancestor = ancestor->GetParent();
    }
}

void VisualElement::InvalidateMeasure()
{
    m_arrangeDirty = true;
    m_measureDirty = true;
    m_bitmapCachedInvalid = true;

   auto ancestor = GetParent();

    while(ancestor)
    {
        ancestor->m_measureDirty = true;
        ancestor->m_arrangeDirty = true;
        ancestor = ancestor->GetParent();
    }
}

const Graphucks::Size& VisualElement::MinimumSize() const
{
    return m_minSize;
}

void VisualElement::MinimumSize(const Graphucks::Size& size)
{
    m_minSize = size;
}

EffectCompositeMode VisualElement::CompositeMode()
{
    return m_compositeMode;
}

void VisualElement::CompositeMode(EffectCompositeMode mode)
{
    if(m_compositeMode == mode)
        return;

    m_compositeMode = mode;

    InvalidateVisual();
}

void VisualElement::OnInheritedPropertyChanged( const DependencyPropertyPtr& property, const PropertyChangedEventArgs& propertyChangedArgs)
{
    SetInheritedValue(property, propertyChangedArgs.NewValue());
}

void VisualElement::OnPropertyChanged( const DependencyPropertyPtr& property, const PropertyChangedEventArgs& propertyChangedArgs)
{
    auto flags = property->PropertyMetadata()->FrameworkPropertyMetadataOptionsFlags();
    

    auto invalidateFlags = 0;
    if(flags & (int)FrameworkPropertyMetadataOptions::AffectsArrange)
    {
        invalidateFlags |= INVALIDATE_FLAG_ARRANGE;
    }
    if(flags & (int)FrameworkPropertyMetadataOptions::AffectsMeasure)
    {
        invalidateFlags |= INVALIDATE_FLAG_MEASURE;
    }
    if(flags & (int)FrameworkPropertyMetadataOptions::AffectsRender)
    {
        invalidateFlags |= INVALIDATE_FLAG_VISUAL;
    }
    
    if(invalidateFlags)
        Invalidate(invalidateFlags);

    if(flags & (int)FrameworkPropertyMetadataOptions::Inherits)
    {
        for(const VisualElementPtr& child : m_children)
        {
            child->OnInheritedPropertyChanged(property, propertyChangedArgs);
            child->OnPropertyChanged(property, propertyChangedArgs);
        }      
    }
}
