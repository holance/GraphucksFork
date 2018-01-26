#pragma once
#include "VisualElement.h"
#include "..\Text\TextLayout.h"
#include "..\Geometry\RoundedRectangle.h"

namespace Graphucks
{
    class TextBlockElement;
    typedef std::shared_ptr<TextBlockElement> TextBlockElementPtr;

    class TextBlockElement : public VisualElement
    {
    public: 
        static const DependencyPropertyPtr FontSizeProperty;
        static const DependencyPropertyPtr FontFamilyProperty;
        static const DependencyPropertyPtr TextProperty;
        static const DependencyPropertyPtr BackgroundProperty;
        static const DependencyPropertyPtr ForegroundProperty;
    private:
        static void FontSizeChanged   (DependencyObject* const ptr, const PropertyChangedEventArgs& changed);
        static void FontFamilyChanged (DependencyObject* const ptr, const PropertyChangedEventArgs& changed);
        static void TextChanged       (DependencyObject* const ptr, const PropertyChangedEventArgs& changed);
    public:
        TextBlockElement(void);
        virtual ~TextBlockElement(void);

        void Text(const std::wstring& text);
        auto Text() -> const std::wstring;

        void FontFamily(const std::wstring& fontFamily);
        auto FontFamily() -> const std::wstring;

        void FontSize(float fontSize);
        float FontSize();

        void Background(const BrushPtr& brush);
        const BrushPtr Background();

        void Foreground(const BrushPtr& brush);
        const BrushPtr Foreground();
    protected:
        virtual auto MeasureOverride(const Graphucks::Size& availableSize)  -> Size override;
        virtual auto ArrangeOverride(const Graphucks::Size& finalSize)      -> Size override;
        virtual void RenderOverride(const IRenderContextPtr& renderContext)         override;
        virtual bool HitTest(const Point& point)                                    override;
    private:
        TextLayoutPtr       m_textLayout;
        RoundedRectanglePtr m_backgroundRect;
        Size                m_lastSize;
    };
}
