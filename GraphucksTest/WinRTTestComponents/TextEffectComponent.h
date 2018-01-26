#pragma once
#include "GraphucksComponentBase.h"
namespace WinRTTestComponents
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class TextEffectComponent sealed : public GraphucksComponent
    {
    public:
        TextEffectComponent();
    protected:
        virtual void OnInitialized(Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl) override;
    };
}