#pragma once
#include "GeometryBase.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class RoundedRectangle;
    typedef std::shared_ptr<RoundedRectangle> RoundedRectanglePtr;

    class RoundedRectangle : public GeometryBase
    {
    public:
        RoundedRectangle(Graphucks::Rectangle rect, float rx, float ry): 
            m_rect(rect), 
            m_RX(rx), 
            m_RY(ry){}

        virtual ~RoundedRectangle(){}
    public:
        auto RadiusX() const -> float;
        void RadiusX(float rx);

        auto RadiusY() const -> float;
        void RadiusY(float ry);

        auto Rect() const -> Graphucks::Rectangle;
        void Rect(Rectangle rect);
    protected:
        virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
    private:
        Graphucks::Rectangle m_rect;
        float                m_RX;
        float                m_RY;
    };
}

