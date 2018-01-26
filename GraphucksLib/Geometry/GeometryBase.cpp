#include "pch.h"
#include "GeometryBase.h"
#include "Utils.h"
#include "RenderContext.h"

using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace D2D1;


GeometryBase::GeometryBase() : m_geometryCacheEnabled(false), m_transform(D2D1::Matrix3x2F::Identity())
{
    m_Factory = RenderContext::GetFactory();
}

bool GeometryBase::HitTest(const Point& point, const Matrix3x2F& matrix)
{
    BOOL result = false;
    GetResource<ID2D1Geometry>()->FillContainsPoint(point, 
                                                    matrix, &result);

    return result != 0;
}

bool GeometryBase::HitTestStroke(const Point& point, const PenPtr& pen, const Matrix3x2F& matrix)
{
    ID2D1StrokeStyle* strokeStyle = nullptr;
    BOOL result = false;
    if(pen->StrokeStyle())
        strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(pen->StrokeStyle());

    GetResource<ID2D1Geometry>()->StrokeContainsPoint(point, 
                                                      pen->Thickness(), 
                                                      strokeStyle, 
                                                      matrix, 
                                                      &result);

    return result != 0;
}

auto GeometryBase::GetFillMask(ID2D1RenderTarget* rt) -> ID2D1BitmapPtr
{
    if(!m_geometryCacheEnabled)
    {
        return nullptr;
    }

    if (!m_FillMask)
        m_FillMask = CreateFillMask(rt);

    return (m_FillMask ? m_FillMask->Mask : ID2D1BitmapPtr());
}


auto GeometryBase::GetStrokeMask(ID2D1RenderTarget* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle) -> ID2D1BitmapPtr
{
    if(!m_geometryCacheEnabled)
    {
        return nullptr;
    }

    if (m_StrokeMasks.find(strokeWidth) == m_StrokeMasks.end())
    {
        MaskDataPtr mask = nullptr;
            
        mask = CreateStrokeMask(rt, strokeWidth, strokeStyle);
           
        m_StrokeMasks[strokeWidth] = mask;
    }
    return (m_StrokeMasks[strokeWidth] ? m_StrokeMasks[strokeWidth]->Mask : ID2D1BitmapPtr());
}

auto GeometryBase::GetStrokeMaskTransform(float strokeWidth) -> const D2D1_MATRIX_3X2_F
{
    if (m_StrokeMasks.find(strokeWidth) != m_StrokeMasks.end())
    {
        return m_StrokeMasks[strokeWidth]->Transform;
    }
    else
    {
        return Matrix3x2F::Identity();
    }
}

