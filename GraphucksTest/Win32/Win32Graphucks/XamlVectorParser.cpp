#include "stdafx.h"
#include "XamlVectorParser.h"
#include "xmllite.h"
#include <map>
#include <string>
#include <strstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace Microsoft::WRL;
using namespace Graphucks;

#pragma comment(lib, "xmllite.lib")

struct ParsingContext
{
    map<wstring, BrushPtr> brushResources;
};

bool GetAttributeString(wchar_t* attributeName, const wchar_t* namespaceUri, const wchar_t** result, const ComPtr<IXmlReader>& reader)
{
    if(reader->MoveToAttributeByName(attributeName, namespaceUri) != S_OK)
        return false;

    UINT len;
    return reader->GetValue(result, &len) == S_OK;
}

const bool GetAttributeDouble(wchar_t* attributeName, const wchar_t* namespaceUri, double& result, const ComPtr<IXmlReader>& reader)
{
    const wchar_t* val;

    if(!GetAttributeString(attributeName, namespaceUri, &val, reader))
        return false;

    result = _wtof(val);

    return true;
}

const bool GetAttributeFloat(wchar_t* attributeName, const wchar_t* namespaceUri, float& result, const ComPtr<IXmlReader>& reader)
{
   double doubleResult = 0;

   auto ret = GetAttributeDouble(attributeName, namespaceUri, doubleResult, reader);

   if(!ret)
       return ret;

   result = (float)doubleResult;

   return ret;
}

void ReadCanvas(const CanvasPtr& canvas, const ComPtr<IXmlReader>& pReader)
{
    Graphucks::Size canvasSize;

    GetAttributeFloat(L"Width", nullptr, canvasSize.width, pReader);
    GetAttributeFloat(L"Height", nullptr, canvasSize.height, pReader);
    
    canvas->SetSize(canvasSize);
}

void ColorConvert(const wchar_t* hexValueString, Graphucks::Color& color)
{
  wchar_t *p;
  const wchar_t *pHex = hexValueString + 1;

  unsigned long hexValue = wcstoul(pHex, nullptr, 16 );

  if(wcslen (pHex) == 6)
  {
      color.A = 1;
      color.R = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
      color.G = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
      color.B = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
      return;
  }

  color.A = ((hexValue >> 24) & 0xFF) / 255.0;
  color.R = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
  color.G = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  color.B = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
}

