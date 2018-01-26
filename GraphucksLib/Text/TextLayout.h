#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"
#include "TextFormat.h"

namespace Graphucks
{
    class TextLayout;
    typedef std::shared_ptr<TextLayout> TextLayoutPtr;

    typedef Microsoft::WRL::ComPtr<IDWriteFactory> IDWriteFactoryPtr;

    class TextLayout : public IndependantResource
    {
    public:
        TextLayout(void);
        virtual ~TextLayout(void);

        void MaxSize(const Graphucks::Size& size);
        void Text(const std::wstring& text);
        auto Text() const -> const std::wstring&;

        void FontFamily(const std::wstring& fontFamily);
        auto FontFamily() const -> const std::wstring&;

        void FontSize(float fontSize);
        float FontSize() const;

        auto GetLayoutBounds() -> const Graphucks::Rectangle;

        bool HitTest(const Point& point);
    protected:
        virtual auto OnCreateResource() -> Microsoft::WRL::ComPtr<IUnknown> override;
        virtual auto IsResourceValid() const -> bool override;
    private:
        std::wstring      m_text;
        std::wstring      m_fontFamily;
        float             m_fontSize;
        Graphucks::Size   m_maxSize;
        IDWriteFactoryPtr m_factory;
        TextFormatPtr     m_textFormat;
    };
}
