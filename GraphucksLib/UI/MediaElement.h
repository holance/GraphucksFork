#pragma once
#include "VisualElement.h"
#include "..\Media\MediaPlayer.h"
#include <functional>

namespace Graphucks
{
    class MediaElement;
    typedef std::shared_ptr<MediaElement> MediaElementPtr;

    class MediaElement : public VisualElement
    {
    public:
        MediaElement(void);
        virtual ~MediaElement(void);
    public:
#if defined(__cplusplus_winrt)
        void SetSource(Windows::Foundation::Uri^ uri);
        void SetSource(Windows::Storage::Streams::IRandomAccessStream^ randomAccessStream);
#else
        void SetSource(const std::wstring& uri);
#endif
        auto Stretch() const -> Graphucks::Stretch;
        void Stretch(Graphucks::Stretch stretch);

        auto StretchDirection() const -> Graphucks::StretchDirection;
        void StretchDirection(Graphucks::StretchDirection val);

    protected:
        virtual Size ArrangeOverride(const Graphucks::Size& finalSize) override;
        virtual Size MeasureOverride(const Graphucks::Size& availableSize) override;
        virtual void RenderOverride( const std::shared_ptr<IRenderContext>& renderContext ) override;
        virtual void Rendering() override;
    private:
        Size CalculateScale(const Size& availableSize);
        void MediaPlayerMediaLoaded(EventArgs args);

        void EnsureInitialized();
    private:
        std::shared_ptr<MediaPlayer> m_mediaPlayer;
        Graphucks::Stretch           m_stretch;
        Graphucks::StretchDirection  m_stretchDirection;
        bool                         m_initialized;
    };
}