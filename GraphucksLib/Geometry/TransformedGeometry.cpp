#include "pch.h"
#include "TransformedGeometry.h"

using namespace std;
using namespace Graphucks;
using namespace Microsoft::WRL;

TransformedGeometry::TransformedGeometry(const GeometryPtr& geometry, const TransformPtr& transform) : m_originalGeometry(geometry), m_transform(transform)
{
}

TransformedGeometry::~TransformedGeometry(void)
{
}

Microsoft::WRL::ComPtr<ID2D1Geometry> TransformedGeometry::OnCreateGeometry()
{
    ComPtr<ID2D1TransformedGeometry> transformedGeometry;
    auto matrix = m_transform->GetTransformMatrix();

    auto g1 = GetResourceFrom<ID2D1Geometry>(m_originalGeometry);

    HRESULT hr = m_Factory->CreateTransformedGeometry(g1, &matrix, transformedGeometry.ReleaseAndGetAddressOf());
    m_originalGeometry = nullptr;
    return transformedGeometry;
}