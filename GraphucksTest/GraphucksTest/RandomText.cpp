#include "pch.h"
#include "RandomText.h"
#include <memory>
#include <math.h>

using namespace std;
using namespace Graphucks;


RandomText::RandomText(void) : m_animationVal(0), m_testVal(20)
{
    m_stackPanel = make_shared<StackPanel>();

    AddVisualChild(m_stackPanel);
    m_textTransform  = make_shared<CompositeTransform>();
    m_textTransform2 = make_shared<CompositeTransform>();

    auto textForeground = make_shared<SolidColorBrush>(Color(0.5f, 0.3f, 0.0f, 1.0f));

    {
        auto textBlock = make_shared<TextBlockElement>();
        textBlock->Text(L"THIS IS TEXT");
        textBlock->CompositeMode(EffectCompositeMode::Plus);
       //textBlock->FontSize(100);
        textBlock->Foreground(textForeground);
        m_stackPanel->AddChild(textBlock);
    }

    {
        auto textBlock = make_shared<TextBlockElement>();
        textBlock->Text(L"Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Nam cursus. Morbi ut mi. Nullam enim leo, egestas id, condimentum at, laoreet mattis, massa. Sed eleifend nonummy diam. Praesent mauris ante, elementum et, bibendum at, posuere sit amet, nibh. ");
        //textBlock->FontSize(25);
        textBlock->SetSize(Size(600, Graphucks::Infinity()));
        textBlock->Foreground(textForeground);
        textBlock->RenderTransform(m_textTransform2);
        textBlock->CompositeMode(EffectCompositeMode::Plus);
        m_stackPanel->AddChild(textBlock);
    }
    

    {
        auto effect = make_shared<BlurEffect>();
     
        effect->BlurSize(5);

        m_clickableTextblock = make_shared<TextBlockElement>();
        m_clickableTextblock->Text(L"CLICK THIS TEXT");
        m_clickableTextblock->FontSize(200);
        m_clickableTextblock->Foreground(textForeground);
        m_clickableTextblock->ElementEffect(effect);
        m_clickableTextblock->RenderTransform(m_textTransform);
        m_clickableTextblock->CompositeMode(EffectCompositeMode::Plus);
        m_stackPanel->AddChild(m_clickableTextblock);
    }
}


RandomText::~RandomText(void)
{
}

void RandomText::Rendering()
{
    m_animationVal += 0.01f;
    auto translate = abs(sin(m_animationVal)) * -400;
    auto translate2 = abs(sin(m_animationVal)) * 400;

   // m_textTransform->TranslateY(translate);
    m_textTransform2->TranslateX(translate2);
}

void RandomText::Loaded()
{
    m_clickToken = m_clickableTextblock->OnPointerDown() += [&] (VisualElementPtr sender, const Point& point)
    {
        if(!m_clickableTextblock->Text().compare(L"TEXT WAS CLICKED"))
        {
            m_clickableTextblock->Text(L"CLICK ME");
            this->SetValue(TextBlockElement::FontFamilyProperty, std::wstring(L"Segoe"));
        }
        else
        {
            m_clickableTextblock->Text(L"TEXT WAS CLICKED");
            m_testVal += 3;

            /* test property inheritance */
            this->SetValue(TextBlockElement::FontSizeProperty, (float)m_testVal);
            this->SetValue(TextBlockElement::FontFamilyProperty, std::wstring(L"Courier New"));
            if (m_testVal > 30)
            {
                this->ClearValue(TextBlockElement::FontSizeProperty);
            }
        }
    };
}

void RandomText::Unloaded()
{
     m_clickableTextblock->OnPointerDown() -= m_clickToken;
}
