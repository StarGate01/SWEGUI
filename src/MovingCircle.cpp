#include "MovingCircle.hpp"

#define TIMESTEP 20

MovingCircle::MovingCircle(SFMLWidget &widget) : widget(widget)
{
    if (!sf::Shader::isAvailable()) perror("Shaders are not available on this GPU");
    if (!shader.loadFromFile(PATH_TO_FRAG_SHADER, sf::Shader::Fragment)) perror("Cannot load shader");
    shader.setParameter("texture", sf::Shader::CurrentTexture);

    background.setSize(sf::Vector2f(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y));
    shader.setParameter("screensize", widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    circle.setRadius(20.f);
    circle.setPosition(100, 300);
    shader.setParameter("circlepos", 100 + circle.getRadius(), 300 + circle.getRadius());
    circle.setFillColor(sf::Color::Blue);
    vx = 0.15f;
    vy = 0.25f;

    Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(this, &MovingCircle::animate), true), TIMESTEP);
    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &MovingCircle::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &MovingCircle::resize_view)));
}

void MovingCircle::animate()
{
    sf::Vector2f position = circle.getPosition();
    position.x += vx * TIMESTEP;
    position.y += vy * TIMESTEP;

    circle.setPosition(position);

    if(position.x > widget.renderWindow.getSize().x - (circle.getRadius() * 2.f)
        || position.x < 0) vx *= (-1.f);
    if(position.y > widget.renderWindow.getSize().y - (circle.getRadius() * 2.f)
        || position.y < 0) vy *= (-1.f);

    shader.setParameter("circlepos", position.x + circle.getRadius(), position.y + circle.getRadius());
    widget.invalidate();
}

void MovingCircle::draw()
{
    widget.renderWindow.clear(sf::Color::Blue);
    widget.renderWindow.draw(background, &shader);
    widget.renderWindow.draw(circle);
    widget.display();
}

void MovingCircle::resize_view()
{
    background.setSize(sf::Vector2f(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y));
    shader.setParameter("screensize", widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::Vector2f lower_right(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::View view(lower_right * 0.5f, lower_right);
    widget.renderWindow.setView(view);
}