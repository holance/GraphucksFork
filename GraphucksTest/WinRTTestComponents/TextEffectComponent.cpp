#include "pch.h"
#include "TextEffectComponent.h"
#include <Graphucks.h>
#include <string>

using namespace std;
using namespace WinRTTestComponents;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Markup;
using namespace Graphucks;

TextEffectComponent::TextEffectComponent()
{
    
}

void TextEffectComponent::OnInitialized( Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl )
{
    auto xamlSceneRoot = ref new XamlSceneRoot();
    rootXamlContentControl->Content = xamlSceneRoot;
    xamlSceneRoot->SetBackBufferSize(Graphucks::Size(1366, 350));

    auto text = make_shared<TextBlockElement>();
    text->FontSize(200);
    auto brush = make_shared<SolidColorBrush>(Graphucks::Color(1,1,1,1));
   

    text->Text(L"GRAPHUCKS");
    text->Foreground(brush);
    text->Margin(Graphucks::Thickness(25,25,25,25));
    auto blur = make_shared<BlurEffect>();

    blur->BlurSize(10);

    text->ElementEffect(blur);
    xamlSceneRoot->SetRoot(text);
}
