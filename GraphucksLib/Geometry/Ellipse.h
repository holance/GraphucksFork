#pragma once
#include "..\Geometry\GeometryBase.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class Ellipse;
    typedef std::shared_ptr<Ellipse> EllipsePtr;

    class Ellipse: public GeometryBase
    {
    public:
        Ellipse(Point center, float rx, float ry): m_Center(center), m_RX(rx), m_RY(ry){}
        virtual ~Ellipse(){}
    public:
        auto RadiusX() const -> float;
        void RadiusX(float rx);

        auto RadiusY() const -> float;
        void RadiusY(float ry);

        auto Center() const -> Graphucks::Point;
        void Center(const Graphucks::Point& center);
    protected:
        virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
    private:
        Graphucks::Point m_Center;
        float            m_RX;
        float            m_RY;
    };

}
