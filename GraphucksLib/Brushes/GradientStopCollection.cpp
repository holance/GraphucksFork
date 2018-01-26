#include "pch.h"
#include "GradientStopCollection.h"
#include "Utils.h"

using namespace Graphucks;
using namespace std;
using namespace Microsoft::WRL;

void GradientStopCollection::AddGradientStop(const GradientStop& gs)
{
    Invalidate();
    m_stops.push_back(gs);
}

ComPtr<IUnknown> GradientStopCollection::OnCreateResource(ID2D1DeviceContext* deviceContext)
{
   int noOfStops = int(m_stops.size());

    auto stops = unique_ptr<D2D1_GRADIENT_STOP[]>(new D2D1_GRADIENT_STOP[noOfStops]);

    for (int i = 0; i < noOfStops; ++i)
    {
        stops[i].color = m_stops[i].C;
        stops[i].position = m_stops[i].Position;
    }

    ID2D1GradientStopCollection* gsc;
    Check(deviceContext->CreateGradientStopCollection(stops.get(), 
                                                      noOfStops, 
                                                      D2D1_GAMMA_2_2, 
                                                      (D2D1_EXTEND_MODE)m_ExtendMode, 
                                                      &gsc));
    return gsc;
}

void GradientStopCollection::SetExtendMode(ExtendMode m)
{
    m_ExtendMode = m;
    Invalidate();
}

void GradientStopCollection::ClearGradientStops()
{
    m_stops.clear();
    Invalidate();
}
