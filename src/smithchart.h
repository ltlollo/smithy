#ifndef SMITHCHART_H
#define SMITHCHART_H

#include "shapes.h"
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <array>
#include <vector>
#include <cmath>
#include <limits>

namespace smith
{

constexpr float uStep{2.}, sizeScale{0.99}, uGrad{0.02};
constexpr char title[]{"Smithy"};
constexpr unsigned aliasLvl{8};
constexpr char fontFile[]{"/usr/share/fonts/truetype/dejavu/"
    "DejaVuSansMono.ttf"
};

class SmithChart final: public shapes::CoordTrans
{
    const sf::ContextSettings settings;
    sf::RenderWindow window;
    sf::Event event;
    sf::Mouse mouse;
    sf::Font font;
    sf::Text coord;
    std::vector<sf::CircleShape> grid, usrShapes;
    std::vector<sf::RectangleShape> usrLines;
    const sf::Vertex xAxis[2];
    const std::array<double, 16> steps;
    bool showCCircle, showCLine, showRCircle,
    showICircle, showSCircle, showCoord, showOutline;
    enum usrType {typeLine, typeCircle};
    enum direction {left, right, up, down, clock, aclock};
    enum changeSize {inflate, deflate};
    long currentObj;
    std::vector<usrType> actions;
    const sf::Color inside, insideOutline;

    double get_wl(const sf::Vector2i& p) const noexcept;
    void drawCLine(const sf::Vector2i& p);
    void drawFixedElements();
    void drawMovingElements();
    void undoLastAction();
    void undoAllActions();
    void addCLine();
    template<direction T> void moveObj(const size_t pos);
    template<changeSize T> void resizeObj(const size_t pos);
    void setOutline(const size_t pos, const bool outline);

    template<direction T> void moveLastObj();
    template<changeSize T> void resizeLastObj();

    template<direction T> void moveCurrObj();
    template<changeSize T> void resizeCurrObj();
    void setOutlineCurrObj(const bool outline);

    size_t toRealPos(const size_t pos) const;
    void deleteObj(const size_t pos);
    void deleteCurrObj();

    template<shapes::CType T, shapes::CStyle U=shapes::CStyle::Foreground>
    void addCircle();
    template<shapes::CType T, shapes::CStyle U=shapes::CStyle::Foreground>
    void drawCircle();

    void markCurrObj();
    void unmarkCurrObj();

    void toggle(bool& val) noexcept;
    void setOff(bool& val) noexcept;

public:
    SmithChart();
    void display();
    void operator ()()
    {
        display();
    }
};

}

#endif // SMITHCHART_H
