#include "pch.h"
#include "GraphucksApp.h"
#include <ppltasks.h>
#include "FauxLights.h"
#include <math.h>

using namespace std;
using namespace Graphucks;
using namespace Concurrency;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;

GraphucksApp::GraphucksApp(void) : m_clickedEventToken(0), m_scaleAnimValue(0)
{
    m_mainPanel = make_shared<MultiContentPanelElement>();
    AddVisualChild(m_mainPanel);

    auto background = make_shared<ImageElement>();
    background->Stretch(Stretch::Fill);
    
    m_backgroundTransform = make_shared<CompositeTransform>();
    background->RenderTransform(m_backgroundTransform);
    background->RenderTransformOrigin(Point(0, 0));
    
    auto filePath = ref new Windows::Foundation::Uri(L"ms-appx:///background.jpg");

    Bitmap::LoadFromApplicationUrl(filePath).then([=](task<BitmapPtr> bitmapTask)
    {
        auto bitmap = bitmapTask.get();
        background->Source(bitmap);
    });

    m_backgroundEffect = make_shared<SaturationEffect>();
    background->ElementEffect(m_backgroundEffect);

    m_mainPanel->AddChild(background);
     /* m_mediaElement = make_shared<Graphucks::MediaElement>();
    auto effect = make_shared<SaturationEffect>();
     m_mainPanel->AddChild(m_mediaElement);*/
    auto fauxLights = make_shared<FauxLights>();

    m_mainPanel->AddChild(fauxLights);
   
    SetValue(TextBlockElement::FontSizeProperty, 5.0f);

   /*
    m_mediaElement->ElementEffect(effect);
    effect->SaturationAmount(0);
    m_mediaElement->RenderTransform(m_backgroundTransform);

    FileOpenPicker^ getVidFile = ref new FileOpenPicker();

    getVidFile->SuggestedStartLocation = PickerLocationId::VideosLibrary;
    getVidFile->ViewMode = PickerViewMode::Thumbnail;

    getVidFile->FileTypeFilter->Clear();
    getVidFile->FileTypeFilter->Append(".mp4");
    getVidFile->FileTypeFilter->Append(".m4v");
    getVidFile->FileTypeFilter->Append(".mts");
    getVidFile->FileTypeFilter->Append(".mov");
    getVidFile->FileTypeFilter->Append(".wmv");
    getVidFile->FileTypeFilter->Append(".avi");
    getVidFile->FileTypeFilter->Append(".asf");
    

    auto pickFileTask = task<StorageFile^>(getVidFile->PickSingleFileAsync());

    pickFileTask.then([=](StorageFile^ fileHandle)
    {
        try
        {
            if(!fileHandle)
            {
                return;
            }

            task<IRandomAccessStream^> fOpenStreamTask(fileHandle->OpenAsync(Windows::Storage::FileAccessMode::Read));

            
            fOpenStreamTask.then([=](IRandomAccessStream^ streamHandle)
            {
                try
                {
                    m_mediaElement->SetSource(streamHandle);
                } 
                catch(Platform::Exception^)
                {
                   
                }
            });

        } 
        catch(Platform::Exception^)
        {

        }
    });*/
}

GraphucksApp::~GraphucksApp(void)
{
    
}

void GraphucksApp::Loaded()
{

}

void GraphucksApp::Unloaded()
{
   
}

void GraphucksApp::Rendering()
{
    m_scaleAnimValue += 0.05f;
  
    auto scale = abs(sin(m_scaleAnimValue)) * 0.2f + 1;

    m_backgroundTransform->ScaleX(scale);
    m_backgroundTransform->ScaleY(scale);

    auto saturation = abs(sin(m_scaleAnimValue));

    m_backgroundEffect->SaturationAmount(saturation);
}
