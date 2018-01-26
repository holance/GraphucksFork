#pragma once
#include "GeometryBase.h"
#include "..\Core\Transform.h"

namespace Graphucks
{

    class TransformedGeometry : public GeometryBase
    {
    public:
        TransformedGeometry(const GeometryPtr& geometry, const TransformPtr& transform);
        virtual ~TransformedGeometry(void);
    protected:
       virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
    private:
        GeometryPtr m_originalGeometry;
        TransformPtr m_transform;
    };
}