auto GeometryBase::CreateFillMask(ID2D1RenderTarget* rt) -> MaskDataPtr 
{
    auto geometry = GetResource<ID2D1Geometry>();

    D2D1_RECT_F boundingBox;
    geometry->GetBounds(IdentityMatrix(), &boundingBox);

    if (boundingBox.left > boundingBox.right)
    {
        // empty bounds
        return MaskDataPtr();
    }

    //we grow the rectangle by one unit on all sizes 
    //so that the mask alpha smoothly falls off to 0 at boundaries
    GrowRectangle(boundingBox, 1);

    auto size = D2D1::SizeF(boundingBox.right - boundingBox.left, 
                            boundingBox.bottom - boundingBox.top);

    if ((size.width > m_MaskUsageThreshold) || (size.height > m_MaskUsageThreshold))
    {
        // too big to use bitmap masks
        return MaskDataPtr();
    }

    // create intermediate render target. We only set a desired dip size, so our render target
    // actually may contain more pixels in high dpi.
    auto pf = D2D1::PixelFormat(DXGI_FORMAT_A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    ComPtr<ID2D1BitmapRenderTarget> intermediate;

    Check(rt->CreateCompatibleRenderTarget(&size, 
                                           NULL/*desired pixel size*/, 
                                           &pf,
                                           D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, 
                                           &intermediate));

    // create opaque brush
    ComPtr<ID2D1SolidColorBrush> opaqueBrush;
    Check(intermediate->CreateSolidColorBrush(ColorF(0, 0, 0, 1), &opaqueBrush));

    // render to intermediate
    intermediate->BeginDraw();
    intermediate->Clear(ColorF(0, 0, 0, 0));

    // move geometry inside the render target
    intermediate->SetTransform(Matrix3x2F::Translation(-boundingBox.left,
                                                       -boundingBox.top));

    auto fillMaskTransform = Matrix3x2F::Translation(boundingBox.left, boundingBox.top);

    // render
    intermediate->FillGeometry(GetResource<ID2D1Geometry>(), opaqueBrush.Get());

    Check(intermediate->EndDraw());

    ComPtr<ID2D1Bitmap> mask;
    Check(intermediate->GetBitmap(&mask));

    return MaskDataPtr(new MaskData(mask, fillMaskTransform));
}

auto GeometryBase::CreateStrokeMask(ID2D1RenderTarget* rt, float strokeWidth, ID2D1StrokeStyle* strokeStyle) -> MaskDataPtr
{
    auto geometry = GetResource<ID2D1Geometry>();

    D2D1_RECT_F boundingBox;
    geometry->GetWidenedBounds(strokeWidth, 
                               strokeStyle, 
                               nullptr/*transform*/, 
                               &boundingBox);

    if (boundingBox.left > boundingBox.right)
    {
        // empty bounds
        return MaskDataPtr();
    }

    //we grow the rectangle by one unit on all sizes 
    //so that the mask alpha smoothly falls off to 0 at boundaries
    GrowRectangle(boundingBox, 1);

    auto size = SizeF(boundingBox.right - boundingBox.left, 
                      boundingBox.bottom - boundingBox.top);

    if ((size.width > m_MaskUsageThreshold) || 
        (size.height > m_MaskUsageThreshold))
    {
        // too big to use bitmap masks
        return MaskDataPtr();
    }

    // create intermediate render target and brush
    auto pf = D2D1::PixelFormat(DXGI_FORMAT_A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
    ComPtr<ID2D1BitmapRenderTarget> intermediate;
    Check(rt->CreateCompatibleRenderTarget(&size, 
                                           NULL/*desired pixel size*/, 
                                           &pf,
                                           D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, 
                                           &intermediate));
        

    // create opaque brush
    ComPtr<ID2D1SolidColorBrush> opaqueBrush;
    Check(intermediate->CreateSolidColorBrush(ColorF(0, 0, 0, 1), &opaqueBrush));
        

    // render to intermediate
    intermediate->BeginDraw();
    intermediate->Clear(D2D1::ColorF(0, 0, 0, 0));

    // move geometry inside the render target
    intermediate->SetTransform(Matrix3x2F::Translation(-boundingBox.left, -boundingBox.top));
    auto strokeMaskTransform = Matrix3x2F::Translation(boundingBox.left, boundingBox.top);

    // render
    intermediate->DrawGeometry(geometry, opaqueBrush.Get(), strokeWidth);

    Check(intermediate->EndDraw());

    ComPtr<ID2D1Bitmap> mask;
    Check(intermediate->GetBitmap(&mask));

    return MaskDataPtr(new MaskData(mask, strokeMaskTransform));
}

auto GeometryBase::GetBounds(const PenPtr& pen, const Matrix3x2F& worldTransform) -> Graphucks::Rectangle
{
    auto geometry = GetResource<ID2D1Geometry>();
    D2D1_RECT_F bounds;

    if(pen)
    {
        ID2D1StrokeStyle* strokeStyle = nullptr;

        if(pen->StrokeStyle())
            strokeStyle = GetResourceFrom<ID2D1StrokeStyle>(pen->StrokeStyle());

        geometry->GetWidenedBounds(pen->Thickness(), 
                                   strokeStyle, 
                                   worldTransform, 
                                   &bounds);

        GrowRectangle(bounds, pen->Thickness()/2);
    }    
    else
        geometry->GetBounds(worldTransform, &bounds);
        

    return Graphucks::Rectangle(bounds.left, bounds.top , bounds.right, bounds.bottom);
}

auto GeometryBase::GetFillMaskTransform() const -> const D2D1_MATRIX_3X2_F&
{
    return m_FillMask->Transform;
}

void GeometryBase::GeometryCacheEnabled( bool enabled )
{
    m_geometryCacheEnabled = enabled;
    if(!enabled)
    {
        m_FillMask = nullptr;
        m_StrokeMasks.clear();
    }
}

bool GeometryBase::GeometryCacheEnabled()
{
    return m_geometryCacheEnabled;
}

Microsoft::WRL::ComPtr<IUnknown> GeometryBase::OnCreateResource()
{
    auto geometry = OnCreateGeometry();

    if(!m_transform.IsIdentity())
    {
        ComPtr<ID2D1TransformedGeometry> transformedGeometry;
        //auto matrix = m_transform->GetTransformMatrix();

        HRESULT hr = m_Factory->CreateTransformedGeometry(geometry.Get(), 
                                                          &m_transform, 
                                                          transformedGeometry.ReleaseAndGetAddressOf());
        return transformedGeometry;
    }
    else
    {
        return geometry;
    }
}

void GeometryBase::Transform( const TransformPtr& transform )
{
    /*if(m_transform != nullptr && transform != nullptr)
    {
        auto m1 = m_transform->GetTransformMatrix();
        auto m2 = transform->GetTransformMatrix();

        if(m1._11 == m2._11 && 
            m1._12 == m2._12 && 
            m1._21 == m2._21 && 
            m1._22 == m2._22 && 
            m1._31 == m2._31 && 
            m1._32 == m2._32)
            return;

        if(m2.IsIdentity())
            return;
    }*/
    m_transform = m_transform * transform->GetTransformMatrix();
    Invalidate();
}
