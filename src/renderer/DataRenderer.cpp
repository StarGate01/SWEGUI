#include <iostream>
#include <sstream>
#include "DataRenderer.hpp"
#include "../resources/ResourceHelper.hpp"
#include "Errors.hpp"

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
    shader.loadFromMemory(sf::String(shader_src), sf::Shader::Fragment);
    string shader_scale_src = resources::ResourceHelper::global_to_string(PATH_TO_FRAG_SCALE_SHADER);
    shader_scale.loadFromMemory(sf::String(shader_scale_src), sf::Shader::Fragment);
    for(int i=0; i<5; i++) 
    {
        layers[i]->index = i;
        layers[i]->shader = &shader;
        layers[i]->update_shader(true);
    }
    
    load_texture(PATH_TO_LUT, &lut);
    load_texture(PATH_TO_CROSSHAIR_TEX, &crosshair_tex);
    load_texture(PATH_TO_CROSSHAIR_ACTIVE_TEX, &crosshair_active_tex);
    load_texture(PATH_TO_GIZMO_TEX, &gizmo_tex);
    load_texture(PATH_TO_ZERO_TEX, &zero_tex);

    font.loadFromMemory(resources::ResourceHelper::global_to_memory(PATH_TO_FONT),
        resources::ResourceHelper::global_get_size(PATH_TO_FONT));

    info_text.setCharacterSize(14);
    info_text.setFont(font);
    info_text.setColor(sf::Color::White);
    info_text.setPosition(10.f, 5.f);
    info_rect.setFillColor(sf::Color(0, 0, 0, 70));
    info_rect.setPosition(5.f, 5.f);

    probe_text.setFont(font);
    probe_text.setCharacterSize(12);
    probe_text.setColor(sf::Color::White);
    probe_rect.setFillColor(sf::Color(0, 0, 0, 70));

    gizmo_rect.setSize(sf::Vector2f(32, 32));
    gizmo_rect.setTexture(&gizmo_tex);

    zero_rect.setSize(sf::Vector2f(25, 25));
    zero_rect.setTexture(&zero_tex);
    zero_rect.setOrigin(12.5, 12.5);

    scale_rect.setSize(sf::Vector2f(100, 20));
    scale_bg_rect.setFillColor(sf::Color(0, 0, 0, 70));

    coordinate_text.setFont(font);
    coordinate_text.setCharacterSize(12);
    coordinate_text.setColor(sf::Color::White);

    shader.setParameter("lut", lut);
    shader_scale.setParameter("lut", lut);
    update_transform();

    widget.signal_draw().connect(sigc::bind_return(sigc::hide(sigc::mem_fun(this, &DataRenderer::draw)), true));
    widget.signal_size_allocate().connect(sigc::hide(sigc::mem_fun(this, &DataRenderer::resize_view)));
    widget.add_events(Gdk::EventMask::BUTTON_PRESS_MASK | Gdk::EventMask::BUTTON_RELEASE_MASK
        | Gdk::EventMask::BUTTON2_MOTION_MASK | Gdk::EventMask::BUTTON3_MOTION_MASK | Gdk::EventMask::SCROLL_MASK);
    widget.signal_button_press_event().connect(sigc::mem_fun(this, &DataRenderer::on_button_press_event));
    widget.signal_button_release_event().connect(sigc::mem_fun(this, &DataRenderer::on_button_release_event));
    widget.signal_scroll_event().connect(sigc::mem_fun(this, &DataRenderer::on_scroll_event));
    widget.signal_motion_notify_event().connect(sigc::mem_fun(this, &DataRenderer::on_motion_notify_event));
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
    zoom = 1.f;
    pan = sf::Vector2f(0.f, 0.f);
    update_transform();
    update_shader();
    return ERROR_SUCCESS;
}

void DataRenderer::load_texture(std::string path, sf::Texture* tex)
{
    tex->loadFromMemory(resources::ResourceHelper::global_to_memory(path),
        resources::ResourceHelper::global_get_size(path));
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

string DataRenderer::float_to_string(float value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}