#pragma once
#include <d2d1.h>

namespace Graphucks
{
    class Point
    {
    public:
         Point(float px, float py): x(px),
                                    y(py)
         {/**/}

         Point(): x(0), 
                  y(0)
         {/**/}

        ~Point(){/**/}

        float x;
        float y;

        operator D2D1_POINT_2F()const
        {
            return D2D1::Point2F(x, y);
        }

        Point& operator += (Point& point)
        {
            x += point.x;
            y += point.y;
            return *this;
        }
        
        Point& operator -= (Point& point)
        {
            x -= point.x;
            y -= point.y;
            return *this;
        }
    };

    class Size
    {
    public:
        Size(float w, float h): width  (w), 
                                height (h)
        {/**/}

        Size(): width(0), 
                height(0)
        {/**/}

        ~Size(){/**/}

        float width;
        float height;

        bool IsEmpty() const
        {
            return width == 0 && height == 0;
        }

        bool IsOneDimensionalOrEmpty() const
        {
            return width == 0 || height == 0;
        }

        Size Scale(float scale) const
        {
            return Size(width * scale, height * scale);
        }

        bool operator == (Size& size) const
        {
            if(size.width == width && size.height == height)
                return true;

            return false;
        }

        bool operator == (const Size& size) const
        {
            if(size.width == width && size.height == height)
                return true;

            return false;
        }

        bool operator != (Size& size) const 
        {
            if(size.width == width && size.height == height)
                return false;

            return true;
        }

        operator D2D1_SIZE_F() const
        {
            return D2D1::SizeF(width, height);
        }
    };

    class Rectangle
    {
    public:
        Rectangle(float pLeft, float pTop, float pRight, float pBottom) : left    (pLeft), 
                                                                          top     (pTop), 
                                                                          right   (pRight), 
                                                                          bottom  (pBottom)
        {/**/}

        Rectangle() : left(0), top(0), right(0), bottom(0)
        {/**/}

        float left;
        float top;
        float right;
        float bottom;
        
        float Height() const
        {
            return bottom - top;
        }

        void Height(float height)
        {
            bottom = height + top;   
        }

        float Width() const
        {
            return right - left;
        }

        void Width(float width)
        {
            right = width + left;
        }
        
        Point Location() const
        {
            return Point(left, top);
        }

        bool IsEmpty() const
        {
            return left == 0 && right == 0 && bottom == 0 && top == 0;
        }

        void Location(const Point& point)
        {
            right -= (left - point.x);
            bottom -= (top - point.y);
            left = point.x;
            top = point.y;
        }

        void Grow(float by)
        {
            left -= by;
            top -= by;
            right += by;
            bottom += by;
        }

        Graphucks::Size Size() const
        {
            return Graphucks::Size(Width(), Height());
        }

        operator D2D1_RECT_F const()
        {
            return D2D1::RectF(left, top, right, bottom);
        }
    };
    
    enum class HorizontalAlignment
    {
        Left,
        Center,
        Right,
        Stretch
    };

    enum class VerticalAlignment
    {
        Top,
        Center,
        Bottom,
        Stretch
    };

    struct Thickness
    {
        Thickness() : Left   (0), 
                      Top    (0), 
                      Right  (0), 
                      Bottom (0)
        {
        }

        Thickness(float left, float top, float right, float bottom) : Left     (left), 
                                                                      Top      (top), 
                                                                      Right    (right), 
                                                                      Bottom   (bottom)
        {
        }

        float Left;
        float Top;
        float Right;
        float Bottom;
    };

    class Color
    {
      public:
        Color(float r, float g, float b, float a): R(r), 
                                                   G(g), 
                                                   B(b), 
                                                   A(a)
        {/**/}

        Color(): R(0), 
                 G(0), 
                 B(0), 
                 A(0)
        {/**/}

        virtual ~Color()
        {/**/}

        float R;
        float G;
        float B;
        float A;

        operator D2D1_COLOR_F() const
        {
            return D2D1::ColorF(R, G, B, A);
        }
    };

    enum class InterpolationMode
    {
        NearestNeightbor,
        Linear,
        Cubic,
        MultisampleLinear,
        Anisotropic,
        HighQualityCubic
    };

    enum class Stretch
    {
        Fill,
        None,
        Uniform,
        UniformToFill
    };

    enum class ExtendMode
    {
        Clamp = 0, Wrap = 1, Mirror = 2
    };

    enum class SweepDirection
    {
         Clockwise = 0,
         CounterClockwise = 1
    };

    enum class ArcSize
    {
        Small = 0,
        Large = 1
    };

    enum class StretchDirection
    {
        Both = 0,
        UpOnly,
        DownOnly
    };



    static const float GRAPHUCKS_INFINITY = FLT_MAX;//D2D1::FloatMax();


    inline float Infinity() { return FLT_MAX; }

    enum class EffectCompositeMode
    {
        SourceOver = 0,
        DestinationOver = 1,
        SourceIn = 2,
        DestinationIn = 3,
        SourceOut = 4,
        DestinationOut = 5,
        SourceATop = 6,
        DestinationATop = 7,
        XOR = 8,
        Plus = 9,
        SourceCopy = 10,
        BoundedSourceCopy = 11,
        MaskInvert = 12,
    };

}