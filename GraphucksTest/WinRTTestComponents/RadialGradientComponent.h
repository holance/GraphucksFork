#pragma once
#include "GraphucksComponentBase.h"
namespace WinRTTestComponents
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class RadialGradientComponent sealed : public GraphucksComponent
    {
    public:
        RadialGradientComponent();
    protected:
        virtual void OnInitialized(Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl) override;
    };
}