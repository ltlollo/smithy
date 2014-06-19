#include "smithchart.h"

using namespace sf;
using namespace std;
using namespace shapes;

namespace smith
{

double SmithChart::get_wl(const Vector2i& p) const noexcept
{
    const int x{halfWin - p.x}, y{halfWin - p.y};
    const double ang{get_angle(x, y)};
    return ang/(4*pi);
}

SmithChart::SmithChart()
    : settings(0, 0, aliasLvl, 3, 0),
      window{{winSize, winSize}, title,
             Style::Resize | Style::Close, settings
},
      grid{Circle<RCircle>(1., Foreground).getShape(),
           Circle<RCircle>(0., Grid).getShape()
},
      xAxis{Vertex(Vector2f(0.,      halfWin), Color::Black),
            Vertex(Vector2f(winSize, halfWin), Color::Black)
},
      steps{0.1, 0.2, 0.4, 0.6, 0.8, 1.0,
            1.2, 1.4, 1.6, 1.8, 2.0, 3.0,
            5.0, 10., 20., 50.
},
      showCCircle{false}, showCLine{false}, showRCircle{false},
      showICircle{false}, showSCircle{false}, showCoord{true},
      showOutline{true},  currentObj{-1},
      inside{Color(0,0,0xCA, 50)}, insideOutline{Color(0xCA,0,0,50)}
{
    font.loadFromFile(fontFile);
    coord = Text{{}, font, 10};
    coord.setColor(Color(0x99, 0x00, 0x00));

    grid.reserve(50);
    usrShapes.reserve(4);
    usrLines.reserve(4);
    actions.reserve(8);

    for_each(steps.cbegin(), steps.cend(), [&](const double s)
    {
        grid.emplace_back(Circle<RCircle>(s).getShape());
        grid.emplace_back(Circle<ICircle>(s).getShape());
        grid.emplace_back(Circle<ICircle>(-s).getShape());
    });
}

void SmithChart::drawCLine(const sf::Vector2i& p)
{
    const int x{halfWin - p.x}, y{halfWin - p.y};
    const double ang{radDeg*get_angle(x, y)};
    window.draw(ThickLine(ang).getShape());
}

void SmithChart::drawFixedElements()
{
    window.draw(xAxis, 2, Lines);

    for_each(grid.cbegin(), grid.cend(), [&](const CircleShape& it)
    {
        window.draw(it);
    });

    for_each(usrShapes.cbegin(), usrShapes.cend(), [&](const CircleShape& it)
    {
        window.draw(it);
    });

    for_each(usrLines.cbegin(), usrLines.cend(), [&](const RectangleShape& it)
    {
        window.draw(it);
    });
}

void SmithChart::markCurrObj() { setOutlineCurrObj(true); }
void SmithChart::unmarkCurrObj() { setOutlineCurrObj(false); }

template<CType T, CStyle U>
void SmithChart::drawCircle()
{
    Vector2i pos{mouse.getPosition(window)};
    window.draw(Circle<T>(pos, U).getShape());
}

template<CType T, CStyle U>
void SmithChart::addCircle()
{
    unmarkCurrObj();
    Vector2i pos{mouse.getPosition(window)};
    usrShapes.emplace_back(Circle<T>(pos, U).getShape());
    actions.emplace_back(typeCircle);
    currentObj = actions.size()-1;
    markCurrObj();
}

void SmithChart::drawMovingElements()
{
    Vector2i pos{mouse.getPosition(window)};
    double g{get_g(pos)}, x{get_x(pos)}, wl{get_wl(pos)};
    coord.setString("R:" + to_string(g) + "\nI:" + to_string(x) +
                    "\nWLTG:" + to_string(wl) + "\nWLTL:" + to_string(0.5-wl)
                    );

    if (pos.x < halfWin)
        if  (pos.y < halfWin) coord.setPosition(pos.x + 10, pos.y + 20);
        else coord.setPosition(pos.x + 10, pos.y - 60);
    else if (pos.y < halfWin) coord.setPosition(pos.x - 80, pos.y + 20);
    else coord.setPosition(pos.x - 80, pos.y - 60);

    if (g > 0)
    {
        if (showRCircle) drawCircle<RCircle>();
        if (showICircle) drawCircle<ICircle>();
        if (showCoord) window.draw(coord);
    }
    if (showCCircle) drawCircle<CCircle>();
    if (showCLine) drawCLine(pos);
    if (showSCircle) drawCircle<SCircle>();
}

void SmithChart::undoLastAction()
{
    if (actions.empty()) return;
    unmarkCurrObj();
    if (actions.back() == typeCircle) usrShapes.pop_back();
    else usrLines.pop_back();
    actions.pop_back();
    currentObj = actions.size()-1;
    markCurrObj();
}

void SmithChart::undoAllActions()
{
    usrLines.erase(usrLines.begin(), usrLines.end());
    usrShapes.erase(usrShapes.begin(), usrShapes.end());
    actions.erase(actions.begin(), actions.end());
    currentObj = -1;
}

void SmithChart::display()
{
    while(window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) window.close();
            else if (event.type == Event::KeyPressed)
                switch (event.key.code) {
                case Keyboard::C:           // Show/hide a centered circle
                    toggle(showCCircle);
                    break;
                case Keyboard::S:           // Show/hide a stub circle
                    toggle(showSCircle);
                    break;
                case Keyboard::L:           // Show/hide radius
                    toggle(showCLine);
                    break;
                case Keyboard::R:           // Show/hide a real circle
                    toggle(showRCircle);
                    break;
                case Keyboard::I:           // Show/hide a img circle
                    toggle(showICircle);
                    break;
                case Keyboard::O:           // Show/hide the parameters & outline
                    toggle(showOutline); setOutlineCurrObj(showOutline);
                    toggle(showCoord);
                    break;
                case Keyboard::E:           // Hide all the dyn. shapes
                    setOff(showRCircle); setOff(showICircle);
                    setOff(showCCircle); setOff(showSCircle);
                    setOff(showCLine);
                    break;
                case Keyboard::Delete:      // Undo all  the actions
                    undoAllActions();
                    break;
                case Keyboard::BackSpace:   // Remove the last element
                    deleteCurrObj();
                    break;
                case Keyboard::Num1:        // Add a centered circle
                    addCircle<CCircle>();
                    break;
                case Keyboard::Num2:        // Add a real circle
                    addCircle<RCircle>();
                    break;
                case Keyboard::Num3:        // Add an img circle
                    addCircle<ICircle>();
                    break;
                case Keyboard::Num4:        // Add a centered line
                    addCLine();
                    break;
                case Keyboard::Num5:        // Add a stub circle
                    addCircle<SCircle>();
                    break;
                case Keyboard::Num6:        // Add a non ad. Dom.
                    addCircle<SCircle, Background>();
                    break;
                case Keyboard::Num7:        // Add a non ad. Dom.
                    addCircle<RCircle, Background>();
                    break;
                case Keyboard::Num8:        // Add a non ad. Dom.
                    addCircle<ICircle, Background>();
                    break;
                case Keyboard::Up:          // Move the last obj up
                    moveCurrObj<up>();
                    break;
                case Keyboard::Down:        // Move the last obj down
                    moveCurrObj<down>();
                    break;
                case Keyboard::Left:        // Move the last obj to the left
                    moveCurrObj<left>();
                    break;
                case Keyboard::Right:       // Move the last obj to the right
                    moveCurrObj<right>();
                    break;
                case Keyboard::Z:         // Increase the last obj's size
                    resizeCurrObj<inflate>();
                    break;
                case Keyboard::D:    // Decrease the last obj's size
                    resizeCurrObj<deflate>();
                    break;
                case Keyboard::PageDown:    // Rotate last obj clockwise
                    moveCurrObj<aclock>();
                    break;
                case Keyboard::PageUp:      // Rotate last obj anti-clockwise
                    moveCurrObj<clock>();
                    break;
                case Keyboard::N:           // Select next object;
                case Keyboard::Tab:
                    if (actions.empty()) break;
                    unmarkCurrObj();
                    currentObj = (currentObj+1)%actions.size();
                    markCurrObj();
                    break;
                case Keyboard::P:           // Select previous object;
                    if (actions.empty()) break;
                    markCurrObj();
                    currentObj = (actions.size()+currentObj-1)%actions.size();
                    unmarkCurrObj();
                    break;
                case Keyboard::Q:           // Quit the program
                case Keyboard::Escape:
                    window.close();
                    break;
                default:                    // Do nothing
                    break;
                }
        }
        window.clear(Color::White);
        drawFixedElements();
        drawMovingElements();
        window.display();
    }
}

