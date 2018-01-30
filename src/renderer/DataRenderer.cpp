/**
 * @file DataRenderer.cpp
 * @brief Implements part of the functionality defined in DataRenderer.hpp
 * 
 * Event handler related functionality is defined in DataRendererActions.cpp
 * Renderer related functionality is defined in DataRendererCore.cpp
 * Async data loading related functionality is defined in DataRendererThreads.cpp
*/

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
    hx.clip_min = 0.f;
    hx.clip_max = 1.f;

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

    dispatcher_select_timestamp.connect(sigc::mem_fun(this, &DataRenderer::on_thread_select_timestamp_notify));
    dispatcher_open.connect(sigc::mem_fun(this, &DataRenderer::on_thread_open_notify));
    dispatcher_sample_batch.connect(sigc::mem_fun(this, &DataRenderer::on_thread_sample_batch_notify));
}

DataRenderer::type_signal_update DataRenderer::signal_update()
{
    return m_signal_update;
}

DataRenderer::type_signal_select DataRenderer::signal_select()
{
    return m_signal_select;
}

void DataRenderer::load_texture(std::string path, sf::Texture* tex)
{
    tex->loadFromMemory(resources::ResourceHelper::global_to_memory(path),
        resources::ResourceHelper::global_get_size(path));
}

int DataRenderer::get_current_timestamp()
{
    return current_timestamp;
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
        else
        {
            if(probes[name] == nullptr) return name;
        }
        index++;
    }
}

string DataRenderer::float_to_string(float value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

bool DataRenderer::save_screenshot(string filename)
{
    sf::Vector2u windowSize = widget.renderWindow.getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(widget.renderWindow);
    sf::Image screenshot = texture.copyToImage();
    return screenshot.saveToFile(filename);
}