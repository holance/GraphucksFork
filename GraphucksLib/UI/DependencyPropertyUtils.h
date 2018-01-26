#pragma once

#define DP_PROPERTY(propertyName) public: static const DependencyPropertyPtr propertyName##Property; \

#define DP_PROPERTY_WITH_CALLBACK(propertyName) \
        DP_PROPERTY(propertyName)\
        private:\
            propertyName##Changed(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs) \

#define DP_PROPERTY_IMPL(propertyName, ownerType, dpType, defaultValue, affectsArgs)\
    const DependencyPropertyPtr ownerType##::propertyName##Property = DependencyProperty::Register<ownerType##/*ownerType*/>(L"name##", \
                                                                                                  FrameworkPropertyMetadata::Create<dpType##/*dp type*/>(\
                                                                                                  defaultValue, \
                                                                                                  affectsArgs##,\
                                                                                                  nullptr)\
                                                                                               ); \

    #define DP_PROPERTY_IMPL_WITH_CALLBACK(propertyName, ownerType, dpType, defaultValue, affectsArgs)\
    const DependencyPropertyPtr ownerType##::propertyName##Property = DependencyProperty::Register<ownerType##/*ownerType*/>(L"name##", \
                                                                                                  FrameworkPropertyMetadata::Create<dpType##/*dp type*/>(\
                                                                                                  defaultValue, \
                                                                                                  affectsArgs##,\
                                                                                                  &propertyName##Changed)\
                                                                                               );