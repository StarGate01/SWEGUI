#include <iostream>
#include "DataRenderer.hpp"
#include "../resources/ResourceHelper.hpp"
#include "Errors.hpp"

// #define TIMESTEP 20

using namespace renderer;

DataRenderer::DataRenderer(widgets::SFMLWidget &widget) : widget(widget)
{
    b.name = "b"; h.name = "h"; hu.name = "hu"; hv.name = "hv"; hx.name = "hx"; hx.computed = true;
    meta_info = &netcdf_stream.meta_info;

    //test
    b.enable = true;
    h.enable = true;
    h.clip = true;
    b.colors[0] = sf::Color::Black;
    b.colors[0].a = 0;
    for(int i=2; i<8; i++) b.colors[i] = sf::Color::White;
    //end test

    if (!sf::Shader::isAvailable()) perror("Shaders are not available on this GPU");
    string shader_src = resources::ResourceHelper::global_to_string(PATH_TO_FRAG_SHADER);
    if (!shader.loadFromMemory(sf::String(shader_src), sf::Shader::Fragment)) perror("Cannot load shader");
    for(int i=0; i<5; i++) 
    {
        layers[i]->index = i;
        layers[i]->shader = &shader;
        layers[i]->update_shader(true);
    }

    crosshair_tex.loadFromMemory(resources::ResourceHelper::global_to_memory(PATH_TO_CROSSHAIR_TEX),
        resources::ResourceHelper::global_get_size(PATH_TO_CROSSHAIR_TEX));
    crosshair_active_tex.loadFromMemory(resources::ResourceHelper::global_to_memory(PATH_TO_CROSSHAIR_ACTIVE_TEX),
        resources::ResourceHelper::global_get_size(PATH_TO_CROSSHAIR_ACTIVE_TEX));

    update_padding();

    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &DataRenderer::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &DataRenderer::resize_view)));
    widget.add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    widget.signal_button_press_event().connect(sigc::mem_fun(this, &DataRenderer::on_button_press_event));
}

DataRenderer::type_signal_update DataRenderer::signal_update()
{
    return m_signal_update;
}

DataRenderer::type_signal_select DataRenderer::signal_select()
{
    return m_signal_select;
}

int DataRenderer::open(std::string filename)
{
    bool ret = netcdf_stream.open(filename);
    if(!ret) return ERROR_FILE;
    int res = select_timestamp(3);
    if(res != ERROR_SUCCESS) return res;
    update_padding();
    update_shader();
    return ERROR_SUCCESS;
}

int DataRenderer::select_timestamp(int timestamp)
{
    int res = select_load(NetCdfImageStream::Variable::B, 0, b);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::H, timestamp, h);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::Hu, timestamp, hu);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::Hv, timestamp, hv);
    if(res != ERROR_SUCCESS) return res;
    current_timestamp = timestamp;
    return ERROR_SUCCESS;
}

int DataRenderer::get_current_timestamp()
{
    return current_timestamp;
}

float DataRenderer::sample(NetCdfImageStream::Variable var, float x, float y, int timestamp)
{
    if(timestamp == -1) timestamp = current_timestamp;
    if(timestamp == -1) return 0.f;
    return netcdf_stream.sample(var, x, y, timestamp);
}

int DataRenderer::select_load(NetCdfImageStream::Variable variable, int index, Layer& lay)
{
    bool ret = netcdf_stream.select(variable, index);
    if(!ret) return ERROR_SELECT;
    lay.meta_info = netcdf_stream.meta_info; //copy!
    sf::Image img;
    ret = img.loadFromStream(netcdf_stream);
    if(!ret) return ERROR_STREAM;
    // int max_tex = sf::Texture::getMaximumSize();
    // sf::Rect<int> area(0, 0, 
    //     max(netcdf_stream.meta_info->width, max_tex),
    //     max(netcdf_stream.meta_info->height, max_tex));
    ret = lay.texture.loadFromImage(img); //, area);
    if(!ret) return ERROR_IMAGE;
    return ERROR_SUCCESS;
}

void DataRenderer::update_shader()
{
    for(int i=0; i<5; i++) layers[i]->update_shader();
    widget.invalidate();
}

