#include "pch.h"
#include "MediaPlayer.h"
#include "D3DManagerLock.h"

#include <mfapi.h>

using namespace Graphucks;
using namespace std;
using namespace Microsoft::WRL;
#if defined(__cplusplus_winrt)
using namespace Windows::Storage::Streams;
#endif
using namespace KennyKerr::Direct3D;

MediaPlayer::MediaPlayer(void) : m_d3dFormat(DXGI_FORMAT_B8G8R8A8_UNORM), m_isFrameReady(false)
{
    InitializeDirect3D();
    Initialize();
}

MediaPlayer::~MediaPlayer(void)
{
    if(m_mediaEngine)
    {
        HRESULT hr = m_mediaEngine->Shutdown();
    }
     
    MFShutdown();
}

void MediaPlayer::Initialize()
{
    HRESULT hr = MFStartup(MF_VERSION);

    if(FAILED(hr))
        throw exception("Could not initialize MediaFoundation");
    
    ComPtr<IMFMediaEngineClassFactory> factory;
    ComPtr<IMFAttributes>              attributes;    	    
    ComPtr<MediaEngineNotify>          notifyInstance;

    hr = MFCreateDXGIDeviceManager(&m_resetToken, &m_dxgiManager);
    if(FAILED(hr))
        throw exception("Could not create DXGI Device Manager");

    hr = m_dxgiManager->ResetDevice(m_device.Get(), m_resetToken);
    if(FAILED(hr))
        throw exception("Could not set device to DXGI Device Manager");

    notifyInstance = new MediaEngineNotify();
    
    notifyInstance->MediaEngineNotifyCallback(this);
      
    hr = CoCreateInstance(CLSID_MFMediaEngineClassFactory, 
                          nullptr, 
                          CLSCTX_INPROC_SERVER, 
                          IID_PPV_ARGS(&factory));

    if(FAILED(hr))
        throw exception("Could not create MediaEngine class factory");

    hr = MFCreateAttributes(&attributes, 1);
    if(FAILED(hr))
        throw exception("Could not create MediaFoundation attributes");

    attributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER,        m_dxgiManager.Get());
    attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK,            notifyInstance.Get());
    attributes->SetUINT32 (MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, m_d3dFormat);
    
    hr = factory->CreateInstance(MF_MEDIA_ENGINE_WAITFORSTABLE_STATE, 
                                 attributes.Get(), 
                                 &m_mediaEngine);

    if(FAILED(hr))
        throw exception("Could not create MediaEngine instance");

    hr = m_mediaEngine.As(&m_mediaEngineEx);
}

void MediaPlayer::OnMediaEngineEvent(DWORD meEvent)
{
    switch (meEvent)
    {
    case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA:
        InitializeFramebuffer();
        m_mediaLoadedEvent.Invoke(EventArgs(nullptr));
        break;
    case MF_MEDIA_ENGINE_EVENT_CANPLAY:			
        Play();				
        break;        
    case MF_MEDIA_ENGINE_EVENT_PLAY:	
        break;				
    case MF_MEDIA_ENGINE_EVENT_PAUSE:        
        break;
    case MF_MEDIA_ENGINE_EVENT_ENDED:
        break;
    case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:        
        break;
    case MF_MEDIA_ENGINE_EVENT_ERROR:        
        break;    
    }

    return;
}

#if defined(__cplusplus_winrt)
void MediaPlayer::SetSource(Windows::Foundation::Uri^ uri)
{
    const wchar_t* pData = uri->RawUri->Data();

    //TODO: RAII this
    BSTR bUri = SysAllocString(pData);
    
    HRESULT hr = m_mediaEngineEx->SetSource(bUri);
    
    SysFreeString(bUri);

    if(FAILED(hr))
        throw exception("Could not set source uri");
}

