#pragma once
#include "VisualElement.h"
#include "..\Media\Bitmap.h"

namespace Graphucks
{
    class ImageElement : public VisualElement
    {
    public:
        ImageElement(void);
        virtual ~ImageElement(void);

        const BitmapPtr& Source();
        void Source(const BitmapPtr& bitmapSource);

        auto Stretch() const -> Graphucks::Stretch;
        void Stretch(Graphucks::Stretch stretch);

        auto StretchDirection() const -> Graphucks::StretchDirection;
        void StretchDirection(Graphucks::StretchDirection val);
    protected:
		virtual void RenderOverride(const IRenderContextPtr& renderContext);
        virtual Size MeasureOverride(const Graphucks::Size& availableSize);
        virtual Size ArrangeOverride(const Graphucks::Size& finalSize);
    private:
        Size CalculateScale(const Size& availableSize);
    private:
        BitmapPtr                   m_bitmap;
        Graphucks::Stretch          m_stretch;
        Graphucks::StretchDirection m_stretchDirection;

    };
}
