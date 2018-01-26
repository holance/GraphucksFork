#pragma once
#include <vector>
#include "DependencyObject.h"
#include "..\Rendering\IRenderContext.h"
#include "..\Core\Transform.h"
#include "..\Effects\ElementEffect.h"

#include "Delegate.h"

namespace Graphucks
{


	class VisualElement;
	typedef std::shared_ptr<VisualElement> VisualElementPtr;

    class SceneRoot;
    
	class VisualElement : public std::enable_shared_from_this<VisualElement>, public DependencyObject
    {
    public:
        VisualElement();
		virtual ~VisualElement(){}
    public: 
        DP_PROPERTY(RenderTransform)
        static const DependencyPropertyPtr ElementEffectProperty;
    protected:
        virtual void Invalidate(int flags) final /* IInvalidatable */;
    public:
		VisualElement* GetParent();
    protected:
		void AddVisualChild(const VisualElementPtr& element);
        void RemoveVisualChild(const VisualElementPtr& element);
        auto GetVisualChild(int index) -> VisualElementPtr;
        auto GetVisualChildCount() -> int;
        void RemoveAllChildren();
    protected:
        virtual void Loaded();
        virtual void Unloaded();
    private:
        void InvokeUnloaded();
        void InvokeLoaded();
    public:
        virtual bool HitTest(const Point& point);
        void HitTestTree(const Point&point, VisualElementPtr& hitElement);

        const PointerDownEvent& OnPointerDown();
    public:
        void RenderTransform(const TransformPtr& transform);
        auto RenderTransform() -> const TransformPtr;

        void RenderTransformOrigin(const Point& point);
    public:
        auto GetSize() const -> const Graphucks::Size&;
        void SetSize(Graphucks::Size& size);

        auto MinimumSize() const -> const Graphucks::Size& ;
        void MinimumSize(const Graphucks::Size& size);

        auto MaximumSize() -> const Size&;
        void MaximumSize(const Graphucks::Size& size);

        auto GetVerticalAlignment() -> const VerticalAlignment;
        void SetVerticalAlignment(VerticalAlignment alignment);

        auto GetHorizontalAlignment() -> const HorizontalAlignment;
        void SetHorizontalAlignment(HorizontalAlignment alignment);

        auto DesiredSize() -> const Size&;

        void Measure(const Size& availableSize);
        void Arrange(const Rectangle& finalRect);

        void InvalidateArrange();
        void InvalidateMeasure();

        auto GetRenderSize() -> const Size&;

        auto Margin() -> const Thickness& ;
        void Margin(const Thickness& margin);

        void InvalidateVisual();
        void EnableBitmapCache(bool enable);
        void SetBitmapCacheRenderScale(float scale);

        void ClipToBounds(bool clipToBounds);
        auto ClipToBounds() const -> bool;
        void ElementEffect(const ElementEffectPtr& elementEffect);
        auto ElementEffect() -> ElementEffectPtr;
        EffectCompositeMode CompositeMode();
        void CompositeMode(EffectCompositeMode mode);

        bool Loaded() const { return m_loaded; }
    private:
        void ClipRect(const Graphucks::Rectangle& clipRect);
        void ClipEnable(bool enabled);
        void RenderSizeChanged(const Graphucks::Size& newSize);
    protected:
		virtual void RenderOverride(const IRenderContextPtr& renderContext);
    protected:
        virtual Size MeasureOverride(const Graphucks::Size& availableSize);
        virtual Size ArrangeOverride(const Graphucks::Size& finalSize);

        virtual void Rendering();

        bool IsRooted();
    protected:
        const D2D1::Matrix3x2F& RealSceneTransform();
        virtual void OnPropertyChanged( const DependencyPropertyPtr& property, const PropertyChangedEventArgs& e) override;
        void OnInheritedPropertyChanged( const DependencyPropertyPtr& property, const PropertyChangedEventArgs& e);
	private:
		VisualElement(const VisualElement&); // not implemented
        VisualElement& operator = (const VisualElement&); // not implemented
    private:
        HRESULT SetParent(VisualElement* parent);
		void RenderImpl(const IRenderContextPtr& renderContext);
        virtual void UpdateImpl(LONGLONG time);
        void CalculateTransform();


        void RenderingImpl();/*temp anim solution*/
    private:
        void EnsureBitmapCache(const Graphucks::Size& size, const int maximumBitmapSize);
    private:
        void CalculateMinMax(Size& minSize, Size& maxSize);
    private:
        void SetDesiredSize(Size& size);
    protected:
        virtual void OnParentChanged(const VisualElement* oldParent, const VisualElement* newParent);
	private:
        Size                          m_desiredSize;
        std::vector<VisualElementPtr> m_children;
        D2D1::Matrix3x2F              m_transform;
        D2D1::Matrix3x2F              m_lastRenderTransform;
        D2D1::Matrix3x2F              m_realSceneTransform; //For hittesting
		VisualElement*                m_parentElement;
        Graphucks::Rectangle          m_clipRect;
        bool                          m_clipEnabled;
        Size                          m_size;
        Size                          m_minSize;
        Size                          m_maxSize;
        Size                          m_unclippedDesiredSize;
        HorizontalAlignment           m_horizontalAlignment;
        VerticalAlignment             m_verticalAlignment;

     
        Point                         m_renderTransformOrigin;
        Size                          m_renderSize;
        Thickness                     m_margin;

        bool                          m_arrangeDirty;
        bool                          m_measureDirty;
        bool                          m_transformDirty;
        bool                          m_renderInvalid;

        BitmapPtr                     m_bitmapCache;
        bool                          m_bitmapCacheEnabled;
        bool                          m_bitmapCachedInvalid;
        float                         m_bitmapCacheRenderAtScale;

        bool                          m_clipToBounds;
        Point                         m_layoutOffset;
        PointerDownEvent              m_pointerDownEvent;
        EffectCompositeMode           m_compositeMode;

        bool                          m_loaded;
        bool                          m_isRoot;
        friend SceneRoot;
	};
}
