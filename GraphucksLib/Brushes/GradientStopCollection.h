#pragma once
#include <memory>
#include <d2d1.h>
#include <vector>

#include "..\Core\Resource.h"
#include "..\Brushes\GradientStop.h"

namespace Graphucks
{
    class GradientStopCollection;
    typedef std::shared_ptr < GradientStopCollection > GradientStopCollectionPtr;

    class GradientStopCollection: public DeviceDependantResource
    {
    public:
        GradientStopCollection(): m_ExtendMode(ExtendMode::Clamp){}
        virtual ~GradientStopCollection(){}
    public:
        void AddGradientStop(const GradientStop& gs);
        void SetExtendMode(ExtendMode m);
        void ClearGradientStops();
    protected:
         virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext) -> Microsoft::WRL::ComPtr<IUnknown>;
    private:
        std::vector<GradientStop> m_stops;
        ExtendMode                m_ExtendMode;
    };
}
