#include <iostream>
#include "DataRenderer.hpp"
#include "../resources/ResourceHelper.hpp"
#include "Errors.hpp"

// #define TIMESTEP 20

using namespace renderer;

DataRenderer::DataRenderer(widgets::SFMLWidget &widget) : widget(widget)
{
    hx.computed = true;
    b.enable = true;
    h.enable = true;
    h.clip = true;

    meta_info = &netcdf_stream.meta_info;

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

    lut.loadFromMemory(resources::ResourceHelper::global_to_memory(PATH_TO_LUT),
        resources::ResourceHelper::global_get_size(PATH_TO_LUT));
    shader.setParameter("lut", lut);

    update_transform();

    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &DataRenderer::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &DataRenderer::resize_view)));
    widget.add_events(Gdk::EventMask::BUTTON_PRESS_MASK | Gdk::EventMask::SCROLL_MASK);
    widget.signal_button_press_event().connect(sigc::mem_fun(this, &DataRenderer::on_button_press_event));
    widget.signal_scroll_event().connect(sigc::mem_fun(this, &DataRenderer::on_scroll_event));
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
    int res = select_timestamp(0);
    if(res != ERROR_SUCCESS) return res;
    update_transform();
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

float DataRenderer::get_current_time()
{
    return netcdf_stream.get_time(current_timestamp);
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
        sf::Vector2f d2s = screen_to_tex.getInverse() * 
            (tex_to_data.getInverse() * sf::Vector2f(probe.second.x, probe.second.y));
        probe.second.getSprite().setPosition(d2s.x, d2s.y);
        if(probe.first == active_probe_name) probe.second.getSprite().setTexture(crosshair_active_tex);
        else probe.second.getSprite().setTexture(crosshair_tex);
        widget.renderWindow.draw(probe.second.getSprite());
    }
    widget.display();
}

void DataRenderer::resize_view()
{
    update_transform();
    sf::Vector2f lower_right(widget.renderWindow.getSize().x, widget.renderWindow.getSize().y);
    sf::View view(lower_right * 0.5f, lower_right);
    widget.renderWindow.setView(view);
}

void DataRenderer::update_transform()
{
    sf::Vector2f ss = (sf::Vector2f)widget.renderWindow.getSize();
    background.setSize(ss);

    float dasp = meta_info->ax() / meta_info->ay(); 
    sf::Vector2f psize = ss;
    if(dasp > (ss.x / ss.y)) psize.y = ss.x / dasp;
    else psize.x = ss.y * dasp;

    screen_to_tex = sf::Transform::Identity;
    screen_to_tex.scale(1.f / psize.x, 1.f / psize.y);
    screen_to_tex.translate((ss - psize) / (-2.f));

    tex_to_data = sf::Transform::Identity;
    tex_to_data.translate(meta_info->xmin, meta_info->ymin);
    tex_to_data.scale(meta_info->ax(), -meta_info->ay());
    tex_to_data.translate(0, -1);

    shader.setParameter("transform", screen_to_tex);
}

bool DataRenderer::on_button_press_event(GdkEventButton *event)
{
    sf::Vector2f tp = screen_to_tex * sf::Vector2f(event->x, event->y);
    if(tp.x < 0 || tp.x > 1 || tp.y < 0 || tp.y > 1) return true;
    if((event->type == GDK_2BUTTON_PRESS) && (event->button == 1))
    {
        sf::Vector2f s2d = tex_to_data * (screen_to_tex * sf::Vector2f(event->x, event->y));
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

bool DataRenderer::on_scroll_event(GdkEventScroll *event)
{
    std::cout << "scrll" <<std::endl;
    return true;
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