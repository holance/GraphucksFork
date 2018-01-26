#pragma once
#include <Graphucks.h>

class FauxLights : public Graphucks::VisualElement
{
public:
    FauxLights(void);
    virtual ~FauxLights(void);
protected:
    virtual void Rendering() override;
private:
    Graphucks::CompositeTransformPtr m_transform;
    float m_rotation;
    Graphucks::RadialPanelPtr m_panel;
};