void MediaPlayer::SetSource(IRandomAccessStream^ randomAccessStream)
{
    HRESULT hr = S_OK;
    ComPtr<IMFByteStream> mfByteStream = nullptr;	

    hr = MFCreateMFByteStreamOnStreamEx(reinterpret_cast<IUnknown*>(randomAccessStream), 
                                        &mfByteStream);
    if(FAILED(hr))
        throw exception("Could not convert random access stream to MediaFoundation byte stream");

    hr = m_mediaEngineEx->SetSourceFromByteStream(mfByteStream.Get(), L"file");
    if(FAILED(hr))
        throw exception("Could not set source stream");

    return;
}
#else

void MediaPlayer::SetSource( const std::wstring& uri )
{
     //TODO: RAII this
    BSTR bUri = SysAllocString(uri.c_str());
    
    HRESULT hr = m_mediaEngineEx->SetSource(bUri);
    
    SysFreeString(bUri);

    if(FAILED(hr))
        throw exception("Could not set source uri");
}

#endif

void MediaPlayer::Play()
{
    if(!m_mediaEngine)
        return;

    m_mediaEngine->Play();
}

void MediaPlayer::InitializeDirect3D()
{
#ifdef _DEBUG
    m_device = CreateDevice(CreateDeviceFlag::BgraSupport | CreateDeviceFlag::Debug);
#else
    m_device = CreateDevice();
#endif

    m_deviceContext = m_device.GetImmediateContext();

    m_device.AsMultiThread().SetMultithreadProtected();
}

void MediaPlayer::InitializeFramebuffer()
{
    if(!m_device || !m_mediaEngine->HasVideo())
        return;
    
    HRESULT hr = S_OK;

    CAutoDXGILock        dxgiLock(m_dxgiManager);
    ComPtr<ID3D11Device> device;
       
    dxgiLock.LockDevice(device);
    
    DWORD width, height;
    hr = m_mediaEngine->GetNativeVideoSize(&width, &height);

    if(FAILED(hr) || width == 0 || height == 0)
        throw exception("Could get native video size");

    TextureDescription2D textureProperties(static_cast<KennyKerr::Dxgi::Format>(m_d3dFormat),
                                           width,
                                           height,
                                           1, /* array size */
                                           1, /* mip map levels */
                                           BindFlag::ShaderResource | BindFlag::RenderTarget, 
                                           Usage::Default, 
                                           CpuAccessFlag::None, 
                                           ResourceMiscFlag::Shared);

    m_frameBuffer = m_device.CreateTexture2D(textureProperties);

    if(FAILED(hr))
        throw exception("Could not create frame buffer");
}

Graphucks::Size MediaPlayer::GetNativeSize()
{
    if(!m_mediaEngine || !m_mediaEngine->HasVideo())
        return Size();

    HRESULT hr = S_OK;
    DWORD width, height;
    hr = m_mediaEngine->GetNativeVideoSize(&width, &height);

    return Size(static_cast<float>(width), 
                static_cast<float>(height));
}

bool MediaPlayer::IsFrameReady()
{
    if(!m_mediaEngine || !m_mediaEngine->HasVideo())
        return false;

    if(m_isFrameReady)
        return true;

    LONGLONG t = 0;
    HRESULT hr = S_OK;

    hr = m_mediaEngineEx->OnVideoStreamTick(&t);
     
    if(hr == S_OK)
    {
        m_isFrameReady = true;
        return true;
    }

    return false;
}

void MediaPlayer::Pause()
{
    if(!m_mediaEngine)
        return;

    m_mediaEngine->Pause();
}

ComPtr<IDXGISurface> MediaPlayer::GetTexture(const ComPtr<ID3D11Device>& device)
{
    if(!m_frameBuffer.Get())
        return nullptr;

    if(m_isFrameReady)
    {
        m_mediaEngineEx->TransferVideoFrame(m_frameBuffer.Get(), 
                                            nullptr, 
                                            nullptr, 
                                            nullptr);
        m_deviceContext.Flush();
        m_isFrameReady = false;
    }

    Device dev = device.Get();

    auto sharedSurface = dev.OpenSharedResource(m_frameBuffer);

    return ComPtr<IDXGISurface>(sharedSurface.Get());
}

const EventBase<EventArgs>& MediaPlayer::MediaLoaded()
{
    return m_mediaLoadedEvent;
}
