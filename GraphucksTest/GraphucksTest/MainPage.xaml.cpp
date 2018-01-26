#include "pch.h"
#include <memory>
#include <Graphucks.h>
#include "GraphucksApp.h"
#include "MainPage.xaml.h"
#include <atomic>

using namespace Graphucks;
using namespace GraphucksTest;
using namespace std;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;

using namespace D2D1;
using namespace Concurrency;

MainPage::MainPage()
{
	InitializeComponent();
}

void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}

void GraphucksTest::MainPage::Page_Loaded_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    auto xamlSceneRoot = ref new XamlSceneRoot();
    xamlSceneRoot->SetBackBufferSize(Graphucks::Size(1366,768));
    
    this->xamlContentControl->Content = xamlSceneRoot;

    auto root  = make_shared<GraphucksApp>();
   
    xamlSceneRoot->SetRoot(root);
}


void GraphucksTest::MainPage::Slider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
    if (xamlContentControl != nullptr)

    {

        xamlContentControl->Width = ActualWidth * e->NewValue;

        xamlContentControl->Height =ActualHeight * e->NewValue;

    }

}
