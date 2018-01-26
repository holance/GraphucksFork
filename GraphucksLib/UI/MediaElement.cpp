#include "pch.h"
#include "MediaElement.h"

using namespace Graphucks;
#if defined(__cplusplus_winrt)
using namespace Windows::Storage::Streams;
#endif
using namespace std;



MediaElement::MediaElement(void) : m_mediaPlayer(make_shared<MediaPlayer>()), 
                                   m_stretch(Stretch::Uniform), 
                                   m_stretchDirection(StretchDirection::Both),
                                   m_initialized(false)
{
    //auto x = shared_ptr<MediaElement>(this);

   
   // 
}


MediaElement::~MediaElement(void)
{
}

void MediaElement::RenderOverride( const IRenderContextPtr& renderContext )
{
    auto renderSize = GetRenderSize();
    renderContext->DrawMediaPlayer(m_mediaPlayer, renderSize);
}

void MediaElement::Rendering()
{
    if(m_mediaPlayer->IsFrameReady())
        InvalidateVisual();
}

#if defined(__cplusplus_winrt)
void MediaElement::SetSource( IRandomAccessStream^ randomAccessStream )
{
    EnsureInitialized();
    m_mediaPlayer->SetSource(randomAccessStream);
}

void MediaElement::SetSource( Windows::Foundation::Uri^ uri )
{
    EnsureInitialized();
    m_mediaPlayer->SetSource(uri);
}
#else
void MediaElement::SetSource( const std::wstring& uri )
{
    EnsureInitialized();
    m_mediaPlayer->SetSource(uri);
}

#endif

Size MediaElement::ArrangeOverride( const Graphucks::Size& finalSize )
{
    if(!m_mediaPlayer)
        return Size();

    auto mediaSize = m_mediaPlayer->GetNativeSize();

    auto scale = CalculateScale(finalSize);
    
    return Size(mediaSize.width * scale.width, mediaSize.height * scale.height); 
}


auto MediaElement::StretchDirection() const -> Graphucks::StretchDirection 
{
    return m_stretchDirection;
}

void MediaElement::StretchDirection(Graphucks::StretchDirection val)
{
    m_stretchDirection = val;
    InvalidateVisual();
}

auto MediaElement::Stretch() const -> Graphucks::Stretch
{
    return m_stretch;
}

void MediaElement::Stretch(Graphucks::Stretch stretch)
{
    m_stretch = stretch;
    InvalidateVisual();
}


Size MediaElement::CalculateScale(const Size& availableSize)
{
    float scaleX = 1.0;
    float scaleY = 1.0;
 
    auto bitmapSize = m_mediaPlayer->GetNativeSize();

    bool isConstrainedWidth = availableSize.width != GRAPHUCKS_INFINITY;
    bool isConstrainedHeight = availableSize.height != GRAPHUCKS_INFINITY;
 
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


Size MediaElement::MeasureOverride( const Graphucks::Size& availableSize )
{
    if(!m_mediaPlayer)
        return Size();

    auto bitmapSize = m_mediaPlayer->GetNativeSize();

    auto scale = CalculateScale(availableSize);
    
    return Size(bitmapSize.width * scale.width, bitmapSize.height * scale.height); 
}

void MediaElement::MediaPlayerMediaLoaded( EventArgs args )
{

}

void MediaElement::EnsureInitialized()
{
    if(m_initialized)
        return;

    m_initialized = true;

    m_mediaPlayer->MediaLoaded() += CreateDelegate<EventArgs>(&MediaElement::MediaPlayerMediaLoaded, dynamic_pointer_cast<MediaElement>(shared_from_this()));
}
