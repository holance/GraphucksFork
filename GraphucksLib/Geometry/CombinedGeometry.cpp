#include "pch.h"
#include "CombinedGeometry.h"
#include "Utils.h"

using namespace Graphucks;
using namespace Microsoft::WRL;

Microsoft::WRL::ComPtr<ID2D1Geometry> CombinedGeometry::OnCreateGeometry()
{
    ComPtr<ID2D1PathGeometry> path;
    ComPtr<ID2D1GeometrySink> sink;

    Check(m_Factory->CreatePathGeometry(&path));
    Check(path->Open(&sink));

    auto g1 = GetResourceFrom<ID2D1Geometry>(m_Geometry1);
    auto g2 = GetResourceFrom<ID2D1Geometry>(m_Geometry2);

    Check(g1->CombineWithGeometry(g2, m_Mode, nullptr/*optional transform*/, sink.Get()));
    Check(sink->Close());

    return path;
}
