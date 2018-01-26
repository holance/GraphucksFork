#pragma once
#include "GeometryBase.h"

namespace Graphucks
{
    class CombinedGeometry: public GeometryBase
    {
    public:
        CombinedGeometry(GeometryPtr g1, GeometryPtr g2, D2D1_COMBINE_MODE mode)
                         : m_Geometry1(g1), m_Geometry2(g2), m_Mode(mode){}
        virtual ~CombinedGeometry(){}
    protected:
       virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
    private:
        GeometryPtr       m_Geometry1;
        GeometryPtr       m_Geometry2;
        D2D1_COMBINE_MODE m_Mode;
    };
}