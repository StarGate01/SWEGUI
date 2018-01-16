#include <iostream>
#include "DataRenderer.hpp"
#include "Errors.hpp"

// #define TIMESTEP 20

using namespace renderer;


DataRenderer::DataRenderer(sfml::SFMLWidget &widget) : widget(widget)
{
    b.name = "b"; b.index = 0; h.name = "h"; h.index = 1;
    hu.name = "hu"; hu.index = 2; hv.name = "hv";  hv.index = 3;
    hx.name = "hx"; hx.index = 4; hx.computed = true;
    meta_info = &netcdf_stream.meta_info;

    if (!sf::Shader::isAvailable()) perror("Shaders are not available on this GPU");
    if (!shader.loadFromFile(PATH_TO_FRAG_SHADER, sf::Shader::Fragment)) perror("Cannot load shader");
    for(int i=0; i<5; i++) layers[i]->update_shader(shader, true);

    background.setSize(sf::Vector2f(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y));
    shader.setParameter("screensize", widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);

    crosshair_tex.loadFromFile(PATH_TO_CROSSHAIR_TEX);
    crosshair_active_tex.loadFromFile(PATH_TO_CROSSHAIR_ACTIVE_TEX);


    // Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(this, &DataRenderer::animate), true), TIMESTEP);
    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &DataRenderer::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &DataRenderer::resize_view)));
    widget.add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    widget.signal_button_press_event().connect(sigc::mem_fun(this, &DataRenderer::on_button_press_event));
}

DataRenderer::type_signal_click DataRenderer::signal_click()
{
    return m_signal_click;
}

DataRenderer::type_signal_select DataRenderer::signal_select()
{
    return m_signal_select;
}

int DataRenderer::open(std::string filename)
{
    bool ret = netcdf_stream.open(filename);
    if(!ret) return ERROR_FILE;
    int res = select_timestamp(0);
    if(res != ERROR_SUCCESS) return res;
    update_shader();
    return ERROR_SUCCESS;
}

int DataRenderer::select_timestamp(int timestamp)
{
    int res = select_load(NetCdfImageStream::Variable::B, 0, layers[0]->texture);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::H, timestamp, layers[1]->texture);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::Hu, timestamp, layers[2]->texture);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::Hv, timestamp, layers[3]->texture);
    if(res != ERROR_SUCCESS) return res;
    return ERROR_SUCCESS;
}

int DataRenderer::select_load(NetCdfImageStream::Variable variable, int index, sf::Texture& tex)
{
    bool ret = netcdf_stream.select(NetCdfImageStream::Variable::B, 0);
    if(!ret) return ERROR_SELECT;
    sf::Image img;
    ret = img.loadFromStream(netcdf_stream);
    if(!ret) return ERROR_STREAM;
    // int max_tex = sf::Texture::getMaximumSize();
    // sf::Rect<int> area(0, 0, 
    //     max(netcdf_stream.meta_info->width, max_tex),
    //     max(netcdf_stream.meta_info->height, max_tex));
    ret = tex.loadFromImage(img); //, area);
    if(!ret) return ERROR_IMAGE;
    return ERROR_SUCCESS;
}

void DataRenderer::update_shader()
{
    for(int i=0; i<5; i++) layers[i]->update_shader(shader);
    widget.invalidate();
}

void DataRenderer::invalidate()
{
    widget.invalidate();
}

// void DataRenderer::animate()
// {
//     for(auto& chair: crosshairs)
//     {
//         chair.rotate(1.f);
//     }
//     widget.invalidate();
// }

void DataRenderer::draw()
{
    widget.renderWindow.clear(sf::Color::White);
    widget.renderWindow.draw(background, &shader);
    for(unsigned int i = 0; i < probes.size(); i++) 
    {
        probes[i].getSprite().setPosition(((probes[i].getPosition().x - meta_info->originx) / meta_info->dx) * (float)widget.renderWindow.getSize().x,
            ((probes[i].getPosition().y - meta_info->originy) / meta_info->dy) * (float)widget.renderWindow.getSize().y);
        if(&(probes[i]) == active_probe) probes[i].getSprite().setTexture(crosshair_active_tex);
        else probes[i].getSprite().setTexture(crosshair_tex);
        widget.renderWindow.draw(probes[i].getSprite());
    }
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

bool DataRenderer::on_button_press_event(GdkEventButton *event)
{
    if((event->type == GDK_2BUTTON_PRESS) && (event->button == 1))
    {
        m_signal_click.emit(((event->x / (float)widget.renderWindow.getSize().x) * meta_info->dx) + meta_info->originx,
            ((event->y / (float)widget.renderWindow.getSize().y) * meta_info->dy) + meta_info->originy);
        return true;
    }
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 1))
    {
        for(auto& probe: probes)
        {
            float dx = probe.getSprite().getPosition().x - event->x;
            float dy = probe.getSprite().getPosition().y - event->y;
            if(sqrt(dx*dx + dy*dy) <= 16.f)
            {
                active_probe = &probe;
                m_signal_select.emit();
                return true;
            }
        }
        return true;
    }
    return false;
}