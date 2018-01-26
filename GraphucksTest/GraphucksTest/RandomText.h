#pragma once
#include <Graphucks.h>

class RandomText : public Graphucks::VisualElement
{
public:
    RandomText(void);
    virtual ~RandomText(void);
protected:
    virtual void Rendering() override;
    virtual void Loaded() override;
    virtual void Unloaded() override;
private:
    Graphucks::StackPanelPtr m_stackPanel;
    Graphucks::CompositeTransformPtr m_textTransform;
    Graphucks::CompositeTransformPtr m_textTransform2;
    float                            m_animationVal;
    int                              m_testVal;
    Graphucks::TextBlockElementPtr   m_clickableTextblock;
    Graphucks::EventToken            m_clickToken;
};

