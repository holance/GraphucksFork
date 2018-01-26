#include "pch.h"
#include "DropShadowEffect.h"

using namespace Graphucks;
using namespace std;

DropShadowEffect::DropShadowEffect(void)
{
    m_shadowEffect = make_shared<EffectResource>(CLSID_D2D1Shadow);
    m_affineEffect = make_shared<EffectResource>(CLSID_D2D12DAffineTransform);
    m_compositeEffect = make_shared<EffectResource>(CLSID_D2D1Composite);

    auto matrix = D2D1::Matrix3x2F::Translation(0, 0);

    m_shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, m_blurSize);
    m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix);
   

    SetEffect(m_compositeEffect);
}

DropShadowEffect::~DropShadowEffect(void)
{
}

void DropShadowEffect::SetInput(const ImagePtr& image)
{
    if(image == m_inputImage)
        return;

    m_inputImage = image;

    m_shadowEffect->SetInput(0, m_inputImage);
    m_affineEffect->SetInput(0, m_shadowEffect);
    m_compositeEffect->SetInput(0, m_affineEffect);
    m_compositeEffect->SetInput(1, m_inputImage);
}

float DropShadowEffect::BlurSize() const
{
    return m_blurSize;
}

void DropShadowEffect::BlurSize(float blurSize)
{
    m_blurSize = blurSize;
    m_shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, m_blurSize);
    NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
}

void DropShadowEffect::ShadowTransform( const TransformPtr& transform )
{
    m_transform = transform;
    m_affineEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, m_transform->GetTransformMatrix());

    NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
}

void Graphucks::DropShadowEffect::ShadowColor( const Graphucks::Color& color )
{
    m_shadowColor = color;

    auto colorVector = D2D1::Vector4F(color.R, color.G, color.B, color.A);
    m_shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, colorVector);
    NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
}
