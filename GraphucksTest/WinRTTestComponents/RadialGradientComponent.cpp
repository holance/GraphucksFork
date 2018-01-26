#include "pch.h"
#include "RadialGradientComponent.h"
#include <Graphucks.h>

using namespace std;
using namespace WinRTTestComponents;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Markup;
using namespace Graphucks;

RadialGradientComponent::RadialGradientComponent(void)
{
}

void RadialGradientComponent::OnInitialized( Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl )
{
    auto xamlSceneRoot = ref new XamlSceneRoot();
    rootXamlContentControl->Content = xamlSceneRoot;
    xamlSceneRoot->SetBackBufferSize(Graphucks::Size(500, 500));

    auto gsc = make_shared<GradientStopCollection>();
      
    gsc->AddGradientStop(GradientStop(0, Color(1.0f, 0.0f, 0.0f, 1.0f)));
    gsc->AddGradientStop(GradientStop(1, Color(0.0f, 0.0f, 1.0f, 0.0f)));

    auto brush = make_shared<RadialGradientBrush>(gsc,
                                                  Graphucks::Point(0.5f, 0.5f), 
                                                  Graphucks::Point(0.0f, 0.0f), 
                                                  0.5f, 
                                                  0.5f);

    auto ellipseElement = make_shared<EllipseElement>();
       
    ellipseElement->FillBrush(brush);
    
    xamlSceneRoot->SetRoot(ellipseElement);
}
