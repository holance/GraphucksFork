#include "pch.h"
#include "PathGeometry.h"
#include "LineSegment.h"
#include "ArcSegment.h"
#include "BezierSegment.h"
#include "QuadraticBezierSegment.h"
#include "..\Core\Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;
using namespace std;

PathGeometry::PathGeometry(void) : m_fillMode(Graphucks::FillMode::EvenOdd)
{
}


PathGeometry::~PathGeometry(void)
{
}

Microsoft::WRL::ComPtr<ID2D1Geometry> PathGeometry::OnCreateGeometry()
{
    ComPtr<ID2D1PathGeometry> pathGeometry;
    ComPtr<ID2D1GeometrySink> geometrySink;

    m_Factory->CreatePathGeometry(&pathGeometry);

    pathGeometry->Open(&geometrySink);
    geometrySink->SetFillMode((D2D1_FILL_MODE)m_fillMode);

    for (auto figure : m_figures)
    {
        figure->Create(geometrySink);
    }

    HRESULT hr = geometrySink->Close();
    Check(hr);
    return pathGeometry;
}

void PathGeometry::Clear()
{
    m_figures.clear();
    m_currentFigure = nullptr;
    Invalidate();
}

void PathGeometry::LineTo(const Point& point, bool isStroked, bool isSmoothJoin)
{
    if(!m_currentFigure)
        throw std::logic_error("BeginFigure not called");

    m_currentFigure->AddSegment(make_shared<LineSegment>(point), isStroked, isSmoothJoin);
}

void PathGeometry::ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked, bool isSmoothJoin)
{
    if(!m_currentFigure)
        throw std::logic_error("BeginFigure not called");

    m_currentFigure->AddSegment(make_shared<ArcSegment>(point, size, rotation, sweepDirection, arcSize), isStroked, isSmoothJoin);
}

void PathGeometry::BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked, bool isSmoothJoin)
{
    if(!m_currentFigure)
        throw std::logic_error("BeginFigure not called");

    m_currentFigure->AddSegment(make_shared<BezierSegment>(p1, p2, p3), isStroked, isSmoothJoin);
}

void PathGeometry::QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin)
{
    if(!m_currentFigure)
        throw std::logic_error("BeginFigure not called");

    m_currentFigure->AddSegment(make_shared<QuadraticBezierSegment>(point1, point2), isStroked, isSmoothJoin);
}

void PathGeometry::BeginFigure(const Point& start, bool filled,  bool closed)
{
    if(m_currentFigure)
        throw std::logic_error("EndFigure not called");

    m_currentFigure = unique_ptr<Figure>(new Figure(start, filled, closed));
}

void PathGeometry::EndFigure()
{
    if(!m_currentFigure)
        throw std::logic_error("BeginFigure not called");

    m_figures.push_back(move(m_currentFigure));
    
    Invalidate();
}

void PathGeometry::FillMode( Graphucks::FillMode mode )
{
    if(m_fillMode == mode)
        return;

    m_fillMode = mode;
   // Invalidate();
}

Graphucks::FillMode PathGeometry::FillMode()
{
    return m_fillMode;
}
