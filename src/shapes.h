#ifndef SHAPES_H
#define SHAPES_H

#include <SFML/Graphics.hpp>

namespace shapes
{

constexpr double pi{3.14159265358979323846}, radDeg{180/pi};
constexpr int winSize{600}, halfWin{winSize/2};

enum CType  {ICircle, RCircle, SCircle, CCircle};
enum CStyle {Background , Foreground, Grid};

class CoordTrans
{
public:
    double get_g(const sf::Vector2i& p) const noexcept;
    double get_x(const sf::Vector2i& p) const noexcept;
    double get_angle(const double x, const double y) const noexcept;
    template <typename T>
    double get_angle(const sf::Vector2<T> p) const noexcept
    {
        return atan2(p.y, p.x);
    }
};

class ThickLine
{
    sf::RectangleShape line;

public:
    ThickLine(const double ang);
    sf::RectangleShape getShape() &&
    {
        return line;
    }
};

template<CType T>
class Circle final: public CoordTrans
{
    sf::CircleShape shape;
    sf::Color color;

public:
    Circle(const double d, const CStyle style=CStyle::Grid);
    Circle(const sf::Vector2i& p, const CStyle style);
    sf::CircleShape getShape() &&
    {
        return shape;
    }
};

}

#endif // SHAPES_H
