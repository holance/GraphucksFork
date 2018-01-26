#include "pch.h"
#include "TextBlockElement.h"

using namespace Graphucks;
using namespace std;



void TextBlockElement::FontSizeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
    auto textBlock = static_cast<TextBlockElement*>(ptr);
    auto newValue = changedArgs.NewValue<float>();
    textBlock->m_textLayout->FontSize(newValue);
}

const DependencyPropertyPtr TextBlockElement::FontSizeProperty = DependencyProperty::Register<TextBlockElement/*ownerType*/>(L"FontSize", 
                                                                                              FrameworkPropertyMetadata::Create<float/*dp type*/>(
                                                                                                  5.0f, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender | 
                                                                                                  (int)FrameworkPropertyMetadataOptions::Inherits, 
                                                                                                  &FontSizeChanged)
                                                                                               );


void TextBlockElement::FontFamilyChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
    auto textBlock = static_cast<TextBlockElement*>(ptr);
    auto newVal = changedArgs.NewValue<wstring>();
 
    textBlock->m_textLayout->FontFamily(newVal);
}

const DependencyPropertyPtr TextBlockElement::FontFamilyProperty = DependencyProperty::Register<TextBlockElement/*ownerType*/>(L"FontFamily", 
                                                                                              FrameworkPropertyMetadata::Create<wstring>(
                                                                                                  wstring(L"Arial"), 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender | 
                                                                                                  (int)FrameworkPropertyMetadataOptions::Inherits, 
                                                                                                  &FontFamilyChanged)
                                                                                               );



void TextBlockElement::TextChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
    auto textBlock = static_cast<TextBlockElement*>(ptr);
    auto newVal = changedArgs.NewValue<wstring>();
 
    textBlock->m_textLayout->Text(newVal);
}

const DependencyPropertyPtr TextBlockElement::TextProperty = DependencyProperty::Register<TextBlockElement/*ownerType*/>(L"Text", 
                                                                                              FrameworkPropertyMetadata::Create<wstring>(
                                                                                                  wstring(L""), 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender, 
                                                                                                  &TextChanged)
                                                                                               );

const DependencyPropertyPtr TextBlockElement::BackgroundProperty = DependencyProperty::Register<TextBlockElement/*ownerType*/>(L"Background", 
                                                                                              FrameworkPropertyMetadata::Create<BrushPtr/*dp type*/>(
                                                                                                  nullptr, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender,
                                                                                                  nullptr)
                                                                                               );

const DependencyPropertyPtr TextBlockElement::ForegroundProperty = DependencyProperty::Register<TextBlockElement/*ownerType*/>(L"Foreground", 
                                                                                              FrameworkPropertyMetadata::Create<BrushPtr/*dp type*/>(
                                                                                                  nullptr, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender,
                                                                                                  nullptr)
                                                                                               );


void TextBlockElement::Text(const std::wstring& text)
{
    SetValue(TextProperty, text);
}

auto TextBlockElement::Text() -> const std::wstring
{
    return GetValue<wstring>(TextProperty);
}

auto TextBlockElement::FontFamily() -> const std::wstring
{
    return GetValue<wstring>(FontFamilyProperty);
}

void TextBlockElement::FontFamily(const wstring& fontFamily)
{
    SetValue(FontFamilyProperty, fontFamily);
}

void TextBlockElement::FontSize(float fontSize)
{
    SetValue(FontSizeProperty, fontSize);
}

auto TextBlockElement::FontSize() -> float 
{
    return GetValue<float>(FontSizeProperty);
}

TextBlockElement::TextBlockElement(void)
{
    m_textLayout = make_shared<TextLayout>();

    auto fontsize = FontSize();

    m_textLayout->FontSize(fontsize);
    m_textLayout->FontFamily(FontFamily());
    m_textLayout->Text(Text());
}

TextBlockElement::~TextBlockElement(void)
{
}

auto TextBlockElement::MeasureOverride(const Graphucks::Size& availableSize) -> Size
{
    m_textLayout->MaxSize(availableSize);
    auto bounds = m_textLayout->GetLayoutBounds();

    return bounds.Size();
}

auto TextBlockElement::ArrangeOverride(const Graphucks::Size& finalSize) -> Size
{
    m_textLayout->MaxSize(finalSize);

    auto bounds = m_textLayout->GetLayoutBounds();
    return finalSize;//bounds.Size();
}

void TextBlockElement::RenderOverride(const IRenderContextPtr& renderContext)
{
    auto backGround = Background();

    if(backGround)
    {
        auto bounds = m_textLayout->GetLayoutBounds();
        auto boundsSize = bounds.Size();
        if(!m_backgroundRect || boundsSize != m_lastSize)
        {
            Graphucks::Rectangle rect(0, 0, boundsSize.width, boundsSize.height);
            m_backgroundRect = make_shared<RoundedRectangle>(rect, 0.0f, 0.0f);
            m_lastSize = bounds.Size();
        }
        renderContext->DrawGeometry(m_backgroundRect, backGround, nullptr);
    }

    auto foreGround = Foreground();
    if(foreGround)
    {
        renderContext->DrawText(m_textLayout, foreGround, Point(0,0));
    }
    return;
}


void TextBlockElement::Background(const BrushPtr& brush)
{
    SetValue(BackgroundProperty, brush);
}

auto TextBlockElement::Background() -> const BrushPtr 
{
    return GetValue<Brush>(BackgroundProperty);
}

void TextBlockElement::Foreground(const BrushPtr& brush)
{
    SetValue(ForegroundProperty, brush);
}

auto TextBlockElement::Foreground() -> const BrushPtr 
{
    return GetValue<Brush>(ForegroundProperty);
}

bool TextBlockElement::HitTest( const Point& point )
{
    bool result = false;
    auto background = Background();

    if(background)
       result = m_backgroundRect->HitTest(point);

    if(!m_textLayout)
        return false;

    if(result)
        return result;

    return m_textLayout->HitTest(point);
}
