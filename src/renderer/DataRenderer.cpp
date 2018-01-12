#include "DataRenderer.hpp"

using namespace renderer;

#define TIMESTEP 20

DataRenderer::DataRenderer(sfml::SFMLWidget &widget) : widget(widget)
{
    if (!sf::Shader::isAvailable()) perror("Shaders are not available on this GPU");
    if (!shader.loadFromFile(PATH_TO_FRAG_SHADER, sf::Shader::Fragment)) perror("Cannot load shader");
    shader.setParameter("texture", sf::Shader::CurrentTexture);

    background.setSize(sf::Vector2f(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y));
    shader.setParameter("screensize", widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);

    //Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(this, &DataRenderer::animate), true), TIMESTEP);
    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &DataRenderer::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &DataRenderer::resize_view)));
}

bool DataRenderer::open(std::string filename)
{
    return netcdf_stream.open(filename);
}

// void DataRenderer::animate()
// {
//     widget.invalidate();
// }

void DataRenderer::draw()
{
    widget.renderWindow.clear(sf::Color::Blue);
    widget.renderWindow.draw(background, &shader);
    widget.display();
}

void DataRenderer::resize_view()
{
    background.setSize(sf::Vector2f(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y));
    shader.setParameter("screensize", widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::Vector2f lower_right(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::View view(lower_right * 0.5f, lower_right);
    widget.renderWindow.setView(view);
}