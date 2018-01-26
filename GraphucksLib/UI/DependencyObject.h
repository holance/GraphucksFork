#pragma once
#include "DependencyProperty.h"
#include "DependantValueEntry.h"
#include "..\Core\Resource.h"

#include <map>
#include <memory>
#include <type_traits>

namespace Graphucks
{
    template <typename T>
    typename std::enable_if<std::is_base_of<DependencyObject, T>::value, std::shared_ptr<T>>::type gmake_shared()
    {
        auto item = std::make_shared<T>();
        item->Initialize();
        return item;
    }

    template <typename T>
    typename std::enable_if<!std::is_base_of<DependencyObject, T>::value, std::shared_ptr<T>>::type gmake_shared()
    {
        auto item = std::make_shared<T>();
        return item;
    }

    template <typename T, typename A1>
    typename std::enable_if<std::is_base_of<DependencyObject, T>::value, std::shared_ptr<T>>::type gmake_shared(A1&& a1)
    {
        auto item = std::make_shared<T>(a1);
        item->Initialize();
        return item;
    }

    template <typename T, typename A1>
    typename std::enable_if<!std::is_base_of<DependencyObject, T>::value, std::shared_ptr<T>>::type gmake_shared(A1&& a1)
    {
        auto item = std::make_shared<T>(a1);
        return item;
    }

    class DependencyObject : public IInvalidatable
    {
        template <typename T>
        friend typename std::enable_if<std::is_base_of<DependencyObject, T>::value, std::shared_ptr<T>>::type gmake_shared();
    public:
        DependencyObject(void);
        virtual ~DependencyObject(void);

    public:

        template <typename T>
        typename std::enable_if<std::is_base_of<DependantObject, T>::value, std::shared_ptr<T>>::type
        GetValue(const DependencyPropertyPtr& dependencyProperty)
        {
            auto val = GetValue(dependencyProperty);
            
            return std::dynamic_pointer_cast<T>(std::static_pointer_cast<DependantObject>(val));
        }

        template <typename T>
        typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, T>::type 
        GetValue(const DependencyPropertyPtr& dependencyProperty)
        {
            auto val = GetValue(dependencyProperty);
            
            return *static_cast<T*>(val.get());
        }


        SharedVoid GetValue(const DependencyPropertyPtr& dependencyProperty);

    public:
        template <class T>
        void SetValue(const DependencyPropertyPtr& dependencyProperty, const T& value)
        {
            /* probably use template specialization instead of SFINAE */
            SetValueTemplate<T>(dependencyProperty, value);
        }

        void ClearValue(const DependencyPropertyPtr& dependencyProperty);
        
         virtual void Invalidate(int /*flags*/)  /* IInvalidatable */
         {}
    protected:

         void SetInheritanceContext(DependencyObject* inheritanceContext);

         virtual void OnPropertyChanged(const DependencyPropertyPtr& property, const PropertyChangedEventArgs&);

         virtual void SetInheritedValue( const DependencyPropertyPtr& property, const SharedVoid& value);

         virtual void Initialize()
         {

         }
    private:
        enum class ValueSource
        {
            Inherited,
            Explicit,
            Animation
        };

        
        template <typename T>
        void
        SetValueTemplate(const DependencyPropertyPtr& dp, const DependantObjectPtr& value)
        {
            auto newVal = std::static_pointer_cast<DependantObject>(value);
            SetValueImpl(dp, value);
        }
        
        template <typename T>
        typename std::enable_if<std::is_assignable<SharedVoid, T>::value && !std::is_assignable<DependantObjectPtr, T>::value>::type
        SetValueTemplate(const DependencyPropertyPtr& dp, const T& value)
        {
            SetValueImpl(dp, value);
        }

        template <typename T>
        typename std::enable_if<!std::is_assignable<SharedVoid, T>::value>::type
        SetValueTemplate(const DependencyPropertyPtr& dp, const T& value)
        {
            auto boxedValue = std::make_shared<T>(value);
            SetValueImpl(dp, boxedValue);
        }

        void SetValueImpl(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& value, const ValueSource valueSource=ValueSource::Explicit);

    private:
        bool AreValuesSame(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& oldValue, const SharedVoid& newValue);
        void InvokePropertyChanged(const DependencyPropertyPtr& dependencyProperty, const SharedVoid& oldValue, const SharedVoid& newValue);
    private:
        SharedVoid GetValueInternal(const DependencyPropertyPtr& dependencyProperty);
    private:
        std::map<DependencyPropertyPtr, DependantValueEntryPtr> m_valueEntries;
        bool                                                    m_isInvalidatable;
        DependencyObject*                                       m_inheritanceContext;
        bool                                                    m_inheritanceContextSet;
        std::vector<DependantObjectPtr> m_queuedDependantObjects; /* workaround for shared_from_this in ctor */
    };                                                     
}