#include "pch.h"
#include "StrokeCommand.h"

namespace Graphucks
{
    void StrokeCommand::Run()
    {
        if (!IsResourceValidFrom(m_Geometry))
        {
            PerPrimitiveAliasingScope perPrimitive(m_RT);
            auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
            auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);

            m_RT->DrawGeometry(geometry, brush, m_pen->Thickness());
        }
        else
        {
            auto thickness = m_pen->Thickness();

            ID2D1StrokeStyle* strokeStyle = nullptr;

            if(m_pen->StrokeStyle())
                strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(m_pen->StrokeStyle());

            /* If geometry is small enough there can be a stroke mask */
            ID2D1BitmapPtr mask = m_Geometry->GetStrokeMask(m_RT, thickness, strokeStyle);

            if (mask)
            {
                AliasModeScope aliasScope(m_RT);
                TemporaryTransformScope tempTransform(m_RT, m_Brush, m_Geometry->GetStrokeMaskTransform(thickness));

                
                auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
                m_RT->FillOpacityMask(mask.Get(), brush, D2D1_OPACITY_MASK_CONTENT_GRAPHICS);
            }
            else
            {
                /* No mask for the geometry */
                PerPrimitiveAliasingScope perPrimitive(m_RT);
                
                auto geometry = GetResourceFrom<ID2D1Geometry>(m_Geometry);
                auto brush = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
                ID2D1StrokeStyle* strokeStyle = nullptr;

                if(m_pen->StrokeStyle())
                {
                    strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(m_pen->StrokeStyle());
                }
             
                m_RT->DrawGeometry(geometry, brush, thickness, strokeStyle);
            }
        }
    }

    void StrokeCommand::SetPen(const PenPtr& pen)
    {
        m_pen = pen;
    }

}
