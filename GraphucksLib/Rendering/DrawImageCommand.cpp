#include "pch.h"
#include "DrawImageCommand.h"
#include "..\Media\Bitmap.h"

using namespace Graphucks;
using namespace D2D1;

DrawImageCommand::DrawImageCommand() : m_compositeMode(EffectCompositeMode::SourceOver)
{
}


DrawImageCommand::~DrawImageCommand(void)
{
}

void DrawImageCommand::Run()
{
    auto bmp = GetResourceFrom<ID2D1Bitmap1>(m_bitmap, m_deviceContext);
    auto bitmapSize = m_bitmap->GetSize();

    Matrix3x2F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);

    D2D1_RECT_F imageBounds;
    m_deviceContext->GetImageLocalBounds(bmp, &imageBounds);

    Matrix3x2F newTrans =  Matrix3x2F::Scale(m_targetSize.width/bitmapSize.width, 
                                             m_targetSize.height/bitmapSize.height, 
                                             Point(0, 0));

    newTrans = newTrans * Matrix3x2F::Translation(imageBounds.left, imageBounds.top);

    newTrans = newTrans * oldTransform;

    m_deviceContext->SetTransform(newTrans);

    m_deviceContext->DrawImage(bmp, 
                               Point(0, 0), 
                               imageBounds,
                               D2D1_INTERPOLATION_MODE_LINEAR, 
                               (D2D1_COMPOSITE_MODE)m_compositeMode);

    m_deviceContext->SetTransform(oldTransform);
}
