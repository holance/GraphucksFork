#pragma once
#include "..\Rendering\DrawCommand.h"
#include "..\Brushes\Pen.h"

namespace Graphucks
{
    class StrokeCommand: public DrawCommand
    {
    public:
        StrokeCommand(){}
        virtual ~StrokeCommand(){}
    public:
        virtual void Run();
        void SetPen(const PenPtr& pen);
    private:
        PenPtr m_pen;
    };
}
