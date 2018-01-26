#include "pch.h"
#include "MediaElementTest.h"
#include <Graphucks.h>

using namespace std;
using namespace WinRTTestComponents;
using namespace Platform;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Markup;
using namespace Graphucks;

MediaElementTest::MediaElementTest(void)
{
}

void MediaElementTest::OnInitialized( Windows::UI::Xaml::Controls::ContentControl^ rootXamlContentControl )
{
    auto xamlSceneRoot = m_xamlSceneRoot =  ref new XamlSceneRoot();
    rootXamlContentControl->Content = xamlSceneRoot;
    xamlSceneRoot->SetBackBufferSize(Graphucks::Size(1920, 886));

    m_mediaElement = make_shared<Graphucks::MediaElement>();
	auto blurEffect = make_shared<Graphucks::BlurEffect>();
	blurEffect->BlurSize(10);

    //m_mediaElement->ElementEffect(blurEffect);
    m_mediaElement->Margin(Graphucks::Thickness(20,20,20,20));

    if(m_source)
    {
        m_mediaElement->SetSource(m_source);
        m_source = nullptr;
    }

    xamlSceneRoot->SetRoot(m_mediaElement);
}

void MediaElementTest::SetSource( Windows::Storage::Streams::IRandomAccessStream^ source )
{
    if(!m_mediaElement)
    {
        m_source = source;
        return;
    }

    m_mediaElement->SetSource(source);
}

void MediaElementTest::SetBackbufferSize( Windows::Foundation::Size size )
{
    m_xamlSceneRoot->SetBackBufferSize(Graphucks::Size(size.Width, size.Height));
}
