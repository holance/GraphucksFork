#include "pch.h"
#include "ImageElement.h"

using namespace Graphucks;
using namespace std;
using namespace Microsoft::WRL;

ImageElement::ImageElement(void) : m_stretch(Stretch::Uniform), m_stretchDirection(StretchDirection::Both)
{
}

ImageElement::~ImageElement(void)
{
}

void ImageElement::RenderOverride(const IRenderContextPtr& renderContext)
{
    if(!m_bitmap)
        return;

    auto renderSize = GetRenderSize();
    renderContext->DrawBitmap(m_bitmap, EffectCompositeMode::SourceOver, renderSize);
}

Size ImageElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
    if(!m_bitmap)
        return Size();

    auto bitmapSize = m_bitmap->GetSize();

    auto scale = CalculateScale(finalSize);
    
    return Size(bitmapSize.width * scale.width, bitmapSize.height * scale.height); 
}

Size ImageElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    if(!m_bitmap)
        return Size();

    auto bitmapSize = m_bitmap->GetSize();

    auto scale = CalculateScale(availableSize);
    
    return Size(bitmapSize.width * scale.width, bitmapSize.height * scale.height); 
}

const BitmapPtr& ImageElement::Source()
{
    return m_bitmap;
}

void ImageElement::Source(const BitmapPtr& bitmapSource)
{
    m_bitmap = bitmapSource;
    InvalidateVisual();
}

auto ImageElement::StretchDirection() const -> Graphucks::StretchDirection 
{
    return m_stretchDirection;
}

void ImageElement::StretchDirection(Graphucks::StretchDirection val)
{
    m_stretchDirection = val;
    InvalidateVisual();
}

auto ImageElement::Stretch() const -> Graphucks::Stretch
{
    return m_stretch;
}

void ImageElement::Stretch(Graphucks::Stretch stretch)
{
    m_stretch = stretch;
    InvalidateVisual();
}

Size ImageElement::CalculateScale(const Size& availableSize)
{
    float scaleX = 1.0;
    float scaleY = 1.0;
 
    auto bitmapSize = m_bitmap->GetSize();

    bool isConstrainedWidth = availableSize.width != Graphucks::GRAPHUCKS_INFINITY;
    bool isConstrainedHeight = availableSize.height != Graphucks::GRAPHUCKS_INFINITY;
 
    if ((m_stretch == Stretch::Uniform || m_stretch == Stretch::UniformToFill || m_stretch == Stretch::Fill)
        &&  (isConstrainedWidth || isConstrainedHeight)) 
    {
        scaleX = bitmapSize.width == 0 ? 0.0f : availableSize.width / bitmapSize.width;
        scaleY = bitmapSize.height == 0 ? 0.0f : availableSize.height / bitmapSize.height; 

        if (!isConstrainedWidth)        scaleX = scaleY; 
        else if (!isConstrainedHeight)  scaleY = scaleX; 
        else
        { 
            switch (m_stretch)
            {
            case Stretch::Uniform:
                {
                    float minscale = scaleX < scaleY ? scaleX : scaleY;
                    scaleX = scaleY = minscale; 
                }
                break; 
            case Stretch::UniformToFill:
                {
                    float maxscale = scaleX > scaleY ? scaleX : scaleY;
                    scaleX = scaleY = maxscale;
                }
                break;
            } 
        }

        switch(m_stretchDirection)
        { 
        case StretchDirection::UpOnly: 
                if (scaleX < 1.0f) scaleX = 1.0f;
                if (scaleY < 1.0f) scaleY = 1.0f; 
                break;

        case StretchDirection::DownOnly:
                if (scaleX > 1.0f) scaleX = 1.0f; 
                if (scaleY > 1.0f) scaleY = 1.0f;
                break; 
 
        case StretchDirection::Both:
                break; 

            default:
                break;
        } 
    }

    return Size(scaleX, scaleY);
}