void DataRenderer::invalidate()
{
    widget.invalidate();
}

void DataRenderer::draw()
{
    widget.renderWindow.clear(sf::Color(128, 128, 128, 255));
    widget.renderWindow.draw(background, &shader);
    for(auto& probe: probes) 
    {
        sf::Vector2f d2s = data_to_screen(sf::Vector2f(probe.second.x, probe.second.y));
        probe.second.getSprite().setPosition(d2s.x, d2s.y);
        if(probe.first == active_probe_name) probe.second.getSprite().setTexture(crosshair_active_tex);
        else probe.second.getSprite().setTexture(crosshair_tex);
        widget.renderWindow.draw(probe.second.getSprite());
    }
    widget.display();
}

void DataRenderer::resize_view()
{
    update_padding();
    sf::Vector2f lower_right(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::View view(lower_right * 0.5f, lower_right);
    widget.renderWindow.setView(view);
}

void DataRenderer::update_padding()
{
    float screen_width = widget.renderWindow.getSize().x;
    float screen_height = widget.renderWindow.getSize().y;
    background.setSize(sf::Vector2f(screen_width, screen_height));
    shader.setParameter("screensize", sf::Vector2f(screen_width, screen_height));
    float asp_screen = screen_width / screen_height;
    float asp_data = meta_info->ax() / meta_info->ay();
    if(asp_data > asp_screen)
    {
        float newheight = meta_info->ay() * (screen_width / meta_info->ax());
        pad_v = (screen_height - newheight) / 2.f;
        pad_h = 0.f;
    }
    else
    {
        float newwidth = meta_info->ax() * (screen_height / meta_info->ay());
        pad_h = (screen_width - newwidth) / 2.f;
        pad_v = 0.f;
    }
    shader.setParameter("padding", sf::Vector2f(pad_h, pad_v));
}


sf::Vector2f DataRenderer::screen_to_data(sf::Vector2f coord)
{
    float screen_width = widget.renderWindow.getSize().x;
    float screen_height = widget.renderWindow.getSize().y;
    return sf::Vector2f((((coord.x - pad_h) / (screen_width - (pad_h * 2.0))) * meta_info->ax()) + meta_info->originx,
        (((coord.y - pad_v) / (screen_height - (pad_v * 2.0))) * meta_info->ay()) + meta_info->originy);
}

sf::Vector2f DataRenderer::data_to_screen(sf::Vector2f coord)
{
    float screen_width = widget.renderWindow.getSize().x;
    float screen_height = widget.renderWindow.getSize().y;
    return sf::Vector2f((((coord.x - meta_info->originx) / meta_info->ax()) * (screen_width - (pad_h * 2.0))) + pad_h,
        (((coord.y - meta_info->originy) / meta_info->ay()) * (screen_height - (pad_v * 2.0))) + pad_v);
}

bool DataRenderer::on_button_press_event(GdkEventButton *event)
{
    if((event->type == GDK_2BUTTON_PRESS) && (event->button == 1))
    {
        sf::Vector2f s2d = screen_to_data(sf::Vector2f(event->x, event->y));
        string name = unique_name();
        probe::DataProbe probe(s2d.x, s2d.y);
        probes[name] = probe;
        active_probe_name = name;
        invalidate();
        m_signal_update.emit(true);
        m_signal_select.emit();
        return true;
    }
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 1))
    {
        if(event->x < pad_h || event->x > (widget.renderWindow.getSize().x - pad_h)
            || event->y < pad_v || event->y > (widget.renderWindow.getSize().y - pad_v)) return true;
        for(auto& probe: probes)
        {
            float dx = probe.second.getSprite().getPosition().x - event->x;
            float dy = probe.second.getSprite().getPosition().y - event->y;
            if(sqrt(dx*dx + dy*dy) <= 16.f)
            {
                active_probe_name = probe.first;
                invalidate();
                m_signal_select.emit();
                return true;
            }
        }
        return true;
    }
    return false;
}

string DataRenderer::unique_name()
{
    int index = 0;
    while(true)
    {
        stringstream ss;
        ss << "probe_" << index;
        string name = ss.str();
        if(probes.find(name) == probes.end()) return name;
        index++;
    }
}