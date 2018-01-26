#pragma once
#include "GraphucksComponentBase.h"
#include <Graphucks.h>
namespace WinRTTestComponents
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class MediaElementTest sealed : public GraphucksComponent
    {
    public:
        MediaElementTest();
    public:
        void SetBackbufferSize(Windows::Foundation::Size size);
        void SetSource(Windows::Storage::Streams::IRandomAccessStream^ source);
    protected:
        virtual void OnInitialized(Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl) override;
    private:
        Graphucks::MediaElementPtr                      m_mediaElement;
        Graphucks::XamlSceneRoot                        ^m_xamlSceneRoot;
        Windows::Storage::Streams::IRandomAccessStream^ m_source;
    };
}