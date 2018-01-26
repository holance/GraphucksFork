#include "pch.h"
#include "SaturationEffect.h"

using namespace Graphucks;
using namespace std;

SaturationEffect::SaturationEffect(void) : m_saturationAmount(0.5f)
{
    m_saturationEffect = make_shared<EffectResource>(CLSID_D2D1Saturation);

    SetEffect(m_saturationEffect);
}

SaturationEffect::~SaturationEffect(void)
{
}

void SaturationEffect::SetInput(const ImagePtr& image)
{
    if(image == m_inputImage)
        return;

    m_inputImage = image;

    m_saturationEffect->SetInput(0, m_inputImage);
}

float SaturationEffect::SaturationAmount() const
{
    return m_saturationAmount;
}

void SaturationEffect::SaturationAmount(float saturationAmount)
{
    if(saturationAmount == m_saturationAmount)
        return;

    m_saturationAmount = saturationAmount;
    m_saturationEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, m_saturationAmount);
    NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
}
