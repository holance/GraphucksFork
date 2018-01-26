#pragma once
#include <memory>
#include <string>
#include <functional>

#include "..\core\Resource.h"
#include "DependencyPropertyUtils.h"

namespace Graphucks
{
    enum class FrameworkPropertyMetadataOptions : int 
    {
        None                            = 0x0,
        AffectsMeasure                  = 0x1, 
        AffectsArrange                  = 0x2, 
        AffectsRender                   = 0x4, 
        Inherits                        = 0x8,
        OverridesInheritanceBehavior    = 0x10
    };

    typedef std::shared_ptr<void> SharedVoid;

    class PropertyChangedEventArgs;
    typedef std::shared_ptr<PropertyChangedEventArgs> PropertyChangedEventArgsPtr;

    class PropertyChangedEventArgs
    {
    public:
        PropertyChangedEventArgs(const SharedVoid& oldValue, const SharedVoid& newValue) : 
                                 m_oldValue(oldValue), m_newValue(newValue)
        {

        }

        const SharedVoid& OldValue() const
        {
            return m_oldValue;
        }

        const SharedVoid& NewValue() const
        {
            return m_newValue;
        }

        template <class T>
        T NewValue() const
        {
            return *(T*)(m_newValue.get());
        }

        template <class T>
        T OldValue() const 
        {
            return *(T*)(m_oldValue.get());
        }
    private:
        SharedVoid m_oldValue;
        SharedVoid m_newValue;
    };

    template <class T>
    bool ValueEqualityCallback(const SharedVoid& oldVal, const SharedVoid& newVal)
    {
        T*  oldPtr = static_cast<T*>(oldVal.get());
        T*  newPtr = static_cast<T*>(newVal.get());

        return *oldPtr == *newPtr;
    }



    class DependencyObject;

    typedef std::function<void(DependencyObject* const dependencyObject, const PropertyChangedEventArgs& args)>   PropertyChangedCallback;
    typedef std::function<bool(const SharedVoid& oldValue,               const SharedVoid& newValue)>             PropertyEqualityCallback;

    class FrameworkPropertyMetadata;
    typedef std::shared_ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadataPtr;

    class FrameworkPropertyMetadata
    {
    public:
        FrameworkPropertyMetadata(SharedVoid defaultValue, 
                                 int frameworkPropertyMetadataOptionsFlags, 
                                 PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback) :
                                    m_defaultValue(defaultValue), 
                                    m_changeCallback(changeCallback), 
                                    m_propertyEqualityCallback(propertyEqualityCallback),
                                    m_frameworkPropertyMetadataOptionsFlags(frameworkPropertyMetadataOptionsFlags),
                                    m_isDependantObjectType(false)
        {

        }
    public:
        SharedVoid DefaultValue() const { return m_defaultValue; }
        PropertyChangedCallback ChangeCallback() const { return m_changeCallback; }
        PropertyEqualityCallback EqualityCallback() const { return m_propertyEqualityCallback; }
        int FrameworkPropertyMetadataOptionsFlags() const { return m_frameworkPropertyMetadataOptionsFlags; }


        bool IsDepenantObjectType() const
        {
            return m_isDependantObjectType;
        }
        bool Inherits() const{ return (m_frameworkPropertyMetadataOptionsFlags & (int)FrameworkPropertyMetadataOptions::Inherits) == (int)FrameworkPropertyMetadataOptions::Inherits; }

        template <typename T>
        static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, 
                                                   int frameworkPropertyMetadataOptionsFlags, 
                                                   PropertyChangedCallback changeCallback, 
                                                   PropertyEqualityCallback propertyEqualityCallback = nullptr)
        {
            return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, changeCallback, propertyEqualityCallback);
        }

        template <typename T>
        typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
        static Create(const T&& defaultValue, 
                      int frameworkPropertyMetadataOptionsFlags, 
                      PropertyChangedCallback changeCallback, 
                      PropertyEqualityCallback propertyEqualityCallback = nullptr)
        {
            auto defaultBoxedValue = make_shared<T>(defaultValue);
            PropertyEqualityCallback equalityCallback = propertyEqualityCallback;

            if (equalityCallback != nullptr)
            {
                equalityCallback = &ValueEqualityCallback<T>;
            }
            return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, changeCallback, equalityCallback);
        }
        
    private:

        template <typename T>
        typename std::enable_if<!std::is_assignable<DependantObjectPtr, T>::value, FrameworkPropertyMetadataPtr>::type
        static CreateTemplate(const std::shared_ptr<T>& defaultValue, 
                               int frameworkPropertyMetadataOptionsFlags, 
                               PropertyChangedCallback changeCallback, 
                               PropertyEqualityCallback propertyEqualityCallback = nullptr)
        {
           
            return std::make_shared<FrameworkPropertyMetadata>(defaultValue, 
                                                               frameworkPropertyMetadataOptionsFlags, 
                                                               changeCallback,
                                                               propertyEqualityCallback);
        }

        template <typename T>
        typename std::enable_if<std::is_assignable<DependantObjectPtr, T>::value, FrameworkPropertyMetadataPtr>::type
        static CreateTemplate(const std::shared_ptr<T>& defaultValue, 
                              int frameworkPropertyMetadataOptionsFlags, 
                              PropertyChangedCallback changeCallback,
                              PropertyEqualityCallback propertyEqualityCallback)
        {
            auto metaData = std::make_shared<FrameworkPropertyMetadata>(defaultValue, 
                                                                        frameworkPropertyMetadataOptionsFlags, 
                                                                        changeCallback, 
                                                                        propertyEqualityCallback);
            metaData->m_isDependantObjectType = true;
            return metaData;
        }
    private:
        SharedVoid m_defaultValue;
        int m_frameworkPropertyMetadataOptionsFlags;
        bool m_isDependantObjectType;

        PropertyChangedCallback m_changeCallback;
        PropertyEqualityCallback m_propertyEqualityCallback;
    };

    class DependencyProperty;
    typedef std::shared_ptr<DependencyProperty> DependencyPropertyPtr;

    class DependencyProperty
    {
    private:
        typedef std::function<bool(const DependencyObject* obj)> IsOfOwnerTypeCallback;
        IsOfOwnerTypeCallback m_isofOwnerType;

        DependencyProperty(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata);
    public:
        
        virtual ~DependencyProperty(void);
    public:
        std::wstring Name() const { return m_name; }
        FrameworkPropertyMetadataPtr PropertyMetadata() const { return m_propertyMetadata; }

    public:
        template <class T>
        static DependencyPropertyPtr Register(const std::wstring& name, const FrameworkPropertyMetadataPtr& propertyMetadata)
        {
            auto isofOwnerType = &IsOfOwnerTypeTemplate<T>;
            return Register(name, isofOwnerType, propertyMetadata);
        }

        bool IsOfOwnerType(const DependencyObject* obj)
        {
            return m_isofOwnerType(obj);
        }

    private:
        static DependencyPropertyPtr Register(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata);
    private:
        template<class T>
        static bool IsOfOwnerTypeTemplate(const DependencyObject* obj)
        {
            auto casted = dynamic_cast<const T*>(obj);
            return casted != nullptr;
        }
        std::wstring                        m_name;
        FrameworkPropertyMetadataPtr        m_propertyMetadata;
    };
}
