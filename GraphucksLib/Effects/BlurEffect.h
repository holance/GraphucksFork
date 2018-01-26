#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"
#include "..\Core\Transform.h"

#include "EffectResource.h"
#include "ElementEffect.h"

namespace Graphucks
{
    class BlurEffect;
    typedef std::shared_ptr<BlurEffect> BlurEffectPtr;

    class BlurEffect : public ElementEffect
    {
    public:
        BlurEffect(void);
        virtual ~BlurEffect(void);
    public:
        virtual void SetInput(const ImagePtr& image) override;
    public:
        auto BlurSize() const -> float;
        void BlurSize(float blurSize);
    private:
        EffectResourcePtr m_blurEffect;
        ImagePtr          m_inputImage;
        float             m_blurSize;
    };
}