bool replacestring(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::wstring::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void ReadPath(const PathElementPtr& pathElement, const ParsingContext& parsingContext, const ComPtr<IXmlReader>& pReader)
{
    Graphucks::Color fill;
    Graphucks::Color stroke;
    float strokeThickness = 0;

    const wchar_t* colorString;

    if(GetAttributeString(L"Fill",  nullptr, &colorString, pReader))
    {
       
        if(wcswcs(colorString, L"Resource"))
        {
            wstring key = colorString;
            
            replacestring(key, L"{StaticResource ", L"");
            replacestring(key, L"}", L"");

            auto brush = parsingContext.brushResources.at(key);
            pathElement->FillBrush(brush);
        }
        else
        {
            ColorConvert(colorString, fill);
            pathElement->FillBrush(gmake_shared<Graphucks::SolidColorBrush>(fill));
        }
    }

    if(GetAttributeString(L"Stroke", nullptr,  &colorString, pReader))
    {
       if(wcswcs(colorString, L"Resource"))
        {
            wstring key = colorString;
            
            replacestring(key, L"{StaticResource ", L"");
            replacestring(key, L"}", L"");

            auto brush = parsingContext.brushResources.at(key);
            pathElement->StrokeBrush(brush);
        }
        else
        {
            ColorConvert(colorString, stroke);
            pathElement->StrokeBrush(gmake_shared<Graphucks::SolidColorBrush>(stroke));
        }
    }

    GetAttributeFloat(L"StrokeThickness", nullptr,  strokeThickness, pReader);

    pathElement->StrokeWidth(strokeThickness);
}

void ReadPathGeometry(const PathElementPtr& pathElement, const ComPtr<IXmlReader>& pReader)
{
    Graphucks::FillMode rule = FillMode::NonZero;

    const wchar_t* valueString;

    if(GetAttributeString(L"Figures", nullptr,  &valueString, pReader))
       pathElement->Geometry(PathElement::Parse(std::wstring(valueString)));
    if(GetAttributeString(L"FillRule", nullptr,  &valueString, pReader))
    {
        if(!wcscmp(valueString, L"NonZero") || !wcscmp(valueString, L"nonzero"))
            rule = FillMode::NonZero;
        else
            rule = FillMode::EvenOdd;
    }

    auto pathGeometry = dynamic_pointer_cast<PathGeometry>(pathElement->Geometry());

    pathGeometry->FillMode(rule);
}

Point ReadPoint(const wchar_t * dataString)
{
    wstring input = dataString;
    wistringstream ss(input);
    wstring token;
        
    float number;
    int count = 0;
    Point point;

    while(getline(ss, token, L',')) 
    {
        number = _wtof(token.c_str());
        if(!count)
            point.x = number;
        else
        {
            point.y = number;
            break;
        }
        count++;
    }

    return point;
}

void ReadLinearGradientBrush(const LinearGradientBrushPtr& brush, const ComPtr<IXmlReader>& reader)
{
     const wchar_t* valueString;
     if(GetAttributeString(L"StartPoint", nullptr,  &valueString, reader))
     {
        auto startPoint = ReadPoint(valueString);

        brush->Start(startPoint);
     }

     if(GetAttributeString(L"EndPoint", nullptr,  &valueString, reader))
     {
        auto endPoint = ReadPoint(valueString);

        brush->End(endPoint);
     }

     if(GetAttributeString(L"Key", L"http://schemas.microsoft.com/winfx/2006/xaml",  &valueString, reader))
     {
        wstring name = valueString;
        brush->Name(name);
     }
}

void ReadGradientStop(GradientStop& stop, const ComPtr<IXmlReader>& reader)
{
    const wchar_t* valueString;
    if(GetAttributeString(L"Color", nullptr,  &valueString, reader))
    {
        ColorConvert(valueString, stop.C);
    }

    float position = 0;
    if(GetAttributeFloat(L"Offset", nullptr,  position, reader))
    {
        stop.Position = position;
    }
}

void ReadMatrixTransform(MatrixTransformPtr& transform, const ComPtr<IXmlReader>& reader)
{
    const wchar_t* valueString;

    if(GetAttributeString(L"Matrix", nullptr, &valueString, reader))
    {
        transform = gmake_shared<MatrixTransform>();

        wstring input = valueString;
        wistringstream ss(input);
        wstring token;
        
        float number;
        int count = 0;
        Point point;

        while(getline(ss, token, L' ')) 
        {
            number = _wtof(token.c_str());
            
            switch (count)
            {
            case 0:
                transform->M11(number);
                break;
            case 1:
                transform->M12(number);
                break;
            case 2:
                transform->M21(number);
                break;
            case 3:
                transform->M22(number);
                break;
            case 4:
                transform->OffsetX(number);
                break;
            case 5:
                transform->OffsetY(number);
                break;
            default:
                break;
            }
            count++;
        }
    }
}

XamlVectorParser::XamlVectorParser(void)
{
}


XamlVectorParser::~XamlVectorParser(void)
{
}

Graphucks::VisualElementPtr XamlVectorParser::ParseXaml( Microsoft::WRL::ComPtr<IStream>& pFileStream )
{
   HRESULT hr;
   ComPtr<IXmlReader> pReader;
   XmlNodeType nodeType;
   const wchar_t* localName;
   const wchar_t* xmlValue;

   CanvasPtr mainContainer = gmake_shared<Canvas>();
   ParsingContext parsingContext;

   CanvasPtr                 currentCanvas = nullptr;
   PathElementPtr            currentPathElement;
   LinearGradientBrushPtr    currentlinearGradientBrush;
   GradientStopCollectionPtr currentGradientStops;
   
   if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL))) 
       return nullptr;

   if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
       return nullptr;

   if (FAILED(hr = pReader->SetInput(pFileStream.Get())))
       return nullptr;

   while (S_OK == (hr = pReader->Read(&nodeType))) 
   {
      switch (nodeType)
      {
      case XmlNodeType_XmlDeclaration:
         break;

      case XmlNodeType_Element:
         if (FAILED(hr = pReader->GetLocalName(&localName, nullptr))) 
            return nullptr;
        
                  OutputDebugString(L"+");
         OutputDebugString(localName);
         OutputDebugString(L"\n");


         if(!wcscmp(localName, L"Canvas"))
         {
            currentCanvas = gmake_shared<Canvas>();
            mainContainer->AddChild(currentCanvas);
            ReadCanvas(currentCanvas, pReader);
         }
         else if(!wcscmp(localName, L"Path"))
         {
             currentPathElement = gmake_shared<PathElement>();
             currentPathElement->SetStretch(Stretch::None);
             ReadPath(currentPathElement,parsingContext, pReader);
         }
         else if(!wcscmp(localName, L"PathGeometry"))
         {
             if(currentPathElement)
             {
                 ReadPathGeometry(currentPathElement, pReader);
                 currentCanvas->AddChild(currentPathElement);
                 currentPathElement = nullptr;
             }
         }
         else if(!wcscmp(localName, L"LinearGradientBrush"))
         {
             currentlinearGradientBrush = gmake_shared<LinearGradientBrush>();
             ReadLinearGradientBrush(currentlinearGradientBrush, pReader);
         }
         else if(!wcscmp(localName, L"GradientStopCollection"))
         {
             currentGradientStops = gmake_shared<GradientStopCollection>();
             currentlinearGradientBrush->GradientStopCollection(currentGradientStops);
         }
         else if(!wcscmp(localName, L"GradientStop"))
         {
            GradientStop stop;
             
            ReadGradientStop(stop, pReader);
            currentGradientStops->AddGradientStop(stop);
         }
         else if(!wcscmp(localName, L"MatrixTransform"))
         {
            MatrixTransformPtr transform;
             
            ReadMatrixTransform(transform, pReader);

            if(currentPathElement)
            currentPathElement->RenderTransform(transform);
         }
        
         //OutputDebugString(L"+");
         //OutputDebugString(pwszLocalName);
         //OutputDebugString(L"\n");

         break;

      case XmlNodeType_EndElement:
        
         if (FAILED(hr = pReader->GetLocalName(&localName, NULL))) 
            return nullptr;

         if(!wcscmp(localName, L"LinearGradientBrush"))
         {
            parsingContext.brushResources[currentlinearGradientBrush->Name()] = currentlinearGradientBrush;
         }
         //OutputDebugString(L"-");
         //OutputDebugString(pwszLocalName);
         //OutputDebugString(L"\n");
         break;
      }
   }

    return mainContainer;
}
