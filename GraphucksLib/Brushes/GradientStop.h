#pragma once
#include "..\Core\GraphicsTypes.h"

namespace Graphucks
{
    struct GradientStop
    {
        GradientStop(float position, Color c): Position(position), C(c){}
        GradientStop() : Position(0) {}
        float Position;
        Color C;
    };
}
 