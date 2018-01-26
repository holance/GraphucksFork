#include "pch.h"
#include "PathElement.h"
#include "..\Geometry\PathGeometry.h"
#include "..\Geometry\TransformedGeometry.h"
#include <sstream>
using namespace Graphucks;
using namespace std;

class PathParser
{
    private:
        wstring         m_pathData; 
        size_t          m_currentPosition;
        bool            m_hasFigureBegan;
        Point           m_lastStartingPoint;
        Point           m_lastPoint;
        Point           m_secondToLastPoint;
        wchar_t          m_currentCommand;
        PathGeometryPtr  m_pathGeometry;
    private:
        bool HasMoreToRead()
        { 
            return m_currentPosition < m_pathData.size();
        }

        bool SkipWhiteSpace(bool commaPermitted)
        { 
            bool foundComma = false; 

            while (HasMoreToRead()) 
            {
                wchar_t ch = m_pathData[m_currentPosition];

                switch (ch) 
                {
                case ' ' : 
                case '\n': 
                case '\r':
                case '\t':
                    break;
                case ',':
                    if (commaPermitted) 
                    {
                        foundComma = true; 
                        commaPermitted = false;
                    }
                    else 
                        throw exception("parsing error");
                    break; 

                default: 
                    if (!iswspace(ch))
                        return foundComma;
                } 

                m_currentPosition++; 
            } 

            return foundComma; 
        }

        bool ReadNextToken() 
        {
            SkipWhiteSpace(false); 

            if (HasMoreToRead())
            { 
                m_currentCommand = m_pathData[m_currentPosition++];
 
                return true; 
            }
            else 
            {
                return false;
            }
        } 

        bool IsNumeric(bool commaPermitted) 
        { 
            bool commaFound = SkipWhiteSpace(commaPermitted);
 
            if(HasMoreToRead())
            {
                m_currentCommand = m_pathData[m_currentPosition];
 
                if ((m_currentCommand == '.') || 
                    (m_currentCommand == '-') || 
                    (m_currentCommand == '+') || 
                    ((m_currentCommand >= '0') && (m_currentCommand <= '9')))
                { 
                    return true;
                }
            }
 
            if (commaFound)
            { 
                throw exception("parsing error");
            }
 
            return false;
        }

        void SkipDigits(bool signPermitted) 
        {
            if (signPermitted && HasMoreToRead() && 
               ((m_pathData[m_currentPosition] == '-') || m_pathData[m_currentPosition] == '+')) 
            {
                m_currentPosition++; 
            }

            while (HasMoreToRead() && (m_pathData[m_currentPosition] >= '0') && 
                  (m_pathData[m_currentPosition] <= '9'))
            { 
                m_currentPosition++;
            } 
        } 

        float ParseNumber(bool commaPermitted)
        {
            if (!IsNumeric(commaPermitted)) 
                throw exception("bad token");

            bool simple = true; 
            int start = m_currentPosition;

            if (HasMoreToRead() && ((m_pathData[m_currentPosition] == '-') || m_pathData[m_currentPosition] == '+')) 
            {
                m_currentPosition++;
            }
 
            SkipDigits(false); 

            if (HasMoreToRead() && (m_pathData[m_currentPosition] == '.'))
            { 
                simple = false;
                m_currentPosition++; 
                SkipDigits(false); 
            }
 
            if (HasMoreToRead() && ((m_pathData[m_currentPosition] == 'E') || 
               (m_pathData[m_currentPosition] == 'e')))
            {
                simple = false; 
                m_currentPosition++;
                SkipDigits(true); 
            } 
 
            if (simple && (m_currentPosition <= (start + 8)))
            {
                int sign = 1;
 
                if (m_pathData[start] == '+')
                { 
                    start++; 
                }
                else if (m_pathData[start] == '-') 
                {
                    start++;
                    sign = -1;
                } 

                int newVal = 0; 
 
                while (start < m_currentPosition)
                { 
                    newVal = newVal * 10 + (m_pathData[start] - '0');
                    start++;
                }
 
                return newVal * sign;
            } 
            else 
            {
                wstring subString = m_pathData.substr(start, m_currentPosition - start); 
                return _wtof(subString.c_str());
            }
        }

