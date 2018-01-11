#include "MovingCircle.hpp"

#define TIMESTEP 20

MovingCircle::MovingCircle(SFMLWidget &widget) : widget(widget)
{
    radius = 20.f;
    circle.setRadius(radius);
    circle.setPosition(100, 300);
    vx = 0.15f;
    vy = 0.25f;

    // Let the animate method be called every 50ms
    // Note: MovingCircle::animate() doesn't return any value, but signal_timeout() expects
    //       a boolean value.
    //       Using sigc::bind_return(true, ...) we get a signal returning always true.
    Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(this, &MovingCircle::animate), true), TIMESTEP);

    // Makes our draw Method beeing drawn everytime the widget itself gets drawn.
    // Note: MovingCircle::draw() doesn't accept any parameter, but signal_draw() gives one.
    //       Using sigc::hide(...) we get a signal expecting one.
    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &MovingCircle::draw)), true));
    // NOTE: in earlier gtkmm-versions (<3.0) instead of signal_draw, connext to signal_event_expose:
    // widget.signal_event_expose().connect(sigc::bind_return(
    //                                          sigc::hide(
    //                                              sigc::mem_fun(this, &MovingCircle::draw)),
    //                                          true));

    // Everytime the widget gets resized, we need to adjust the view.
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &MovingCircle::resize_view)));
}

void MovingCircle::animate()
{
   sf::Vector2f position = circle.getPosition();
    position.x += vx * TIMESTEP;
    position.y += vy * TIMESTEP;

    circle.setPosition(position);

    if(position.x > widget.renderWindow.getSize().x - radius 
        || position.x < radius) vx *= (-1.f);
    if(position.y > widget.renderWindow.getSize().y - radius 
        || position.y < radius) vy *= (-1.f);

    widget.invalidate();
}

void MovingCircle::draw()
{
    widget.renderWindow.clear();
    widget.renderWindow.draw(circle);
    widget.display();
}

void MovingCircle::resize_view()
{
    sf::Vector2f lower_right(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::View view(lower_right * 0.5f, lower_right);
    widget.renderWindow.setView(view);
}
