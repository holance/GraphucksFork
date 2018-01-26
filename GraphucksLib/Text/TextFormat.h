#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"
#include <dwrite_1.h>

namespace Graphucks
{
    class TextFormat;
    typedef std::shared_ptr<TextFormat> TextFormatPtr;

    class TextFormat : public IndependantResource
    {
    public:
        TextFormat(void);
        virtual ~TextFormat(void);
    public:
        void FontFamily(const std::wstring& fontFamily);
        const std::wstring& FontFamily() const;

        void FontSize(float fontSize);
        float FontSize() const;
    protected:
        Microsoft::WRL::ComPtr<IUnknown> OnCreateResource();
    private:
        std::wstring m_fontFamily;
        float m_fontSize;
        Microsoft::WRL::ComPtr<IDWriteFactory> m_factory;
    };
}