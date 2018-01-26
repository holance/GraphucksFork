#pragma once
#include <Graphucks.h>
class GraphucksDemo
{
public:
    GraphucksDemo(std::shared_ptr<Graphucks::Win32SceneRoot>& sceneRoot);
    virtual ~GraphucksDemo(void);
    void Update();
private:
    std::shared_ptr<Graphucks::CompositeTransform> m_myXform ;
    std::shared_ptr<Graphucks::ImageElement>       m_background ;
    std::shared_ptr<Graphucks::BlurEffect>         m_blurEffect;
    std::shared_ptr<Graphucks::RadialPanel>        radialPanel;
    float                                          m_counter;
};

