#include "pch.h"
#include "BorderElement.h"

using namespace Graphucks;
using namespace std;

struct RadiusHelper
{
    float LeftTop; 
    float TopLeft;
    float TopRight; 
    float RightTop;
    float RightBottom;
    float BottomRight;
    float BottomLeft; 
    float LeftBottom;

    RadiusHelper(CornerRadius radii, Thickness borders)
    {
        float left   = borders.Left   * 0.5f;
        float top    = borders.Top    * 0.5f;
        float right  = borders.Right  * 0.5f;
        float bottom = borders.Bottom * 0.5f;
       
        if (0 == radii.TopLeft)
        {
            LeftTop = 0.0f;
            TopLeft = 0.0f; 
        }
        else 
        { 
            LeftTop = radii.TopLeft + left;
            TopLeft = radii.TopLeft + top; 
        }
        if (0 == radii.TopRight)
        {
            TopRight = 0.0f;
            RightTop = 0.0f;
        }
        else 
        { 
            TopRight = radii.TopRight + top;
            RightTop = radii.TopRight + right; 
        }
        if (0 == radii.BottomRight)
        {
            RightBottom = 0.0f;
            BottomRight = 0.0f; 
        }
        else 
        { 
            RightBottom = radii.BottomRight + right;
            BottomRight = radii.BottomRight + bottom; 
        }
        if (0 == radii.BottomLeft)
        {
            BottomLeft = 0.0f;
            LeftBottom = 0.0f; 
        }
        else 
        { 
            BottomLeft = radii.BottomLeft + bottom;
            LeftBottom = radii.BottomLeft + left; 
        }
    }
};

const DependencyPropertyPtr BorderElement::BackgroundProperty = DependencyProperty::Register<BorderElement/*ownerType*/>(L"Background", 
                                                                                              FrameworkPropertyMetadata::Create<BrushPtr/*dp type*/>(
                                                                                                  nullptr, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender,
                                                                                                  nullptr)
                                                                                               );

const DependencyPropertyPtr BorderElement::BorderBrushProperty = DependencyProperty::Register<BorderElement/*ownerType*/>(L"Foreground", 
                                                                                              FrameworkPropertyMetadata::Create<BrushPtr/*dp type*/>(
                                                                                                  nullptr, 
                                                                                                  (int)FrameworkPropertyMetadataOptions::AffectsRender,
                                                                                                  nullptr)
                                                                                               );

BorderElement::BorderElement(void)
{
    m_borderThickness.Left = 3;
    m_borderThickness.Top = 3;
    m_borderThickness.Right = 3;
    m_borderThickness.Bottom = 3;
    m_pen = make_shared<Pen>();
}


BorderElement::~BorderElement(void)
{
}

void BorderElement::RenderOverride(const IRenderContextPtr& renderContext)
{
     auto renderSize = GetRenderSize();

    if((!m_pathGeometry && m_geometries.empty()) 
      || m_lastSize != renderSize)
        CreateGeometry();
   
    m_lastSize = renderSize;

    m_pen->Brush(BorderBrush());

    if(m_pathGeometry)
    {
         m_pen->Thickness(m_borderThickness.Top);
         renderContext->DrawGeometry(m_pathGeometry, Background(), m_pen);
    }
    //else if(!m_geometries.empty())
    //{
    //    m_pen->Thickness(m_borderThickness.Top);
    //    renderContext->DrawGeometry(m_geometries[0], nullptr, m_pen);

    //    m_pen->Thickness(m_borderThickness.Right);
    //    renderContext->DrawGeometry(m_geometries[1], nullptr, m_pen);

    //    m_pen->Thickness(m_borderThickness.Bottom);
    //    renderContext->DrawGeometry(m_geometries[2], nullptr, m_pen);

    //    m_pen->Thickness(m_borderThickness.Left);
    //    renderContext->DrawGeometry(m_geometries[3], nullptr, m_pen);
    //}

    return;
}

Graphucks::Size BorderElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    int childCount = GetVisualChildCount();

    if(childCount == 0)
        return Graphucks::Size(0, 0);

    auto borderSize =  Size(m_borderThickness.Left + m_borderThickness.Right, 
                            m_borderThickness.Top  + m_borderThickness.Bottom);
                       
    auto paddingSize = Size(m_padding.Left + m_padding.Right, 
                            m_padding.Top  + m_padding.Bottom);

    Size returnSize;

    if(GetVisualChildCount() > 0)
    {
         auto totalMargin     = Size(borderSize.width  + paddingSize.width, 
                                     borderSize.height + paddingSize.height); 
              
         auto childAvailable  = Size(max(0.0f, availableSize.width  - totalMargin.width),
                                     max(0.0f, availableSize.height - totalMargin.height)); 

         auto visualChild = GetVisualChild(0);

         visualChild->Measure(childAvailable);

         auto childMeasure = visualChild->DesiredSize(); 

         returnSize.width  = childMeasure.width  + totalMargin.width;
         returnSize.height = childMeasure.height + totalMargin.height; 
    }
    else
    {
        returnSize = Size(borderSize.width  + paddingSize.width, 
                          borderSize.height + paddingSize.height);
    }

    return returnSize;
}

