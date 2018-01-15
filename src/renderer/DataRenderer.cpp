#include <iostream>
#include "DataRenderer.hpp"

using namespace renderer;

#define TIMESTEP 20

DataRenderer::DataRenderer(sfml::SFMLWidget &widget) : widget(widget)
{
    if (!sf::Shader::isAvailable()) perror("Shaders are not available on this GPU");
    if (!shader.loadFromFile(PATH_TO_FRAG_SHADER, sf::Shader::Fragment)) perror("Cannot load shader");

    open("/mnt/c/Users/Christoph/Documents/Studium/5_WS1718/TSISIM/Tutorium/SWE/build/data/swe_prod_00.nc");
    //bath_tex.loadFromFile("/mnt/c/Users/Christoph/Desktop/test.bmp");
    shader.setParameter("bath_tex", bath_tex);
    // bath_tex.setSmooth(true);
    // bath_tex.setRepeated(false);

    background.setSize(sf::Vector2f(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y));
    // background.setFillColor(sf::Color::Red);
    // background.setTexture(&bath_tex, true);
    // background.setTextureRect(sf::IntRect(0, 0, 100, 100));
    shader.setParameter("screensize", widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);

    //Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(this, &DataRenderer::animate), true), TIMESTEP);
    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &DataRenderer::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &DataRenderer::resize_view)));
}

int DataRenderer::open(std::string filename)
{
    bool ret = netcdf_stream.open(filename);
    if(!ret) return 1;
    ret = netcdf_stream.select(NetCdfImageStream::Variable::B, 0);
    if(!ret) return 2;

    ret = bath_img.loadFromStream(netcdf_stream);
    if(!ret) return 3;
    // int max_tex = sf::Texture::getMaximumSize();
    // sf::Rect<int> area(0, 0, 
    //     max(netcdf_stream.meta_info->width, max_tex),
    //     max(netcdf_stream.meta_info->height, max_tex));

    ret = bath_tex.loadFromImage(bath_img); //, area);
    if(!ret) return 4;

    widget.invalidate();

    return 0;
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