#pragma once
#include <Graphucks.h>

class XamlVectorParser
{

public:
    static Graphucks::VisualElementPtr ParseXaml(Microsoft::WRL::ComPtr<IStream>& xmlStream);
public:
    XamlVectorParser(void);
    virtual ~XamlVectorParser(void);
};