Graphucks::Size BorderElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
    int childCount = GetVisualChildCount();

    if(childCount == 0)
        return finalSize;
 
    auto visualChild = GetVisualChild(0);

    auto elementRect = Graphucks::Rectangle(0, 0, finalSize.width, finalSize.height);
    Rectangle contentRect;

    contentRect.left   =  elementRect.left + m_borderThickness.Left;
    contentRect.top    =  elementRect.top  + m_borderThickness.Top;
    contentRect.right  =  max(0.0f, elementRect.right  - m_borderThickness.Left);
    contentRect.bottom =  max(0.0f, elementRect.bottom - m_borderThickness.Top);
 
    contentRect.left   =  contentRect.left + m_padding.Left;
    contentRect.top    =  contentRect.top  + m_padding.Top;
    contentRect.right  =  max(0.0f, contentRect.right  - m_padding.Left);
    contentRect.bottom =  max(0.0f, contentRect.bottom - m_padding.Top);


    visualChild->Arrange(contentRect);

    return finalSize;
}

void BorderElement::CreateGeometry()
{
    auto renderSize = GetRenderSize();

    auto radii = RadiusHelper(m_cornerRadius, m_borderThickness);
    auto rect = Rectangle(0, 0, renderSize.width, renderSize.height);

    rect.right -= m_borderThickness.Top * 0.5f;
    rect.left += m_borderThickness.Left * 0.5f;
    rect.bottom -= m_borderThickness.Bottom * 0.5f;
    rect.top += m_borderThickness.Top * 0.5f;

    auto topLeft     =  Point(radii.LeftTop, 0);
    auto topRight    =  Point(rect.Width() - radii.RightTop, 0);
    auto rightTop    =  Point(rect.Width(), radii.TopRight); 
    auto rightBottom =  Point(rect.Width(), rect.Height() - radii.BottomRight);
    auto bottomRight =  Point(rect.Width() - radii.RightBottom, rect.Height()); 
    auto bottomLeft  =  Point(radii.LeftBottom, rect.Height()); 
    auto leftBottom  =  Point(0, rect.Height() - radii.BottomLeft);
    auto leftTop     =  Point(0, radii.TopLeft); 

    auto offset  = rect.Location();
    topLeft      += offset;
    topRight     += offset; 
    rightTop     += offset;
    rightBottom  += offset;
    bottomRight  += offset;
    bottomLeft   += offset;
    leftBottom   += offset;
    leftTop      += offset;

    m_geometries.clear();
    m_pathGeometry = nullptr;

    if(true)
    {
        /* TOP */
        m_pathGeometry = make_shared<PathGeometry>();
        m_pathGeometry->BeginFigure(topLeft, true, true);

        m_pathGeometry->LineTo(topRight);
 
        /* TOP RIGHT CORNER */
        float radiusX = rect.right - topRight.x;
        float radiusY = rightTop.y - rect.top; 
        if (0 != radiusX || 0 != radiusY)
        {
            m_pathGeometry->ArcTo(rightTop, 
                                  Size(radiusX, radiusY), 
                                  0, 
                                  SweepDirection::CounterClockwise, 
                                  ArcSize::Small); 
        }
 
        /* RIGHT */
        m_pathGeometry->LineTo(rightBottom);
 
        /* BOTTOM RIGHT CORNER */
        radiusX = rect.right - bottomRight.x;
        radiusY = rect.bottom - rightBottom.y;
        if (0 != radiusX || 0 != radiusY)
        { 
            m_pathGeometry->ArcTo(bottomRight, 
                                  Size(radiusX, radiusY), 
                                  0, 
                                  SweepDirection::CounterClockwise, 
                                  ArcSize::Small); 
        }
 
        /* BOTTOM */
        m_pathGeometry->LineTo(bottomLeft);

        /* BOTTOM LEFT CORNER */
        radiusX = bottomLeft.x - rect.left;
        radiusY = rect.bottom - leftBottom.y; 
        if (0 != radiusX || 0 != radiusY)
        { 
            m_pathGeometry->ArcTo(leftBottom, 
                                  Size(radiusX, radiusY), 
                                  0, 
                                  SweepDirection::CounterClockwise, 
                                  ArcSize::Small);
        }

        /* LEFT */
        m_pathGeometry->LineTo(leftTop);
 
        /* TOP LEFT CORNER */
        radiusX = topLeft.x - rect.left;
        radiusY = leftTop.y - rect.top; 
        if (0 != radiusX || 0 != radiusY)
        {
            m_pathGeometry->ArcTo(topLeft, 
                                  Size(radiusX, radiusY), 
                                  0, 
                                  SweepDirection::CounterClockwise, 
                                  ArcSize::Small); 
        }

        m_pathGeometry->EndFigure();
    }
}

void BorderElement::BorderBrush(const BrushPtr& borderBrush)
{
    SetValue(BorderBrushProperty, borderBrush);
}

const BrushPtr BorderElement::BorderBrush()
{
    return GetValue<Brush>(BorderBrushProperty);
}

void BorderElement::Background(const BrushPtr& backgroundBrush)
{
    SetValue(BackgroundProperty, backgroundBrush);
}

const BrushPtr BorderElement::Background()
{
    return GetValue<Brush>(BackgroundProperty);
}

void BorderElement::SetContent(const VisualElementPtr& content)
{
    auto childCount = GetVisualChildCount();

    if(childCount > 0)
        RemoveAllChildren();

    AddVisualChild(content);
}

void BorderElement::Padding(const Graphucks::Thickness& thickness)
{
    m_padding = thickness;
}

auto BorderElement::CornerRadii() const -> Graphucks::CornerRadius 
{
    return m_cornerRadius;
}

void BorderElement::CornerRadii(Graphucks::CornerRadius val)
{
    m_cornerRadius = val;
}

auto BorderElement::BorderThickness() const -> float
{
    return m_borderThickness.Left;
}

void BorderElement::BorderThickness(float val)
{
    m_borderThickness.Left = val;
    m_borderThickness.Top = val;
    m_borderThickness.Right = val;
    m_borderThickness.Bottom = val;
}

const Thickness BorderElement::Padding()
{
    return m_padding;
}