        bool ParseBoolean()
        { 
            SkipWhiteSpace(true);

            if (HasMoreToRead())
            { 
                m_currentCommand = m_pathData[m_currentPosition ++];
 
                if (m_currentCommand == '0') 
                {
                    return false; 
                }
                else if (m_currentCommand == '1')
                {
                    return true; 
                }
            } 
 
            throw exception("parse error");
        }
    
        Point ParsePoint(wchar_t cmd, bool commaPermitted)
        { 
            float x = ParseNumber(commaPermitted);
            float y = ParseNumber(true);

            if (cmd >= 'a')
            {
                x += m_lastPoint.x; 
                y += m_lastPoint.y; 
            }
 
            return Point(x, y);
        }

        Point ReflectPoint()
        { 
            return Point(2 * m_lastPoint.x - m_secondToLastPoint.x,
                         2 * m_lastPoint.y - m_secondToLastPoint.y);
        }
 
        void EnsureFigure()
        { 
            if (!m_hasFigureBegan) 
            {
                m_pathGeometry->BeginFigure(m_lastStartingPoint, true, true);
                m_hasFigureBegan = true;
            }
        }

    public:
        void ParseToGeometryContext(const PathGeometryPtr& pathGeometry, const wstring& pathString)
        {
            m_pathGeometry = pathGeometry;
            m_pathData = pathString; 
            
            m_secondToLastPoint = Point(0, 0); 
            m_lastPoint = Point(0, 0);
            m_lastStartingPoint = Point(0, 0); 
 
            m_hasFigureBegan = false;
 
            bool first = true;

            wchar_t lastCommandToken = ' ';
            Graphucks::FillMode fillMode;

            while (ReadNextToken())
            { 
                wchar_t cmd = m_currentCommand; 

                if (first) 
                {
                    if ((cmd != 'M') && (cmd != 'm') && (cmd != 'F') && (cmd != 'f'))
                    {
                        throw exception("parse error");
                    }
 
                    first = false; 
                }
 
                switch (cmd)
                {
                case 'f': case 'F':
                    {
                        float fillType = ParseNumber(false);
                        FillMode fillMode = ( fillType == 0 ) ? FillMode::EvenOdd : FillMode::NonZero;
                        m_pathGeometry->FillMode(fillMode);
                    }
                    break;
                case 'm': case 'M':
               
                    m_lastPoint = ParsePoint(cmd, false);
 
                    if(m_hasFigureBegan)
                        pathGeometry->EndFigure();

                    pathGeometry->BeginFigure(m_lastPoint, true, false); 
                    m_hasFigureBegan = true;
                    m_lastStartingPoint = m_lastPoint; 
                    lastCommandToken = 'M';

                    while (IsNumeric(true))
                    { 
                        m_lastPoint = ParsePoint(cmd, false);
 
                        pathGeometry->LineTo(m_lastPoint, true, false); 
                        lastCommandToken = 'L';
                    } 
                    break;

                case 'l': case 'L':
                case 'h': case 'H': 
                case 'v': case 'V':
                    EnsureFigure(); 
 
                    do
                    { 
                        switch (cmd)
                        {
                            case 'l': m_lastPoint    = ParsePoint(cmd, false); break;
                            case 'L': m_lastPoint    = ParsePoint(cmd, false); break; 
                            case 'h': m_lastPoint.x += ParseNumber(false); break;
                            case 'H': m_lastPoint.x  = ParseNumber(false); break; 
                            case 'v': m_lastPoint.y += ParseNumber(false); break; 
                            case 'V': m_lastPoint.y  = ParseNumber(false); break;
                        } 

                        pathGeometry->LineTo(m_lastPoint, true, false);
                    }
                    while (IsNumeric(true)); 

                    lastCommandToken = 'L'; 
                    break; 

                case 'c': case 'C':
                case 's': case 'S':
                    EnsureFigure();

                    do 
                    {
                        Point p; 
 
                        if ((cmd == 's') || (cmd == 'S'))
                        { 
                            if (lastCommandToken == 'C')
                            {
                                p = ReflectPoint();
                            } 
                            else
                            { 
                                p = m_lastPoint; 
                            }
 
                            m_secondToLastPoint = ParsePoint(cmd, false);
                        }
                        else
                        { 
                            p = ParsePoint(cmd, false);
 
                            m_secondToLastPoint = ParsePoint(cmd, true); 
                        }
 
                        m_lastPoint = ParsePoint(cmd, true);

                        pathGeometry->BezierTo(p, m_secondToLastPoint, m_lastPoint, true, false);
 
                        lastCommandToken = 'C';
                    } 
                    while (IsNumeric(true)); 

                    break; 

                case 'q': case 'Q':
                case 't': case 'T':
                    EnsureFigure(); 

                    do 
                    { 
                        if ((cmd == 't') || (cmd == 'T'))
                        { 
                            if (lastCommandToken == 'Q')
                            {
                                m_secondToLastPoint = ReflectPoint();
                            } 
                            else
                            { 
                                m_secondToLastPoint = m_lastPoint; 
                            }
 
                            m_lastPoint = ParsePoint(cmd, false);
                        }
                        else
                        { 
                            m_secondToLastPoint = ParsePoint(cmd, false);
                            m_lastPoint = ParsePoint(cmd, true); 
                        } 

                        pathGeometry->QuadraticBezierTo(m_secondToLastPoint, m_lastPoint, true, false); 

                        lastCommandToken = 'Q';
                    }
                    while (IsNumeric(true)); 

                    break; 
 
                case 'a': case 'A':
                    EnsureFigure(); 

                    do
                    {
                        float w        = ParseNumber(false);
                        float h        = ParseNumber(true); 
                        float rotation = ParseNumber(true); 
                        bool  large    = ParseBoolean();
                        bool  sweep    = ParseBoolean(); 

                        m_lastPoint = ParsePoint(cmd, true);

                        pathGeometry->ArcTo(m_lastPoint, 
                                            Size(w, h), 
                                            rotation, 
                                            sweep ? SweepDirection::Clockwise : SweepDirection::CounterClockwise, 
                                            large ? ArcSize::Large : ArcSize::Small); 
                    }
                    while (IsNumeric(true)); 

                    lastCommandToken = 'A';
                    break;
 
                case 'z':
                case 'Z': 
                    EnsureFigure(); 
                    pathGeometry->EndFigure();
 
                    m_hasFigureBegan = false;
                    lastCommandToken = 'Z';

                    m_lastPoint = m_lastStartingPoint;
                    break;
 
                default: 
                    throw exception("bad token");
                    break; 
                }
            }

            if(m_hasFigureBegan)
                pathGeometry->EndFigure();
        }

