#include "pch.h"
#include "SolidColorBrush.h"
#include "Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;

auto SolidColorBrush::OnCreateResource(ID2D1DeviceContext* deviceContext) -> ComPtr<IUnknown> 
{
    ComPtr<ID2D1SolidColorBrush> solidColorBrush;
    Check(deviceContext->CreateSolidColorBrush(m_Color, D2D1::BrushProperties(m_Opacity), &solidColorBrush));
    solidColorBrush->SetTransform(m_transform);
    return solidColorBrush;
}

 void SolidColorBrush::FillColor(const Graphucks::Color& c)
 {
     m_Color = c;
     Invalidate();
 }

auto SolidColorBrush::FillColor() const -> Color
 {
     return m_Color;
 }
