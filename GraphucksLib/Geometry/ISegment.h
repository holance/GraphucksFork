#pragma once
#include "GeometryBase.h"

namespace Graphucks
{
    class Figure;
    
    class ISegment
    {
    public:
        ISegment() : m_isDirty(false){}
        virtual ~ISegment(){}
    protected:
        virtual void Create(const ID2D1GeometrySinkPtr& sink) = 0;
    private:
        bool m_isDirty ;
    protected:
        void Invalidate()
        {
            m_isDirty = true;
        }
        auto IsValid() -> bool
        {
            return !m_isDirty;
        }
    friend Figure;
    };
}