        PathParser() : m_currentPosition(0)
        {

        }
};

PathElement::PathElement(void)
{
}

PathElement::~PathElement(void)
{
}

void PathElement::Geometry( GeometryPtr geometry )
{
    SetGeometry(geometry);
}

auto PathElement::Geometry() -> GeometryPtr
{
    return GetGeometry();
}

GeometryPtr PathElement::Parse( std::wstring& data )
{
    auto pathGeometry = make_shared<PathGeometry>();
    PathParser parser;

    parser.ParseToGeometryContext(pathGeometry, data);

    return pathGeometry;
}

void PathElement::RenderOverride(const IRenderContextPtr& renderContext)
{
    auto renderSize = GetRenderSize();

    if(!GetGeometry())
    {
	   return;
    }

    auto fillBrush = FillBrush();
   
    renderContext->DrawGeometry(GetGeometry(), fillBrush, GetPen());

    return;
}

Graphucks::Size PathElement::MeasureOverride(const Graphucks::Size& availableSize)
{
    Size newSize; 

    Stretch mode = GetStretch(); 
 
   auto geometry = GetGeometry();

    if (mode == Stretch::None)
    { 
        auto geometry = GetGeometry();

        if(geometry)
        {
             Rectangle bounds = geometry->GetBounds(GetPen(), D2D1::Matrix3x2F::Identity());
             newSize = Size(max(bounds.right, 0), max(bounds.bottom, 0));
        }
    }
    else
    { 
        if(geometry)
        {
            Rectangle bounds = geometry->GetBounds(GetPen(), D2D1::Matrix3x2F::Identity());
            newSize = GetStretchedRenderSize(mode, StrokeWidth(), availableSize, bounds);
        }
    } 
    
    return newSize; 
}

