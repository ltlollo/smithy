#include "shapes.h"

using namespace sf;

namespace shapes
{

ThickLine::ThickLine(const double ang)
    : line{RectangleShape(Vector2f(halfWin, 1.8))}
{
    line.setFillColor(sf::Color::Black);
    line.setOrigin(0, 0);
    line.setPosition(halfWin, halfWin);
    line.rotate(180+ang);
}

double CoordTrans::get_g(const Vector2i &p) const noexcept
{
    if (p.x == winSize)
    {
        if (p.y == winSize) return INFINITY;
        return -1.; // this is just to hide the coords,
        // since we are outside the chart.
    }
    int x{winSize - p.x}, y{halfWin - p.y};
    long z{x*x + y*y};
    double r = z/(2*x);
    return halfWin/r - 1;
}

double CoordTrans::get_x(const Vector2i &p) const noexcept
{
    if (p.y == halfWin) return 0.;
    int x{winSize - p.x}, y{halfWin - p.y};
    long z{x*x + y*y};
    double r = z/(2*y);
    return halfWin/r;
}


double CoordTrans::get_angle(const double x, const double y) const noexcept
{
    return atan2(y, x);
}

template<>
Circle<SCircle>::Circle(const Vector2i& p, const CStyle style)
    : color(style == CStyle::Background ? Color(0, 0, 0xCA, 50) :
                                      Color::Transparent)
{
    int x = halfWin - p.x, y = halfWin - p.y;
    double ang{get_angle(x,y)};
    float r = halfWin/2;
    float x_d = r*cos(ang), y_d = r*sin(ang);
    shape.setRadius(r);
    shape.setPointCount(100);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(color);
    if (style != CStyle::Background ) shape.setOutlineThickness(1.8);
    shape.setOrigin({r, r});
    shape.setPosition({halfWin - x_d, halfWin - y_d});
}

template<>
Circle<CCircle>::Circle(const Vector2i& p, const CStyle type)
    : color(type == CStyle::Background ? Color(0, 0, 0xCA, 3) :
                                         Color::Transparent)
{
    int x{halfWin - p.x}, y{halfWin - p.y};
    float r = sqrt(x*x + y*y);
    if (r > halfWin) r = halfWin;
    shape.setRadius(r);
    shape.setPointCount(100);
    shape.setOutlineColor(Color::Black);
    shape.setFillColor(color);
    shape.setOutlineThickness(1.8);
    shape.setOrigin({r, r});
    shape.setPosition({halfWin, halfWin});
}

template<>
Circle<RCircle>::Circle(const double d, const CStyle style)
{
    shape.setPosition(d/(1+d)*halfWin + halfWin, halfWin);
    shape.setRadius(1/(1+d)*halfWin);
    shape.setOrigin(1/(1+d)*halfWin, 1/(1+d)*halfWin);
    shape.setPointCount(100);

    if (style == Grid)
    {
        shape.setOutlineColor(Color(0x66, 0x66, 0x66));
        shape.setOutlineThickness(1);
        color = Color(0, 0, 0xCA, 3);
    }
    else
    {
        shape.setOutlineColor(Color::Black);
        if (style == Foreground)
        {
            color = Color::Transparent;
            shape.setOutlineThickness(1.8);
        }
        else
        {
            color = Color(0, 0, 0xCA, 50);
            shape.setOutlineThickness(0.);
        }
    }
    shape.setFillColor(color);
}

template<>
Circle<ICircle>::Circle(const double d, const CStyle style)
{
    shape.setPointCount(200);
    shape.setPosition(halfWin + halfWin, halfWin - halfWin/d);
    shape.setRadius(halfWin/d);
    shape.setOrigin(halfWin/d, halfWin/d);

    if (style == Grid)
    {
        shape.setOutlineColor(Color(0x66, 0x66, 0x66));
        shape.setOutlineThickness(1);
        color = Color(0, 0, 0, 1);
    }
    else
    {
        shape.setOutlineColor(Color::Black);
        if (style == Foreground)
        {
            color = Color::Transparent;
            shape.setOutlineThickness(1.8);
        }
        else
        {
            color = Color(0, 0, 0xCA, 50);
            shape.setOutlineThickness(0.);
        }
    }
    shape.setFillColor(color);
}

template<>
Circle<RCircle>::Circle(const Vector2i& p, const CStyle style)
    : Circle<RCircle>(get_g(p), style)
{}

template<>
Circle<ICircle>::Circle(const Vector2i& p, const CStyle style)
    : Circle<ICircle>(get_x(p), style)
{}

}
