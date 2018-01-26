#pragma once
#include "..\Brushes\BrushBase.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class SolidColorBrush;
    typedef std::shared_ptr<SolidColorBrush> SolidColorBrushPtr;

    class SolidColorBrush: public Brush
    {
    public:
        explicit SolidColorBrush(const Graphucks::Color& c): m_Color(c){}
        virtual ~SolidColorBrush(){}
    public:
        void FillColor(const Graphucks::Color& c);
        auto FillColor() const -> Graphucks::Color;
    private:
        virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext) -> Microsoft::WRL::ComPtr<IUnknown> override;
    private:
        Graphucks::Color m_Color;
    };
}