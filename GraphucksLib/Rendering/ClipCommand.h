#pragma once
#include "RenderContext.h"
namespace Graphucks
{
    class ClipCommand
    {
    public:
        ClipCommand(bool clipEnabled, Graphucks::Rectangle* clipRect, IRenderContext* renderContext)
        {
            m_renderContext = renderContext;
            m_clipEnabled = clipEnabled;

            if(clipEnabled)
                m_renderContext->BeginClip(*clipRect);
        }

        ~ClipCommand()
        {
            if(m_clipEnabled)
                m_renderContext->EndClip();

            m_clipEnabled = false;
        }
    private:
        bool            m_clipEnabled;
        IRenderContext* m_renderContext;
    };
}