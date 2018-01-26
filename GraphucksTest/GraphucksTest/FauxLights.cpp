#include "pch.h"
#include "FauxLights.h"
#include <vector>
#include "RandomText.h"

using namespace std;
using namespace Graphucks;

FauxLights::FauxLights(void) : m_rotation(0)
{
    m_panel = make_shared<RadialPanel>();
    m_panel->RadiusDivisor(2.8f);
    m_transform = make_shared<CompositeTransform>();
    m_panel->RenderTransform(m_transform);

    AddVisualChild(m_panel);

    const int LIGHT_COUNT = 3;
    vector<RadialGradientBrushPtr> brushes;

    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        auto gsc = make_shared<GradientStopCollection>();
        
        switch (i)
        {
        case 0:
            gsc->AddGradientStop(GradientStop(0, Color(0.2f, 0.5f, 1.0f, 0.6f)));
        	break;
        case 1:
            gsc->AddGradientStop(GradientStop(0, Color(0.9f, 0.5f, 0.0f, 0.6f)));
        	break;
        case 2:
            gsc->AddGradientStop(GradientStop(0, Color(0.5f, 0.0f, 1.0f, 0.6f)));
        	break;
        default:
            gsc->AddGradientStop(GradientStop(0, Color(0.9f, 0.5f, 0.0f, 0.6f)));
        	break;
        }
        
        gsc->AddGradientStop(GradientStop(1, Color(0.0f, 0.0f, 0.0f, 0)));

        brushes.emplace_back(make_shared<RadialGradientBrush>(gsc, 
                                                              Point(0.5f, 0.5f), 
                                                              Point(0.0f, 0.0f), 
                                                              0.5f, 
                                                              0.5f));
    }

    for (int i = 0; i < LIGHT_COUNT; i++)
    {
        auto ellipseElement = make_shared<EllipseElement>();
        ellipseElement->SetSize(Size(1200, 1200));
        auto gsc = make_shared<GradientStopCollection>();
       
        ellipseElement->FillBrush(brushes[i]);
        ellipseElement->GeometryCacheEnabled(true);
        m_panel->AddChild(ellipseElement);
    }

    auto randomText = make_shared<RandomText>();

    AddVisualChild(randomText);
}


FauxLights::~FauxLights(void)
{
}

void FauxLights::Rendering()
{
    m_rotation += 0.5f;
    m_transform->Rotation(m_rotation);
}
