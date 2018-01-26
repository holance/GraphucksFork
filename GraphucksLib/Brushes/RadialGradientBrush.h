#pragma once
#include "..\Core\GraphicsTypes.h"
#include "..\Brushes\BrushBase.h"
#include "..\Brushes\GradientStopCollection.h"
#include "..\Geometry\GeometryBase.h"

namespace Graphucks
{
    class RadialGradientBrush;
    typedef std::shared_ptr<RadialGradientBrush> RadialGradientBrushPtr;

    class RadialGradientBrush: public Brush
    {
    public:
        RadialGradientBrush(
            GradientStopCollectionPtr gsc, Point center, Point gradientOriginOffset, float radiusX, float radiusY)
                            : m_gradientStopCollection(gsc), 
                              m_center(center), 
                              m_gradientOriginOffset(gradientOriginOffset),
                              m_radiusX(radiusX), 
                              m_radiusY(radiusY){}

        virtual ~RadialGradientBrush(){}
    public:
        auto GradientStopCollection() const -> const GradientStopCollectionPtr&;

        Point Center() const;
        void Center(Point& val);

        Point GradientOriginOffset() const;
        void GradientOriginOffset(Point& val);

        float RadiusX() const;
        void RadiusX(float val);

        float RadiusY() const;
        void RadiusY(float val);
        
        virtual auto GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen) -> D2D1::Matrix3x2F override;
        virtual auto GetTransformToBounds(const Rectangle& rect) -> D2D1::Matrix3x2F override;
    protected:
        virtual auto IsResourceValid() const -> bool override;
        virtual auto OnCreateResource(ID2D1DeviceContext* deviceContext) -> Microsoft::WRL::ComPtr<IUnknown> override;
    private:
        Point                     m_center;
        Point                     m_gradientOriginOffset;
        float                     m_radiusX;
        float                     m_radiusY;
        GradientStopCollectionPtr m_gradientStopCollection;
    };
}