Size PathElement::GetStretchedRenderSize(Stretch mode, float strokeThickness, Size availableSize, const Graphucks::Rectangle& geometryBounds) 
{ 
    float xScale, yScale, dX, dY;
    Size renderSize; 

    GetStretchSizeAttributes(mode, strokeThickness, availableSize, geometryBounds, xScale, yScale, dX, dY, renderSize);
 
    return renderSize;
} 

void PathElement::GetStretchSizeAttributes(const Graphucks::Stretch mode, const float strokeThickness, const Size& availableSize, const Graphucks::Rectangle& geometryBounds, float& xScale, float& yScale, float& dX, float& dY, Size& stretchedSize)
{
    if (!geometryBounds.IsEmpty()) 
    {
        double margin = strokeThickness / 2;
        bool hasThinDimension = false;
 
        xScale = max(availableSize.width - strokeThickness, 0); 
        yScale = max(availableSize.height - strokeThickness, 0); 
        dX = margin - geometryBounds.left;
        dY = margin - geometryBounds.top; 

        if (geometryBounds.Width() > xScale * FLT_EPSILON)
        { 
            xScale /= geometryBounds.Width();
        }
        else
        { 
            xScale = 1;
            hasThinDimension = true; 
        } 

        if (geometryBounds.Height() > yScale * FLT_EPSILON) 
        {
            yScale /= geometryBounds.Height();
        }
        else 
        {
            yScale = 1; 
            hasThinDimension = true; 
        }

        if (mode != Stretch::Fill && !hasThinDimension) 
        {
            if (mode == Stretch::Uniform) 
            {
                if (yScale > xScale)
                {
                    yScale = xScale;
                } 
                else
                {
                    xScale = yScale;
                }
            }
            else 
            {
                if (xScale > yScale)
                { 
                    yScale = xScale;
                }
                else
                {
                    xScale = yScale; 
                }
            } 
        }

        stretchedSize = Size(geometryBounds.Width() * xScale + strokeThickness, geometryBounds.Height() * yScale + strokeThickness);
    } 
    else
    { 
        xScale = yScale = 1; 
        dX = dY = 0;
        stretchedSize = Size(0,0); 
    }
}

Graphucks::Size PathElement::ArrangeOverride(const Graphucks::Size& finalSize)
{
    Size newSize;
 
    Stretch mode = GetStretch();
    auto geometry = GetGeometry();

    if (mode == Stretch::None) 
    {
        newSize = finalSize; 
    } 
    else if(geometry)
    { 
        Rectangle bounds = geometry->GetBounds(GetPen(), D2D1::Matrix3x2F::Identity());
        newSize = GetRenderSizeTransformGeometry(mode, StrokeWidth(), finalSize, bounds);
    }

    return newSize;
}

Graphucks::Size PathElement::GetRenderSizeTransformGeometry(Graphucks::Stretch mode, float strokeThickness, Graphucks::Size availableSize, const Graphucks::Rectangle& geometryBounds)
{ 
    float xScale, yScale, dX, dY;
    Size renderSize;

    GetStretchSizeAttributes(mode, strokeThickness, availableSize, geometryBounds, xScale, yScale, dX, dY, renderSize);
 
    if(xScale == 1 && yScale == 1 && 
       dX == 0 && dY == 0 && 
       geometryBounds.left == 0 && geometryBounds.top == 0)
        return renderSize;

    auto transform = make_shared<CompositeTransform>();
    
    transform->CenterX(geometryBounds.Location().x);
    transform->CenterY(geometryBounds.Location().y);
    transform->ScaleX(xScale);
    transform->ScaleY(yScale);
    transform->TranslateX(dX);
    transform->TranslateY(dY);
    
    auto geometry = GetGeometry();
    geometry->Transform(transform);

    return renderSize; 
} 