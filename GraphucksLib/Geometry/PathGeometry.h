#pragma once
#include "GeometryBase.h"
#include <vector>
#include "Figure.h"

namespace Graphucks
{
    class PathGeometry;
    typedef std::shared_ptr<PathGeometry> PathGeometryPtr;

    enum class FillMode
    {
        EvenOdd = 0,
        NonZero = 1
    };

    class PathGeometry : public GeometryBase
    {
    public:
        PathGeometry(void);
        virtual ~PathGeometry(void);

        void BeginFigure(const Point& start, bool filled, bool closed);
        void EndFigure();

        void LineTo(const Point& point, bool isStroked = true, bool isSmoothJoin = true);
        void ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize,  bool isStroked = true, bool isSmoothJoin = true);
        void BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked = true, bool isSmoothJoin = true);
        void QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked = true, bool isSmoothJoin = true);

        void FillMode(Graphucks::FillMode mode);
        Graphucks::FillMode FillMode();

        void Clear();
    protected:
       virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
    private:
        std::vector<FigurePtr> m_figures;
        std::unique_ptr<Figure> m_currentFigure;
        Graphucks::FillMode m_fillMode;
    };
}

