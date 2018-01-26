#include "stdafx.h"
#include "GraphucksDemo.h"
#include <Graphucks.h>
#include <memory>
#include <ppl.h>
#include <Shlwapi.h>
#include "XamlVectorParser.h"

#pragma comment(lib, "Shlwapi.lib")

using namespace std;
using namespace Graphucks;
using namespace Concurrency;
using namespace Microsoft::WRL;


GraphucksDemo::GraphucksDemo(shared_ptr<Win32SceneRoot>& sceneRoot) : m_counter(0)
{

    ComPtr<IStream> pFileStream;
    HRESULT hr = S_OK;

    VisualElementPtr parsedVisual;
    m_myXform        = gmake_shared<CompositeTransform>();
   
   if (SUCCEEDED(hr = SHCreateStreamOnFile(L"VectorData.xml", STGM_READ, &pFileStream))) 
   {
      parsedVisual = XamlVectorParser::ParseXaml(pFileStream);
      parsedVisual->EnableBitmapCache(true);
      parsedVisual->SetBitmapCacheRenderScale(2);
      auto myXform = gmake_shared<CompositeTransform>();
      myXform->TranslateX(100);
      myXform->TranslateY(200);

      myXform->ScaleX(1);
      myXform->ScaleY(1);
      parsedVisual->RenderTransform(myXform);
   }


    
    auto colorBrush  = gmake_shared<SolidColorBrush>(Color(0,0, 1.0f, 1));
    auto colorBrush2 = gmake_shared<SolidColorBrush>(Color(1.0f, 1.0f, 1.0f, 1));


    auto imageBrush  = gmake_shared<ImageBrush>();

    wstring filePath = L".\\earth.png";
    auto bitmap = Bitmap::LoadFromPath(filePath);
    imageBrush->Image(bitmap);
    imageBrush->Opacity(0.5f);

    auto multiPanel = gmake_shared<MultiContentPanelElement>();

    m_background = gmake_shared<ImageElement>();
    m_background->Margin(Thickness(-100,-100,-100, -100));
    m_background->Stretch(Stretch::Uniform);

    filePath = L"background.jpg";
    bitmap = Bitmap::LoadFromPath(filePath);
    m_background->Source(bitmap);

    multiPanel->AddChild(m_background);

    auto effect = gmake_shared<DropShadowEffect>();
    effect->ShadowColor(Color(1, 0, 0, 1));
    effect->BlurSize(10.0f);

    //auto mediaElement = make_shared<MediaElement>();
    //mediaElement->RenderTransform(m_myXform);
    //mediaElement->SetSource(L"C:\\Users\\Jeremiah\\Desktop\\output1.mp4");
    //mediaElement->SetSize(Size(800, 500));
    //mediaElement->ElementEffect(effect);
    //multiPanel->AddChild(mediaElement);

    radialPanel = gmake_shared<RadialPanel>();
    radialPanel->SetSize(Size(720,720));
    radialPanel->SetHorizontalAlignment(HorizontalAlignment::Center);
    radialPanel->SetVerticalAlignment(VerticalAlignment::Center);

    for (int i = 0; i < 50; i++)
    {
        auto element = gmake_shared<RectangleElement>();
        element->FillBrush(imageBrush);
        element->SetSize(Size(200,200));
        element->SetStretch(Stretch::Fill);
        element->StrokeWidth(6);
        element->StrokeBrush(colorBrush);
        element->RadiusX(10);
        element->RadiusY(10);
        element->EnableBitmapCache(true);
        element->RenderTransform(m_myXform);
        element->Margin(Thickness(10,10,10,10));
        radialPanel->AddChild(element);

        auto token = element->OnPointerDown() += [=](const VisualElementPtr& sender, 
                                                     const Point& point)
        {
            auto myElement = dynamic_pointer_cast<ShapeElement>(sender);

            if(!myElement->ElementEffect())
                myElement->ElementEffect(effect);
            else
                 myElement->ElementEffect(nullptr);
        };
    }


    auto dropShadowEffect = gmake_shared<DropShadowEffect>();
    dropShadowEffect->BlurSize(15);

    m_blurEffect = gmake_shared<BlurEffect>();
    m_background->ElementEffect(m_blurEffect);

    auto txtBlock = gmake_shared<TextBlockElement>();
    txtBlock->Text(L"clickable text!");
    txtBlock->MaximumSize(Size(1580, 800));
    txtBlock->Margin(Thickness(0,10,0,0));
    txtBlock->SetHorizontalAlignment(HorizontalAlignment::Center);
    txtBlock->SetVerticalAlignment(VerticalAlignment::Center);
    txtBlock->FontSize(200);
    txtBlock->Foreground(colorBrush2);
    txtBlock->FontFamily(L"Segoe UI");
    txtBlock->ElementEffect(dropShadowEffect);
    txtBlock->CompositeMode(EffectCompositeMode::MaskInvert);

    auto token = txtBlock->OnPointerDown() += [=](const VisualElementPtr& sender, 
                                                  const Point& point)
    {
        auto myElement = dynamic_pointer_cast<TextBlockElement>(sender);
        auto txt = myElement->Text();

        if(txt == L"clickable text!")
        {
            txt = L"Clicked";
        }
        else
        {
            txt = L"clickable text!";
        }
       
        myElement->Text(txt);
    };

    wstring data = L"M18.134924,10.069491C18.182504,10.070954 18.226784,10.075679 18.267159,10.083492 18.912856,10.208492 21.203145,10.125092 21.287845,12.937584 21.368644,15.750177 21.420642,32.401232 21.420642,32.401232 21.420642,32.401232 21.128945,34.109426 23.087436,33.067829 23.087436,33.067829 22.892137,24.763252 23.808631,24.179853 24.725327,23.596556 26.699218,23.817855 27.199216,24.401254 27.699212,24.984552 28.03271,24.971453 28.03271,33.346531 28.03271,33.346531 29.329604,34.548225 29.933602,33.125031 30.475297,31.846535 29.712403,27.113346 30.865897,26.569048 31.808593,26.123848 34.532681,26.123848 34.724078,28.291742 34.94918,30.823036 34.224082,36.125023 35.642074,35.791723 35.642074,35.791723 36.64207,36.08342 36.724071,34.541725 36.808566,33.000132 37.170566,30.152338 38.420558,29.902338 39.670554,29.652338 41.226447,30.208437 41.309947,32.291733 41.393249,34.375029 42.30334,46.694096 38.865859,55.403671 35.753875,63.291653 36.420569,62.62495 33.475282,63.12495 30.764396,63.584751 26.699218,63.123753 26.699218,63.123753 26.699218,63.123753 17.724162,64.458344 15.642171,57.624966 13.557382,50.791683 10.808695,40.958406 7.3086622,38.958412 3.8086796,36.958418 4.2461775,36.375023 4.2879275,35.354223 4.329677,34.333426 7.6004109,31.250135 12.975385,35.750124 12.975385,35.750124 16.100369,39.791712 16.100369,42.000107 16.100369,42.000107 15.694171,14.302181 15.808671,12.790584 15.964952,10.759308 17.421215,10.047531 18.134924,10.069491z M16.210455,0.0030574799C21.053691,0.097863197 25.793481,2.3955178 28.834798,6.6344271 32.838635,12.21174 32.700637,19.489713 29.050796,24.853518 28.871198,24.18093 28.625102,23.840536 28.293006,23.463443 28.222709,23.382145 28.14071,23.315745 28.060111,23.246147 30.897265,18.531329 30.847667,12.378437 27.44152,7.6330698 22.879092,1.2750912 13.991233,-0.18582344 7.6346729,4.3779364 1.2753935,8.941597 -0.18307352,17.829642 4.3793546,24.186131 6.8480656,27.624269 10.581187,29.616436 14.495125,30.013029 14.501724,30.597519 14.509524,31.174908 14.517425,31.745998 10.062895,31.351505 5.7987621,29.104744 2.9863064,25.187413 -2.1386123,18.048238 -0.5033884,8.1096115 6.6346891,2.986001 9.5344396,0.90457535 12.896662,-0.06180954 16.210455,0.0030574799z";

    auto canvas = gmake_shared<Canvas>();

    auto yellowBrush  = gmake_shared<SolidColorBrush>(Color(0x00/255.0f, 
                                                            0x68/255.0f, 
                                                            0xe4/255.0f, 1));
    auto pathElement = make_shared<PathElement>();

    auto pathEffect = gmake_shared<DropShadowEffect>();
    pathEffect->ShadowColor(Color(0, 0, 0, 1));
    pathEffect->BlurSize(12.0f);

    pathElement->StrokeBrush(yellowBrush);
    pathElement->FillBrush(yellowBrush);
    pathElement->SetStretch(Stretch::Uniform);
    pathElement->Geometry(PathElement::Parse(data));
    pathElement->ElementEffect(pathEffect);
    pathElement->SetSize(Size(300, 300));

    canvas->SetLeft(pathElement, 200);
    canvas->SetTop(pathElement, 200);

    canvas->AddChild(pathElement);

    multiPanel->AddChild(radialPanel);
    multiPanel->AddChild(txtBlock);

    multiPanel->AddChild(canvas);

    if(parsedVisual)
    {
        parsedVisual->ElementEffect(dropShadowEffect);
        multiPanel->AddChild(parsedVisual);
    }
    sceneRoot->SetRoot(multiPanel);
}

GraphucksDemo::~GraphucksDemo(void)
{
}

void GraphucksDemo::Update()
{
    m_myXform->Rotation(m_myXform->Rotation() + 1);

    m_counter += 0.03f;

    auto x = abs(std::sin(m_counter/2)) * 300;
    auto y = abs(std::sin(m_counter)) * 45;
    auto z = (abs(std::sin(m_counter)) * 3) + 1.5;
    
    m_blurEffect->BlurSize(y);

    //radialPanel->RadiusDivisor(z);

    m_background->Margin(Thickness(-x,-x,-x,-x));
}
