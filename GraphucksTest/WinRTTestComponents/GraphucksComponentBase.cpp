#include "pch.h"
#include "GraphucksComponentBase.h"

using namespace std;
using namespace WinRTTestComponents;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Markup;


void WinRTTestComponents::GraphucksComponent::OnApplyTemplate()
{
    auto cc = safe_cast<Windows::UI::Xaml::Controls::ContentControl^>(GetTemplateChild("root"));

    OnInitialized(cc);
    
    FrameworkElement::OnApplyTemplate();
}

WinRTTestComponents::GraphucksComponent::GraphucksComponent()
{
    auto controlTemplate = safe_cast<ControlTemplate^>    
        (XamlReader::Load(L"<ControlTemplate xmlns=\"http://schemas.microsoft.com/winfx/2006/xaml/presentation\" \
                           xmlns:x=\"http://schemas.microsoft.com/winfx/2006/xaml\"> \
                           <ContentControl x:Name=\"root\"/> \
                           </ControlTemplate>"));

    Template = controlTemplate;
}
