#include "pch.h"
#include "DrawTextCommand.h"

using namespace Graphucks;
using namespace D2D1;

DrawTextCommand::DrawTextCommand(void)
{
}


DrawTextCommand::~DrawTextCommand(void)
{
}

void DrawTextCommand::Run()
{
    auto layout    = GetResourceFrom<IDWriteTextLayout>(m_textLayout);
    auto textBrush = GetResourceFrom<ID2D1Brush>(m_brush, m_deviceContext.Get());

    auto originalTransform = m_brush->Transform();

    auto layoutBounds = m_textLayout->GetLayoutBounds();

    layoutBounds.top    += m_origin.y;
    layoutBounds.left   += m_origin.x;
    layoutBounds.bottom += m_origin.y;
    layoutBounds.right  += m_origin.x;

    auto boundsTransform = m_brush->GetTransformToBounds(layoutBounds);

    boundsTransform = boundsTransform * originalTransform;

    m_brush->Transform(boundsTransform);

    m_deviceContext->DrawTextLayout(m_origin, layout, textBrush);

    m_brush->Transform(originalTransform);
}
