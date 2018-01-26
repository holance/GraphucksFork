#include "pch.h"
#include "DependencyProperty.h"

using namespace Graphucks;

DependencyProperty::DependencyProperty(const std::wstring& name, 
                                       const IsOfOwnerTypeCallback ofOwnerTypeCallback,
                                       const FrameworkPropertyMetadataPtr& propertyMetadata) :
                                            m_name(name),  
                                            m_propertyMetadata(propertyMetadata),
                                            m_isofOwnerType(ofOwnerTypeCallback)
{
}


DependencyProperty::~DependencyProperty(void)
{
}

DependencyPropertyPtr Graphucks::DependencyProperty::Register( const std::wstring& name,
                                                               const IsOfOwnerTypeCallback ofOwnerTypeCallback,
                                                               const FrameworkPropertyMetadataPtr& propertyMetadata )
{
    return std::shared_ptr<DependencyProperty>(new DependencyProperty(name, ofOwnerTypeCallback, propertyMetadata));
}
