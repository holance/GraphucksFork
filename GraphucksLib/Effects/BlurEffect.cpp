#include "pch.h"
#include "BlurEffect.h"

using namespace Graphucks;
using namespace std;

BlurEffect::BlurEffect(void)
{
    m_blurEffect = make_shared<EffectResource>(CLSID_D2D1GaussianBlur);

    SetEffect(m_blurEffect);
}

BlurEffect::~BlurEffect(void)
{
}

void BlurEffect::SetInput(const ImagePtr& image)
{
    if(image == m_inputImage)
        return;

    m_inputImage = image;

    m_blurEffect->SetInput(0, m_inputImage);
}

float BlurEffect::BlurSize() const
{
    return m_blurSize;
}

void BlurEffect::BlurSize(float blurSize)
{
    if(m_blurSize == blurSize)
        return;

    m_blurSize = blurSize;
    m_blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, m_blurSize);
    NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
}
