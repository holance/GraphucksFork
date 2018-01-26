#pragma once
#include "..\Brushes\BrushBase.h"
#include "..\Brushes\Pen.h"
#include "..\Geometry\GeometryBase.h"

namespace Graphucks
{
    class BrushTransformToBounds
    {
    public:
        void Setup(Graphucks::Brush* brush, const Graphucks::GeometryPtr& geometry, const Graphucks::PenPtr& pen) 
        {
            m_brush = brush;

            m_originalTransform = m_brush->Transform();

            auto boundsTransform = m_brush->GetTransformToGeometryBounds(geometry, pen);

            boundsTransform = boundsTransform * m_originalTransform;

            m_brush->Transform(boundsTransform);
        }

        BrushTransformToBounds() : m_brush(nullptr)
        {

        }

        ~BrushTransformToBounds()
        {
            if(m_brush)
                m_brush->Transform(m_originalTransform);
        }
    private:
        Graphucks::Brush* m_brush;
        D2D1::Matrix3x2F      m_originalTransform;
    };
}