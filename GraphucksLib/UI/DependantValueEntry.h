#include "DependencyProperty.h"
#include "DependencyObject.h"

namespace Graphucks
{
    class DependantValueEntry;
    typedef std::shared_ptr<DependantValueEntry> DependantValueEntryPtr;

    class DependantValueEntry
    {
    public:
        DependantValueEntry(const DependencyPropertyPtr& dependencyProperty, DependencyObject* const dependencyObject) : m_isAnimated(FALSE), m_dependencyProperty(dependencyProperty), m_dependencyObject(dependencyObject)
        {
        
        }

        virtual ~DependantValueEntry()
        {
            UnregisterDependants();
        }

        DependencyPropertyPtr DependencyProperty() const { return m_dependencyProperty; }
        
        SharedVoid BaseValue() const { return m_baseValue; }
        void BaseValue(const Graphucks::SharedVoid val) 
        { 
            m_baseValue = val; 
            RegisterDependantCalculatedValue();
        }

        SharedVoid AnimatedValue() const { return m_animatedValue; }
        void AnimatedValue(const Graphucks::SharedVoid& val) 
        {
            m_animatedValue = val; 
            RegisterDependantCalculatedValue();
        }

        SharedVoid InheritatedValue() const { return m_inheritatedValue; }
        void InheritatedValue(const SharedVoid& val)
        { 
            m_inheritatedValue = val; 
            RegisterDependantCalculatedValue();
        }

        bool HasCalculatedValue()
        {
            return ( IsAnimated() && m_animatedValue ) || m_baseValue || m_inheritatedValue;
        }

        bool HasBaseValue() const
        {
            return m_baseValue != nullptr;
        }

        bool HasInheritedValue() const
        {
            return m_inheritatedValue != nullptr;
        }

        bool HasAnimatedValue() const
        {
            return m_animatedValue != nullptr;
        }

        void UnregisterDependants()
        {
            
            if(m_dependencyProperty->PropertyMetadata()->IsDepenantObjectType())
            {

                if(m_animatedValue)
                {
                    auto item = static_cast<DependantObject*>(m_animatedValue.get());
                    item->UnregisterInvalidatable(reinterpret_cast<IInvalidatable*>(m_dependencyObject));
                }

                if(m_inheritatedValue)
                {
                    auto item = reinterpret_cast<DependantObject*>(m_inheritatedValue.get());
                    item->UnregisterInvalidatable(reinterpret_cast<IInvalidatable*>(m_dependencyObject));
                }

                if(m_baseValue)
                {
                    auto item = static_cast<DependantObject*>(m_baseValue.get());
                    item->UnregisterInvalidatable(reinterpret_cast<IInvalidatable*>(m_dependencyObject));
                }

            }
        }

        SharedVoid CalculatedValue()
        {
            SharedVoid returnValue;

            if(IsAnimated() && m_animatedValue)
            {
                returnValue = m_animatedValue;
            }
            else if (m_baseValue)
            {
                returnValue = m_baseValue;
            }
            else if(m_inheritatedValue)
            {
                returnValue = m_inheritatedValue;
            }

            return returnValue;
        }
        bool IsAnimated() const { return m_isAnimated; }
    private:
        void RegisterDependantCalculatedValue()
        {
            bool isDependantType = m_dependencyProperty->PropertyMetadata()->IsDepenantObjectType();

            if (!isDependantType)
            {
                return;
            }

            UnregisterDependants();
            auto calcedValue = CalculatedValue();

            if(calcedValue)
            {
                auto item = static_cast<DependantObject*>(calcedValue.get());
                item->RegisterInvalidatable(reinterpret_cast<IInvalidatable*>(m_dependencyObject));
            }
        }
        DependencyPropertyPtr m_dependencyProperty;
        SharedVoid            m_baseValue;
        SharedVoid            m_animatedValue;
        SharedVoid            m_inheritatedValue;
        bool                  m_isAnimated;
        DependencyObject*     m_dependencyObject;
    };
}
