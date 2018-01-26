#pragma once
#include "Resource.h"

namespace Graphucks
{
    class Transform;
    typedef std::shared_ptr<Transform> TransformPtr;

    class Transform : public DependantObject
    {
    public:
        Transform(void);
        virtual ~Transform(void);
    public:
        virtual operator D2D1::Matrix3x2F() = 0;

        virtual auto GetTransformMatrix() -> D2D1::Matrix3x2F& = 0;
    };
}
