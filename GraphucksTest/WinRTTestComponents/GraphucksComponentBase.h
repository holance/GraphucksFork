#pragma once
namespace WinRTTestComponents
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class GraphucksComponent : public Windows::UI::Xaml::Controls::Control
    {
    internal:
        GraphucksComponent();
    protected:
         virtual void OnApplyTemplate() override;
         virtual void OnInitialized(Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl){};
    };
}
