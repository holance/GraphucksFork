#pragma once
#include "ExecutableBase.h"
#include "..\Media\Bitmap.h"

namespace Graphucks
{
    class DrawImageCommand : ExecutableBase
    {
        typedef Microsoft::WRL::ComPtr<ID2D1DeviceContext>  ID2D1DeviceContextPtr;
    public:
        DrawImageCommand();

        virtual ~DrawImageCommand(void);
    public:
        Graphucks::Bitmap* Bitmap() const { return m_bitmap; }
        void Bitmap(Graphucks::Bitmap * const val) { m_bitmap = val; }

        EffectCompositeMode CompositeMode() const { return m_compositeMode; }
        void CompositeMode(const EffectCompositeMode val) { m_compositeMode = val; }

        Graphucks::Size TargetSize() const { return m_targetSize; }
        void TargetSize(const Graphucks::Size& val) { m_targetSize = val; }

        ID2D1DeviceContext* DeviceContext() const { return m_deviceContext; }
        void DeviceContext(ID2D1DeviceContext* const val) { m_deviceContext = val; }
    public:
        virtual void Run();
    private:
        Graphucks::Bitmap*     m_bitmap; 
        EffectCompositeMode    m_compositeMode; 
        Graphucks::Size        m_targetSize;
        ID2D1DeviceContext*    m_deviceContext;
    };
}
