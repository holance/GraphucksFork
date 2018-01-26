#pragma once
#include "DrawCommand.h"

namespace Graphucks
{
    class FillCommand: public DrawCommand
    {
    public:
        FillCommand(){}
        virtual ~FillCommand(){}
    public:
        virtual void Run();
    };
}
