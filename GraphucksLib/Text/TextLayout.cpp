#include "pch.h"
#include "TextLayout.h"
#include <dwrite_1.h>
#include "RenderContext.h"
#include "..\Core\Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace std;

TextLayout::TextLayout(void)
{
    m_factory = RenderContext::GetDirectWriteFactory();
    m_textFormat = make_shared<TextFormat>();
}

TextLayout::~TextLayout(void)
{
}

auto TextLayout::OnCreateResource() -> ComPtr<IUnknown> 
{
     auto textFormat = GetResourceFrom<IDWriteTextFormat>(m_textFormat);

     ComPtr<IDWriteTextLayout> textLayout;
     HRESULT hr = m_factory->CreateTextLayout(m_text.c_str(), 
                                              (UINT32)m_text.length(), 
                                              textFormat, 
                                              m_maxSize.width, 
                                              m_maxSize.height, 
                                              &textLayout);
     Check(hr);
     return textLayout;
}

auto Graphucks::TextLayout::GetLayoutBounds() -> const Graphucks::Rectangle
{
    auto textLayout = GetResource<IDWriteTextLayout>();

    DWRITE_TEXT_METRICS metrics;
    textLayout->GetMetrics(&metrics);
    
    return Graphucks::Rectangle(metrics.left,
                                metrics.top, 
                                metrics.widthIncludingTrailingWhitespace + metrics.left, 
                                metrics.height + metrics.top);
}

void Graphucks::TextLayout::MaxSize(const Size& maxsize)
{
    if (maxsize != m_maxSize)
    {
        m_maxSize = maxsize;
        auto textLayout = GetResource<IDWriteTextLayout>();
        textLayout->SetMaxHeight(maxsize.height);
        textLayout->SetMaxWidth(maxsize.width);
    } 
}

void TextLayout::Text(const std::wstring& text)
{
	if(text == m_text)
		return;

    m_text = text;
    Invalidate();
}

auto TextLayout::Text() const -> const wstring&
{
    return m_text;
}

auto TextLayout::IsResourceValid() const -> bool
{
    if(!IndependantResource::IsResourceValid())
        return false;


    if(!IndependantResource::IsResourceValid(m_textFormat))
        return false;

    return true;
}

bool TextLayout::HitTest( const Point& point)
{
    bool result = false;
    auto txtLayout = GetResource<IDWriteTextLayout>();

    BOOL isTrailingHit = false;
    BOOL isInside = false;

    DWRITE_HIT_TEST_METRICS metrics;

    txtLayout->HitTestPoint(point.x, point.y, &isTrailingHit, &isInside, &metrics);
   
    result = isInside && metrics.isText /* this correct? */;

    return result != 0;
}


void TextLayout::FontSize(float fontSize)
{
    m_fontSize = fontSize;
    m_textFormat->FontSize(fontSize);
    Invalidate();
}

float TextLayout::FontSize() const
{
    return m_fontSize;
}

const std::wstring& TextLayout::FontFamily() const
{
    return m_fontFamily;
}



void TextLayout::FontFamily(const wstring& fontFamily)
{
    m_fontFamily = fontFamily;
    m_textFormat->FontFamily(fontFamily);
    Invalidate();
}
