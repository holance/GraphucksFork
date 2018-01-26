#include "pch.h"
#include "DependencyObject.h"
#include <algorithm>

using namespace std;
using namespace Graphucks;


DependencyObject::DependencyObject(void) : m_isInvalidatable(false), m_inheritanceContext(nullptr), m_inheritanceContextSet(false)
{
}


DependencyObject::~DependencyObject(void)
{
    
}

SharedVoid DependencyObject::GetValue( const DependencyPropertyPtr& dependencyProperty )
{
    auto value = GetValueInternal(dependencyProperty);
    return value;
}

SharedVoid DependencyObject::GetValueInternal(const DependencyPropertyPtr& dependencyProperty)
{
    bool hasValue = false;

    DependantValueEntryPtr entry;

    auto entryIt = m_valueEntries.find(dependencyProperty);

    if(entryIt != end(m_valueEntries))
    {
         entry = (*entryIt).second;
         if(entry->HasCalculatedValue())
            hasValue = true;
    }
    else
    {
        entry = make_shared<DependantValueEntry>(dependencyProperty, this);
        m_valueEntries[dependencyProperty] = entry;
    }

    bool isInheritable = dependencyProperty->PropertyMetadata()->Inherits();
    
    if(!hasValue && isInheritable && m_inheritanceContext)
    {
        auto inheritedValue = m_inheritanceContext->GetValueInternal(dependencyProperty);
        entry->InheritatedValue(inheritedValue);
        return inheritedValue;
    }

    if(!hasValue)
    {
        entry->InheritatedValue(dependencyProperty->PropertyMetadata()->DefaultValue());
        return dependencyProperty->PropertyMetadata()->DefaultValue();
    }

    return m_valueEntries[dependencyProperty]->CalculatedValue();
}


void DependencyObject::SetValueImpl(const DependencyPropertyPtr& dependencyProperty, 
                                    const shared_ptr<void>& value, 
                                    const ValueSource valueSource )
{
    DependantValueEntryPtr entry;

    auto entryIt = m_valueEntries.find(dependencyProperty);

    if(entryIt != end(m_valueEntries))
    {
        entry = (*entryIt).second;
    }
    else
    {
        entry = make_shared<DependantValueEntry>(dependencyProperty, this);
        m_valueEntries[dependencyProperty] = entry;
    }

    if ((entry->IsAnimated() || entry->HasBaseValue()) && valueSource == ValueSource::Inherited)
    {
         entry->InheritatedValue(value);
         return;
    }

    
    auto currentValue = GetValue(dependencyProperty);

    if(AreValuesSame(dependencyProperty, currentValue, value))
        return;

    switch(valueSource)
    {
    case ValueSource::Animation:
        entry->AnimatedValue(value);
        break;
    case ValueSource::Explicit:
        entry->BaseValue(value);
        break;
    case ValueSource::Inherited:
        entry->InheritatedValue(value);  
        break;
    }

    InvokePropertyChanged(dependencyProperty, currentValue, value);
}

void Graphucks::DependencyObject::SetInheritanceContext( DependencyObject* inheritanceContext )
{
    m_inheritanceContext = inheritanceContext;
    m_inheritanceContextSet = true;

    for(auto& pair : m_valueEntries)
    {
        auto& dependencyProperty = pair.first;

        if(dependencyProperty->PropertyMetadata()->Inherits())
        {
            auto& entry = m_valueEntries[dependencyProperty];

            auto currentValue = entry->InheritatedValue();
            
            auto value = GetValue(dependencyProperty);

            if(AreValuesSame(dependencyProperty, currentValue, value))
                return;

            m_valueEntries[dependencyProperty]->InheritatedValue(value);

            InvokePropertyChanged(dependencyProperty, currentValue, value);
        }
    }

    m_inheritanceContext = inheritanceContext;
}

void DependencyObject::ClearValue( const DependencyPropertyPtr& dependencyProperty )
{
    auto entryIt = m_valueEntries.find(dependencyProperty);

    if(entryIt != end(m_valueEntries))
    {
       DependantValueEntryPtr& entry = (*entryIt).second;

       if(!entry->HasBaseValue())
       {
           return;
       }
       auto oldValue = GetValue(dependencyProperty);

       entry->BaseValue(nullptr);

       auto newValue = GetValue(dependencyProperty);

       if (AreValuesSame(dependencyProperty, oldValue, newValue))
       {
           return;
       }

       InvokePropertyChanged(dependencyProperty, oldValue, newValue);
    }
}

void DependencyObject::SetInheritedValue( const DependencyPropertyPtr& property, const SharedVoid& value )
{
    DependantValueEntryPtr entry;

    auto entryIt = m_valueEntries.find(property);

    if(entryIt != end(m_valueEntries))
    {
        auto& entry = (*entryIt).second;

        if(entry->IsAnimated() || entry->HasBaseValue())
        {
            entry->InheritatedValue(value);
            return;
        }
        
        SetValueImpl(property, value, ValueSource::Inherited);
    }
}

void DependencyObject::InvokePropertyChanged(const DependencyPropertyPtr& dependencyProperty, 
                                             const SharedVoid& oldValue, 
                                             const SharedVoid& newValue )
{
    auto changeCallback = dependencyProperty->PropertyMetadata()->ChangeCallback();
          
    auto args = PropertyChangedEventArgs(oldValue, newValue);

    OnPropertyChanged(dependencyProperty, args);

    if(changeCallback && dependencyProperty->IsOfOwnerType(this))
    {
        changeCallback(this, args);
    }
}

bool DependencyObject::AreValuesSame(const DependencyPropertyPtr& dependencyProperty,
                                     const SharedVoid& oldValue, 
                                     const SharedVoid& newValue )
{
    auto equalityCallback = dependencyProperty->PropertyMetadata()->EqualityCallback();
    if(equalityCallback)
    {
        /* check if one or the other is null first */
        if((!oldValue && newValue) || (oldValue && !newValue))
            return false;

        return equalityCallback(oldValue, newValue);
    }
    else
    {
        return (oldValue == newValue);
    }
}


void DependencyObject::OnPropertyChanged( const DependencyPropertyPtr&, const PropertyChangedEventArgs& )
{

}
