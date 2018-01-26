#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    class Brush;
    typedef std::shared_ptr<Brush> BrushPtr;

    class GeometryBase;
    class Pen;

    class Brush : public DeviceDependantResource
    {
    public:
        Brush();
        virtual ~Brush(){}
    public:
        void Opacity(const float o);
        auto Opacity() const -> float;
    public:
        void Transform(const D2D1::Matrix3x2F& transform);
        auto Transform() const -> const D2D1::Matrix3x2F&;
    public:
        virtual auto GetTransformToGeometryBounds(const std::shared_ptr<GeometryBase>& geometry, 
                                                  const std::shared_ptr<Pen>& pen) -> D2D1::Matrix3x2F;

        virtual auto GetTransformToBounds(const Graphucks::Rectangle& rect) -> D2D1::Matrix3x2F;
    protected:
        float            m_Opacity;
        D2D1::Matrix3x2F m_transform;
    };
}