void SmithChart::addCLine()
{
    unmarkCurrObj();
    Vector2i pos{mouse.getPosition(window)};
    int x{halfWin - pos.x}, y{halfWin - pos.y};
    double ang = radDeg*get_angle(x, y);
    usrLines.emplace_back(ThickLine(ang).getShape());
    actions.emplace_back(typeLine);
    currentObj = actions.size()-1;
    markCurrObj();
}

template<SmithChart::direction T>
void SmithChart::moveObj(const size_t pos)
{
    if (actions.size() < pos) return;

    size_t real_pos{toRealPos(pos)};

    if (actions.at(pos) == typeCircle)
    {
        sf::CircleShape& obj = usrShapes.at(real_pos);
        const Vector2f pos{obj.getPosition()};
        if (T == up)
            obj.move({0, -uStep});
        else if (T == down)
            obj.move({0, uStep});
        else if (T == left)
            obj.move({-uStep, 0});
        else if (T == right)
            obj.move({uStep, 0});
        else    // clock, aclock
        {
            float x{halfWin - pos.x}, y{halfWin - pos.y};
            float r = sqrt(x*x + y*y), ang = get_angle(x, y);
            if (T == clock) obj.setPosition({halfWin - r*cos(ang + uGrad),
                                             halfWin - r*sin(ang + uGrad)}
                                            );  // clock
            else obj.setPosition({halfWin - r*cos(ang - uGrad),
                                  halfWin - r*sin(ang - uGrad)}
                                 ); // aclock
        }
    }
    else
    {
        sf::RectangleShape& obj = usrLines.at(real_pos);
        const float ang{obj.getRotation()};
        if (T == left || T == up ||  T == clock)
            obj.setRotation(ang + uStep);
        else obj.setRotation(ang - uStep);
    }
}

