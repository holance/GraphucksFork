#include "pch.h"
#include <memory>

#include "Utils.h"
#include "ImageBrush.h"
#include <d2d1_1.h>
#include "Bitmap.h"

using namespace Graphucks;
using namespace D2D1;
using namespace Microsoft::WRL;
using namespace std;

ImageBrush::ImageBrush(const ImagePtr& image) : m_image(image), 
                                                m_extentModeX(ExtendMode::Clamp), 
                                                m_extentModeY(ExtendMode::Clamp), 
                                                m_sourceRect(0,0,0,0),
                                                m_interpolationMode(InterpolationMode::Linear)
{
    auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

    if(bitmap)
    {
        auto size = bitmap->GetSize();
        m_sourceRect.bottom = size.height;
        m_sourceRect.right = size.width;
    }
}

ImageBrush::ImageBrush() : 
                            m_extentModeX(ExtendMode::Clamp), 
                            m_extentModeY(ExtendMode::Clamp), 
                            m_sourceRect(0,0,0,0),
                            m_interpolationMode(InterpolationMode::Linear)
{

}


ImageBrush::~ImageBrush(void)
{
}


auto ImageBrush::OnCreateResource(ID2D1DeviceContext* deviceContext) -> ComPtr<IUnknown> 
{
    if(m_image)
    {
        auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

        if(bitmap)
        {
            auto size = bitmap->GetSize();
            m_sourceRect.bottom = size.height;
            m_sourceRect.right = size.width;
        }

        ComPtr<ID2D1ImageBrush> brush = nullptr;

        auto image = GetResourceFrom<ID2D1Image>(m_image);

        deviceContext->CreateImageBrush(image, 
           ImageBrushProperties(
                (D2D1_RECT_F)m_sourceRect, 
                (D2D1_EXTEND_MODE)m_extentModeX, 
                (D2D1_EXTEND_MODE)m_extentModeY, 
                (D2D1_INTERPOLATION_MODE)m_interpolationMode), 
                 BrushProperties(m_Opacity, m_transform),
                    &brush);

        return brush;
    }
    else
    {
         ComPtr<ID2D1SolidColorBrush> brush = nullptr;

         deviceContext->CreateSolidColorBrush(Color(0,0,0,1), &brush);

         return brush;
    }
}

auto ImageBrush::IsImageSet() -> bool
{
    return m_image && IsResourceValid();
}

ExtendMode ImageBrush::ExtentModeX() const
{
    return m_extentModeX;
}

ExtendMode ImageBrush::ExtentModeY() const
{
    return m_extentModeY;
}

void ImageBrush::ExtentModeX(ExtendMode extentMode)
{
    m_extentModeX = extentMode;
    if(IsImageSet())
    {
        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

        brush->SetExtendModeX((D2D1_EXTEND_MODE)m_extentModeX);
        NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
    }
}

auto ImageBrush::SourceRect() const -> Graphucks::Rectangle
{
    return m_sourceRect;
}

void ImageBrush::ExtentModeY(ExtendMode extentMode)
{
    m_extentModeY = extentMode;
    if(IsImageSet())
    {
        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

        brush->SetExtendModeY((D2D1_EXTEND_MODE)m_extentModeY);
        NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
    }
}

void ImageBrush::SourceRect(const Graphucks::Rectangle& rect)
{
    m_sourceRect = rect;

    if(IsImageSet())
    {
        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

        D2D1_RECT_F r = m_sourceRect;
        brush->SetSourceRectangle(&r);
        NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
    }
}

void ImageBrush::Image(const ImagePtr image)
{
    m_image = image;

    auto bitmap = dynamic_cast<Bitmap*>(m_image.get());

    if(bitmap)
    {
        auto size = bitmap->GetSize();
        m_sourceRect.bottom = size.height;
        m_sourceRect.right = size.width;
    }

    if(IsImageSet() && IsResourceValidFrom(image))
    {


        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();
        auto img = GetResourceUnsafeFrom<ID2D1Image>(image);

        brush->SetImage(img);
        NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
    }
    else
    {
        Invalidate();
    }
}

auto ImageBrush::Image() -> ImagePtr
{
    return m_image;
}

void ImageBrush::Interpolation(InterpolationMode mode)
{
    m_interpolationMode = mode;

    if(IsImageSet())
    {
        auto brush = GetResourceUnsafe<ID2D1ImageBrush>();

        brush->SetInterpolationMode((D2D1_INTERPOLATION_MODE) m_interpolationMode);
        NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
    }
}

auto ImageBrush::Interpolation() const -> InterpolationMode
{
    return m_interpolationMode;
}

auto ImageBrush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen) -> D2D1::Matrix3x2F 
{
    D2D1_RECT_F bounds;

    bounds = geometry->GetBounds(pen, Matrix3x2F::Identity());

    auto size = SizeF(m_sourceRect.right - m_sourceRect.left, 
                      m_sourceRect.bottom - m_sourceRect.top);

    auto widthRatio = (bounds.right - bounds.left) / size.width;
    auto heightRatio = (bounds.bottom - bounds.top) / size.height;

    auto matrix = Matrix3x2F::Translation(bounds.left, bounds.top);
    matrix = matrix * Matrix3x2F::Scale(widthRatio, heightRatio, 
                                        D2D1::Point2F(bounds.left, bounds.top));
    return matrix;
}

auto ImageBrush::GetTransformToBounds(const Graphucks::Rectangle& rect) -> D2D1::Matrix3x2F
{
    auto bounds = rect;

    auto size = SizeF(m_sourceRect.right - m_sourceRect.left, 
                      m_sourceRect.bottom - m_sourceRect.top);

    auto widthRatio = (bounds.right - bounds.left) / size.width;
    auto heightRatio = (bounds.bottom - bounds.top) / size.height;

    auto matrix = Matrix3x2F::Translation(bounds.left, bounds.top);
    matrix = matrix * Matrix3x2F::Scale(widthRatio, heightRatio, 
                                        D2D1::Point2F(bounds.left, bounds.top));
    return matrix;
}