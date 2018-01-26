#pragma once
#include "..\Core\Resource.h"

namespace Graphucks
{
    enum class CapStyle
    {
        Flat     = 0,
        Square   = 1,
        Round    = 2,
        Triangle = 3
    };

    enum class LineJoin
    {
        Miter        = 0,
        Bevel        = 1,
        Round        = 2,
        MiterOrBevel = 3
    };

    enum class DashStyle
    {
        Solid         = 0,
        Dash          = 1,
        Dot           = 2,
        DashDot       = 3,
        DashDotDot    = 4,
        Custom        = 5
    };

    class StrokeStyle;
    typedef std::shared_ptr<StrokeStyle> StrokeStylePtr;

    class StrokeStyle : public IndependantResource
    {
    typedef Microsoft::WRL::ComPtr<ID2D1Factory> ID2D1FactoryPtr;

    public:
        StrokeStyle(void);
        virtual ~StrokeStyle(void);
    public:
        Graphucks::CapStyle StartCap() const;
        void StartCap(Graphucks::CapStyle val);

        Graphucks::CapStyle EndCap() const;
        void EndCap(Graphucks::CapStyle val);

        Graphucks::CapStyle DashCap() const;
        void DashCap(Graphucks::CapStyle val);

        LineJoin LineJoinStyle() const;
        void LineJoinStyle(LineJoin val);

        float MiterLimit() const;
        void MiterLimit(float val) { m_miterLimit = val; }

        DashStyle LineDashStyle() const;
        void LineDashStyle(DashStyle val) { m_dashStyle = val; }

        float DashOffset() const;
        void DashOffset(float val);

        std::vector<float> Dashes() const;
        void Dashes(std::vector<float> val);
    protected:
        virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource() override;
    private:
        CapStyle             m_startCap;
        CapStyle             m_endCap;
        CapStyle             m_dashCap;
        LineJoin             m_lineJoin;
        float                m_miterLimit;
        DashStyle            m_dashStyle;
        float                m_dashOffset;
        std::vector<float>   m_dashes;
        ID2D1FactoryPtr      m_factory;
    };
}


