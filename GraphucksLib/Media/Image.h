#pragma once
#include <d2d1_1.h>
#include "..\Core\Resource.h"

namespace Graphucks
{
    class Image;
    typedef std::shared_ptr<Image> ImagePtr;

    class Image : public DeviceDependantResource
    {
    public:
        Image(){};
        virtual ~Image(){}
    };
}