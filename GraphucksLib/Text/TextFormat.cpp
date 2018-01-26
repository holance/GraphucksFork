#include "pch.h"
#include "TextFormat.h"
#include "RenderContext.h"
#include "..\Core\Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace std;

TextFormat::TextFormat(void) : m_fontFamily(L"Segoe UI Light"), m_fontSize(30)
{
    m_factory = RenderContext::GetDirectWriteFactory();
}

TextFormat::~TextFormat(void)
{
}

ComPtr<IUnknown> Graphucks::TextFormat::OnCreateResource()
{
    ComPtr<IDWriteTextFormat> textFormat;

    auto hr = m_factory->CreateTextFormat(
            m_fontFamily.c_str(),
            nullptr,
            DWRITE_FONT_WEIGHT_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            m_fontSize,
            L"", //locale
            &textFormat
           );
    
    Check(hr);
    return textFormat;
}

void TextFormat::FontSize(float fontSize)
{
    m_fontSize = fontSize;
    Invalidate();
}

float TextFormat::FontSize() const
{
    return m_fontSize;
}

const std::wstring& TextFormat::FontFamily() const
{
    return m_fontFamily;
}

void TextFormat::FontFamily(const wstring& fontFamily)
{
    m_fontFamily = fontFamily;
    Invalidate();
}
