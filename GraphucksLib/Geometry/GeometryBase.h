#pragma once
#include "..\Core\Resource.h"
#include "..\Core\GraphicsTypes.h"
#include "..\Brushes\Pen.h"
#include <map>
#include "..\Core\CompositeTransform.h"

namespace Graphucks
{
    typedef Microsoft::WRL::ComPtr<ID2D1Bitmap>       ID2D1BitmapPtr;
    typedef Microsoft::WRL::ComPtr<ID2D1GeometrySink> ID2D1GeometrySinkPtr;
    typedef Microsoft::WRL::ComPtr<ID2D1Factory>      ID2D1FactoryPtr;

	class Figure;
    typedef std::shared_ptr<Figure>       FigurePtr;

    class ISegment;
    typedef std::shared_ptr<ISegment>     ISegmentPtr;

	class GeometryBase;
    typedef std::shared_ptr<GeometryBase> GeometryPtr;

    struct MaskData;
    typedef std::shared_ptr<MaskData>     MaskDataPtr;

    class GeometryBase: public IndependantResource 
    {
    public:
        typedef std::map<float, MaskDataPtr> MaskDataPtrMap;

        GeometryBase();
        virtual ~GeometryBase(){}
    public:
        auto GetFillMask(ID2D1RenderTarget* rt) -> ID2D1BitmapPtr;
        auto GetFillMaskTransform()       const -> const D2D1_MATRIX_3X2_F&;

        auto GetStrokeMask(ID2D1RenderTarget* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle) -> ID2D1BitmapPtr;
        auto GetStrokeMaskTransform(float strokeWidth) -> const D2D1_MATRIX_3X2_F;

        bool HitTest(const Point& point, const D2D1::Matrix3x2F& matrix = D2D1::Matrix3x2F::Identity());
        bool HitTestStroke(const Point& point, const PenPtr& pen, const D2D1::Matrix3x2F& matrix = D2D1::Matrix3x2F::Identity());

        auto GetBounds(const PenPtr& pen, const D2D1::Matrix3x2F& worldTransform) -> Graphucks::Rectangle;

        void GeometryCacheEnabled(bool enabled);
        bool GeometryCacheEnabled();

        void Transform(const TransformPtr& transform);

    protected:
        ID2D1FactoryPtr m_Factory;
    protected:
        virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource() final;
        virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() = 0;
    private:
        auto CreateFillMask(ID2D1RenderTarget* rt) -> MaskDataPtr;
        auto CreateStrokeMask(ID2D1RenderTarget* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle) -> MaskDataPtr;
    private:
        mutable MaskDataPtr    m_FillMask;
        mutable MaskDataPtrMap m_StrokeMasks;
        static const int       m_MaskUsageThreshold = 1000;
        bool                   m_geometryCacheEnabled;
        friend RenderContext;
        D2D1::Matrix3x2F m_transform;
    };
    
    struct MaskData
    {
        MaskData(ID2D1BitmapPtr mask, D2D1_MATRIX_3X2_F transform): Mask(mask), 
                                                                    Transform(transform){}
        ID2D1BitmapPtr Mask;
        D2D1_MATRIX_3X2_F Transform;
    private:
        // non copyable
        MaskData(const MaskData&);
        MaskData& operator = (const MaskData&);
    };
}