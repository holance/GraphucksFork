#pragma once
#include "GraphucksComponent.h"
#include <d2d1_1.h>
#include <vector>
#include <wrl.h>


namespace Graphucks
{

    class RenderContext;
    class ImageBrush;
    class RadialGradientBrush;
    class EffectResource;
    class ElementEffect;
    class ExecutableBase;
    class ScopedCommand;
    class VisualElement;
    class DependantValueEntry;
    class DependencyObject;

    struct IInvalidatable;
    typedef std::weak_ptr<IInvalidatable>   IInvalidatableWeakPtr;
    typedef std::shared_ptr<IInvalidatable> IInvalidatablePtr;

    enum INVALIDATE_FLAGS
    {
        INVALIDATE_FLAG_MEASURE = 0x0,
        INVALIDATE_FLAG_ARRANGE = 0x1,
        INVALIDATE_FLAG_TRANSFORM = 0x2,
        INVALIDATE_FLAG_VISUAL = 0x4
    };

    struct IInvalidatable
    {
        virtual void Invalidate(int flags) = 0;
    };

    class DeviceDependantResource;
    typedef std::shared_ptr<DeviceDependantResource> DeviceDependantResourcePtr;

    class DependantObject;
    typedef std::shared_ptr<DependantObject> DependantObjectPtr;


    class DependantObject
    {
    public:

        DependantObject()
        {

        }

        virtual ~DependantObject()
        {

        }

        std::wstring Name()
        {
            return m_name;
        }

        void Name(std::wstring name)
        {
            m_name = name;
        }
    protected:
        void NotifyInvalidatables(INVALIDATE_FLAGS flags);
    private:
        void RegisterInvalidatable(IInvalidatable* invalidatable);
        void UnregisterInvalidatable(IInvalidatable* invalidatable);
    private:
        std::wstring m_name;
        std::vector<IInvalidatable*>  m_invalidatables;
        friend                              DependencyObject;
        friend                              VisualElement;
        friend                              DependantValueEntry;
    };

    class DeviceDependantResource : public DependantObject
    {
    public:
        DeviceDependantResource() : m_deviceContext(nullptr), m_isDirty(true)
        {
        }
    protected:
        virtual auto IsResourceValid() const -> bool
        {
            if(m_isDirty || !m_lastResource)
                return false;

            return true;
        }

        static auto IsResourceValidFrom(const DeviceDependantResourcePtr& resource) -> bool
        {
            if(resource == nullptr)
                return false;

            return resource->IsResourceValid();
        }

        static auto IsResourceValidFrom(DeviceDependantResource* const resource) -> bool
        {
            return resource->IsResourceValid();
        }

        void Invalidate()
        {
            m_isDirty = true;
            NotifyInvalidatables(INVALIDATE_FLAG_VISUAL);
        }

        virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource(ID2D1DeviceContext* deviceContext) = 0;
        
        template<typename T>
        T* GetResourceUnsafe()
        {
            return reinterpret_cast<T*>(m_lastResource.Get());
        }

        template<typename T>
        T* GetResourceUnsafeFrom(const DeviceDependantResourcePtr& deviceDependantResource)
        {
            return deviceDependantResource->GetResourceUnsafe<T>();
        }

        ID2D1DeviceContext* GetDeviceContext()
        {
            return m_deviceContext;
        }

        template<typename T>
        T* GetResource(ID2D1DeviceContext* deviceContext)
        {
            SetDeviceContext(deviceContext);

            if(!IsResourceValid())
            {
                m_lastResource = OnCreateResource(m_deviceContext);
                m_isDirty = false;
            }
          
            return reinterpret_cast<T*>(m_lastResource.Get());
        }

        template<typename T>
        T* GetResourceFrom(const DeviceDependantResourcePtr& deviceDependantResource, ID2D1DeviceContext* deviceContext)
        {
            return deviceDependantResource->GetResource<T>(deviceContext);
        }
        
        template<typename T>
        T* GetResourceFrom(const DeviceDependantResourcePtr& deviceDependantResource)
        {
            return deviceDependantResource->GetResource<T>(m_deviceContext);
        }
    private:
        void SetDeviceContext(ID2D1DeviceContext* deviceContext)
        {
            if(m_deviceContext != deviceContext)
            {
                m_deviceContext = deviceContext;
                Invalidate();
            }
        }

        Microsoft::WRL::ComPtr<IUnknown>    m_lastResource;
        ID2D1DeviceContext*                 m_deviceContext;
        bool                                m_isDirty;

        friend                              RenderContext;
        friend                              ExecutableBase;
        friend                              ScopedCommand;
        friend                              ElementEffect;
        
    };

    class IndependantResource;
    typedef std::shared_ptr<IndependantResource> IndependantResourcePtr;

    class IndependantResource
    {
    public:
        IndependantResource() : m_isDirty(true)
        {
        }
    protected:
        virtual auto IsResourceValid() const -> bool
        {
            if(m_isDirty || !m_lastResource)
                return false;

            return true;
        }

        static auto IsResourceValid(const IndependantResourcePtr& resource) -> bool
        {
            return resource->IsResourceValid();
        }

        void Invalidate()
        {
            m_isDirty = true;
            m_lastResource.ReleaseAndGetAddressOf();
        }

        virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource() = 0;
        
        template<typename T>
        T* GetResourceUnsafe()
        {
            return reinterpret_cast<T*>(m_lastResource.Get());
        }

    protected:
        template<typename T>
        T* GetResource()
        {
            if(!IsResourceValid())
            {
                m_lastResource = OnCreateResource();
                m_isDirty = false;
            }
          
            return reinterpret_cast<T*>(m_lastResource.Get());
        }

        template<typename T>
        T* GetResourceFrom(const IndependantResourcePtr& independantResource)
        {
            return independantResource->GetResource<T>();
        }

    private:
        Microsoft::WRL::ComPtr<IUnknown> m_lastResource;
        bool                             m_isDirty;
        friend                           RenderContext;
        friend                           ExecutableBase;
        friend                           ScopedCommand;
    };

}