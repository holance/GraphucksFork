#pragma once
#if defined(__cplusplus_winrt)
#include "..\SceneRoot.h"

namespace Graphucks
{
    ref class XamlSurfaceRenderer;
    class RenderContext;

    private ref class XamlSceneRoot sealed : public Windows::UI::Xaml::Controls::Control
    {
        public:
            XamlSceneRoot();
            virtual ~XamlSceneRoot(void);
        internal:
            void SetRoot(const VisualElementPtr& rootElement);
            void SetBackBufferSize(const Graphucks::Size& size);
        protected:
            virtual void OnApplyTemplate() override;
            virtual void OnPointerPressed (Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) override;
            virtual void OnPointerReleased(Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e) override;
            void CreateBackBuffer();
            virtual void OnLoaded       (Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
            virtual void OnUnloaded     (Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
            void CompositionTargetRender(Platform::Object^ sender, Platform::Object^ e);
        private:
            std::shared_ptr<SceneRoot>                      m_sceneRoot;
            std::shared_ptr<RenderContext>                  m_renderContext;
            XamlSurfaceRenderer^                            m_surfaceRenderer;
            Windows::UI::Xaml::Controls::Image^             m_image;
            Microsoft::WRL::ComPtr<ID3D11Device>            m_device;
            Graphucks::Size                                 m_backBufferSize;
            Windows::Foundation::EventRegistrationToken     m_renderCallbackToken;
            Windows::Foundation::EventRegistrationToken     m_loadedEventToken;
            Windows::Foundation::EventRegistrationToken     m_unloadedEventToken;
        };
}
#endif
