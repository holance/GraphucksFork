#pragma once
#include <string>
#include <wrl.h>
#include <mfmediaengine.h>
#include <d3d11.h>
#include "..\Rendering\DX.h"
#include "..\Core\GraphicsTypes.h"
#include "..\UI\Delegate.h"

namespace Graphucks
{
    class MediaPlayer;
    typedef std::shared_ptr<MediaPlayer> MediaPlayerPtr;

    class MediaPlayer
    {
    public:
        MediaPlayer(void);
        virtual ~MediaPlayer(void);
    public:
#if defined(__cplusplus_winrt)
        void SetSource(Windows::Foundation::Uri^ uri);
        void SetSource(Windows::Storage::Streams::IRandomAccessStream^ randomAccessStream);
#else
        void SetSource(const std::wstring& uri);
#endif                            
        void Play();
        void Pause();
        bool IsFrameReady();

        Graphucks::Size GetNativeSize();
        Microsoft::WRL::ComPtr<IDXGISurface> GetTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& device);

        const EventBase<EventArgs>&  MediaLoaded();
    private:
        void Initialize();
        void InitializeFramebuffer();
        void InitializeDirect3D();
        void OnMediaEngineEvent(DWORD meEvent);
    private:
        
    class MediaEngineNotify : public IMFMediaEngineNotify
    {
        long m_cRef;
        MediaPlayer* m_pCB;

    public:
        MediaEngineNotify() : m_cRef(0), m_pCB(nullptr)
        {
        }

        STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
        {
            if(__uuidof(IMFMediaEngineNotify) == riid)
            {
                *ppv = static_cast<IMFMediaEngineNotify*>(this);
            }
            else
            {
                *ppv = nullptr;
                return E_NOINTERFACE;
            }

            AddRef();

            return S_OK;
        }      

        STDMETHODIMP_(ULONG) AddRef()
        {
            return InterlockedIncrement(&m_cRef);
        }

        STDMETHODIMP_(ULONG) Release()
        {
            LONG cRef = InterlockedDecrement(&m_cRef);
            if (cRef == 0)
            {
                delete this;
            }
            return cRef;
        }

        // EventNotify is called when the Media Engine sends an event.
        STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD /*param2*/)
        {
            if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
            {
                SetEvent(reinterpret_cast<HANDLE>(param1));			
            }
            else
            {
                m_pCB->OnMediaEngineEvent(meEvent);			
            }

            return S_OK;
        }

        void MediaEngineNotifyCallback(MediaPlayer* const mediaPlayer)
        {
            m_pCB = mediaPlayer;
        }
    private:
        ~MediaEngineNotify()
        {
        }
    };
    private:
        Microsoft::WRL::ComPtr<IMFMediaEngine>        m_mediaEngine;
        Microsoft::WRL::ComPtr<IMFMediaEngineEx>      m_mediaEngineEx;
        Microsoft::WRL::ComPtr<IMFDXGIDeviceManager>  m_dxgiManager;
        KennyKerr::Direct3D::Device                   m_device;
        KennyKerr::Direct3D::DeviceContext            m_deviceContext;
        KennyKerr::Direct3D::Texture2D                m_frameBuffer;
        UINT                                          m_resetToken;
        DXGI_FORMAT                                   m_d3dFormat;
        bool                                          m_isFrameReady;
        EventBase<EventArgs>                          m_mediaLoadedEvent;
    };
}
