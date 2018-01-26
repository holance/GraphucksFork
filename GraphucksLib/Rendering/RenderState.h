#pragma once
#include "..\Core\GraphicsTypes.h"
#include "..\Media\Bitmap.h"

#include <memory>
#include <d2d1_1.h>
#include <wrl.h>
#include <vector>

namespace Graphucks
{
    enum class ClipStateType
    {
        AxisAlignedClip,
        Layer
    };

    struct ClipState
    {
        Graphucks::Rectangle clipArea;
        std::shared_ptr<ID2D1Layer> layer;
        ClipStateType clipType;
        D2D1::Matrix3x2F transform;

        ClipState(std::shared_ptr<ID2D1Layer>& layer, const D2D1::Matrix3x2F& currentTransform)
        {
            transform = currentTransform;
            this->layer = layer;
            clipType = ClipStateType::Layer;
        }

        ClipState(const Graphucks::Rectangle& clipAreaRect, const D2D1::Matrix3x2F& currentTransform)
        {
            clipArea = clipAreaRect;
            transform = currentTransform;
            clipType = ClipStateType::AxisAlignedClip;
        }

        void PushClip(Microsoft::WRL::ComPtr<ID2D1DeviceContext>& deviceContext)
        {
            deviceContext->SetTransform(transform);
            if(clipType == ClipStateType::AxisAlignedClip)
            {
                deviceContext->PushAxisAlignedClip(clipArea, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            }
            else
            {
                deviceContext->PopLayer();
            }
        }

        void PopClip(Microsoft::WRL::ComPtr<ID2D1DeviceContext>& deviceContext)
        {
            if(!layer)
                deviceContext->PopAxisAlignedClip();
            else
            {

            }
        }
    };

    struct RenderState
    {
        RenderState()
        {
        }

        RenderState(const BitmapPtr& rt) : renderTarget(rt)
        {

        }
        BitmapPtr              renderTarget;
        std::vector<ClipState> clipQueue;
    };
}