template<SmithChart::changeSize T>
void SmithChart::resizeObj(const size_t pos)
{
    if (actions.size() < pos) return;

    size_t real_pos{toRealPos(pos)};
    if (actions.at(pos) == typeCircle)
    {
        CircleShape& obj = usrShapes.at(real_pos);
        const Vector2f size{obj.getScale()};

        if (T == inflate) obj.setScale({size.x/sizeScale, size.y/sizeScale});
        else obj.setScale({size.x*sizeScale, size.y*sizeScale});
    }
    else
    {
        RectangleShape& obj = usrLines.at(real_pos);
        const Vector2f size {obj.getSize()};

        if (T == inflate) obj.setSize({size.x/sizeScale, size.y});
        else obj.setSize({size.x*sizeScale, size.y});
    }
}

template<SmithChart::direction T>
void SmithChart::moveLastObj()
{
    if (!actions.empty()) moveObj<T>(actions.size()-1);
}

template<SmithChart::direction T>
void SmithChart::moveCurrObj()
{
    if (!actions.empty()) moveObj<T>(currentObj);
}

template<SmithChart::changeSize T>
void SmithChart::resizeLastObj()
{
    if(!actions.empty()) resizeObj<T>(usrShapes.size()-1);
}

template<SmithChart::changeSize T>
void SmithChart::resizeCurrObj()
{
    if(!actions.empty()) resizeObj<T>(currentObj);
}

size_t SmithChart::toRealPos(const size_t pos) const
{
    size_t real_pos{0};
    for_each(actions.cbegin(), actions.cbegin()+pos,
             [&real_pos](const usrType& it)
    {
        if (it == usrType::typeCircle) real_pos+=1;
    });
    if (actions.at(pos) == usrType::typeCircle) return real_pos;
    return pos - real_pos;
}

void SmithChart::setOutline(const size_t pos, const bool outline)
{
    if (actions.empty()) return;
    const Color lineColor = (outline && showOutline) ? Color::Red :
                                                       Color::Black,
            fillColor = (outline && showOutline) ? insideOutline :
                                                   inside;
    if (actions.at(pos) == typeCircle)
    {
        usrShapes.at(toRealPos(pos)).setOutlineColor(lineColor);
        if (usrShapes.at(toRealPos(pos)).getFillColor() != Color::Transparent)
            usrShapes.at(toRealPos(pos)).setFillColor(fillColor);
    }
    else usrLines.at(toRealPos(pos)).setFillColor(lineColor);
}

void SmithChart::setOutlineCurrObj(const bool outline)
{
    setOutline(currentObj, outline);
}

void SmithChart::deleteObj(const size_t pos)
{
    if (actions.empty()) return;

    size_t real_pos{toRealPos(pos)};
    if (actions.at(pos) == typeCircle)
        usrShapes.erase(usrShapes.begin()+real_pos);
    else usrLines.erase(usrLines.begin()+real_pos);
    actions.erase(actions.begin()+pos);
    currentObj = actions.size()-1;
    markCurrObj();
}

void SmithChart::deleteCurrObj()
{
    deleteObj(currentObj);
}

void SmithChart::toggle(bool& val) noexcept
{
    val = val ? false : true;
}

void SmithChart::setOff(bool& val) noexcept
{
    if(val) val = false;
}

}
