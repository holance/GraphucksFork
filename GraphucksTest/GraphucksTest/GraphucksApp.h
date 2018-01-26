#pragma once
#include <Graphucks.h>

class GraphucksApp : public Graphucks::VisualElement
{
public:
    GraphucksApp(void);
    virtual ~GraphucksApp(void);
protected:
    virtual void Loaded()   override;
    virtual void Unloaded() override;
protected:
    virtual void Rendering() override;
private:
    Graphucks::MultiContentPanelElementPtr m_mainPanel;
    Graphucks::EventToken                  m_clickedEventToken;
    Graphucks::CompositeTransformPtr       m_backgroundTransform;
    Graphucks::SaturationEffectPtr         m_backgroundEffect;
    std::shared_ptr<Graphucks::MediaElement>          m_mediaElement;
    float                                  m_scaleAnimValue;
};

