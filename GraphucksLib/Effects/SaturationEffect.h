#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"
#include "..\Core\Transform.h"

#include "EffectResource.h"
#include "ElementEffect.h"

namespace Graphucks
{
    class SaturationEffect;
    typedef std::shared_ptr<SaturationEffect> SaturationEffectPtr;

    class SaturationEffect : public ElementEffect
    {
    public:
        SaturationEffect(void);
        virtual ~SaturationEffect(void);
    public:
        virtual void SetInput(const ImagePtr& image) override;
    public:
        auto SaturationAmount() const -> float;
        void SaturationAmount(float blurSize);
    private:
        EffectResourcePtr m_saturationEffect;
        ImagePtr          m_inputImage;
        float             m_saturationAmount;
    };
}
