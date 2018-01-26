#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"
#include "..\Core\Transform.h"

#include "EffectResource.h"
#include "ElementEffect.h"

namespace Graphucks
{
    class DropShadowEffect;
    typedef std::shared_ptr<DropShadowEffect> DropShadowEffectPtr;

    class DropShadowEffect : public ElementEffect
    {
    public:
        DropShadowEffect(void);
        virtual ~DropShadowEffect(void);
    public:
        virtual void SetInput(const ImagePtr& image) override;
    public:
        auto BlurSize() const -> float;
        void BlurSize(float blurSize);

        Graphucks::Color ShadowColor();
        void ShadowColor(const Graphucks::Color& color);
        void ShadowTransform(const TransformPtr& transform);
    private:
        EffectResourcePtr m_shadowEffect;
        EffectResourcePtr m_affineEffect;
        EffectResourcePtr m_compositeEffect;
        ImagePtr          m_inputImage;
        float             m_blurSize;
        TransformPtr      m_transform;
        Graphucks::Color  m_shadowColor;
    };
}
