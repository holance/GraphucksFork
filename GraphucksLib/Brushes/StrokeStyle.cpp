#include "pch.h"
#include "StrokeStyle.h"
#include <wrl.h>
#include "..\Rendering\RenderContext.h"

using namespace Microsoft::WRL;
using namespace Graphucks;
using namespace D2D1;

StrokeStyle::StrokeStyle(void) : m_startCap   (CapStyle::Flat), 
                                 m_endCap     (CapStyle::Flat), 
                                 m_dashCap    (CapStyle::Flat), 
                                 m_lineJoin   (LineJoin::Miter),
                                 m_dashStyle  (DashStyle::Solid),
                                 m_miterLimit (0),
                                 m_dashOffset (0), 
                                 m_factory    (RenderContext::GetFactory())
{
}


StrokeStyle::~StrokeStyle(void)
{
}

auto StrokeStyle::OnCreateResource() -> ComPtr<IUnknown> 
{
    ComPtr<ID2D1StrokeStyle> strokeStyle;

    HRESULT hr = S_OK;
    hr = m_factory->CreateStrokeStyle(StrokeStyleProperties((D2D1_CAP_STYLE)m_startCap, 
                                                            (D2D1_CAP_STYLE)m_endCap, 
                                                            (D2D1_CAP_STYLE)m_dashCap, 
                                                            (D2D1_LINE_JOIN)m_lineJoin, 
                                                            m_miterLimit, 
                                                            (D2D1_DASH_STYLE)m_dashStyle, 
                                                            m_dashOffset), 
                                                            m_dashes.empty() ? nullptr : &m_dashes[0], 
                                                            (UINT32)m_dashes.size(),
                                                            strokeStyle.GetAddressOf());
    if(FAILED(hr))
        throw std::exception("Failed to create StrokeStyle");

    return strokeStyle;
}

CapStyle StrokeStyle::StartCap() const
{
    return m_startCap;
}

void StrokeStyle::StartCap(CapStyle val)
{
    m_startCap = val;
    Invalidate();
}

CapStyle StrokeStyle::EndCap() const
{
    return m_endCap;
}

void StrokeStyle::EndCap(CapStyle val)
{
    m_endCap = val;
    Invalidate();
}

CapStyle StrokeStyle::DashCap() const
{
    return m_dashCap;
}

void StrokeStyle::DashCap(CapStyle val)
{
    m_dashCap = val;
    Invalidate();
}

LineJoin StrokeStyle::LineJoinStyle() const
{
    return m_lineJoin;
}

void StrokeStyle::LineJoinStyle(LineJoin val)
{
    m_lineJoin = val;
    Invalidate();
}

float StrokeStyle::MiterLimit() const
{
    return m_miterLimit;
}

DashStyle StrokeStyle::LineDashStyle() const
{
    return m_dashStyle;
}

float StrokeStyle::DashOffset() const
{
    return m_dashOffset;
}

void StrokeStyle::DashOffset(float val)
{
    m_dashOffset = val;
    Invalidate();
}

std::vector<float> StrokeStyle::Dashes() const
{
    return m_dashes;
}

void StrokeStyle::Dashes(std::vector<float> val)
{
    m_dashes = val;
    Invalidate();
}
