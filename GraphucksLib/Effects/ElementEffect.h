#pragma once
#include "EffectResource.h"
#include "..\Core\GraphicsTypes.h"
#include "..\Core\Resource.h"

namespace Graphucks
{
    class ElementEffect;
    typedef std::shared_ptr<ElementEffect> ElementEffectPtr;

    class RenderContext;
    class DrawEffectCommand;

    class ElementEffect : public DependantObject
    {
    public:
        ElementEffect();
        virtual ~ElementEffect();
       

    protected:
        EffectResourcePtr GetEffectResource();
      
        void SetEffect(const EffectResourcePtr& effect);

    private:
        virtual void SetInput(const ImagePtr& image) = 0;
        Microsoft::WRL::ComPtr<ID2D1Image> GetOutputImage(ID2D1DeviceContext* deviceContext);
        ID2D1ImageBrush* GetOutputImageBrush(ID2D1DeviceContext* deviceContext);

    private:
        Microsoft::WRL::ComPtr<ID2D1ImageBrush> m_imageBrush;
        EffectResourcePtr m_effect;
        friend RenderContext;
        friend DrawEffectCommand;
    };
}
 
