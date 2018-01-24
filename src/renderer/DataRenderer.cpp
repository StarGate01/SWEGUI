#include <iostream>
#include <sstream>
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

    lut.loadFromMemory(resources::ResourceHelper::global_to_memory(PATH_TO_LUT),
        resources::ResourceHelper::global_get_size(PATH_TO_LUT));
    shader.setParameter("lut", lut);

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
    if(render_probes)
    {
        for(auto& probe: probes) 
        {
            sf::Vector2f d2s = tm_screen_to_data.getInverse() * sf::Vector2f(probe.second.x, probe.second.y);
            probe.second.getSprite().setPosition(d2s.x, d2s.y);
            if(probe.first == active_probe_name) probe.second.getSprite().setTexture(crosshair_active_tex);
            else probe.second.getSprite().setTexture(crosshair_tex);
            widget.renderWindow.draw(probe.second.getSprite());
            if(render_probe_names)
            {
                probe_text.setString(probe.first);
                sf::FloatRect text_bounds = probe_text.getGlobalBounds();
                probe_text.setOrigin(text_bounds.width / 2.0f, 0.f);
                probe_text.setPosition(d2s.x, d2s.y + 20.0f);
                probe_rect.setSize(sf::Vector2f(text_bounds.width + 6.f, text_bounds.height + 6.f));
                probe_rect.setOrigin(((text_bounds.width +6.0f) / 2.0f), 0.f);
                probe_rect.setPosition(d2s.x, d2s.y + 20.0f);
                widget.renderWindow.draw(probe_rect);
                widget.renderWindow.draw(probe_text);
            }
        }
    }
    if(render_info)
    {
        widget.renderWindow.draw(info_rect);
        widget.renderWindow.draw(info_text);
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
    sf::Vector2f ssz = (sf::Vector2f)widget.renderWindow.getSize();
    background.setSize(ssz);

    float dasp = meta_info->ax() / meta_info->ay(); 
    sf::Vector2f psize = ssz;
    if(dasp > (ssz.x / ssz.y)) psize.y = ssz.x / dasp;
    else psize.x = ssz.y * dasp;

    sf::Transform tm_prep_zoom = sf::Transform::Identity;
    tm_prep_zoom.translate((ssz - psize) / (-2.f));
    tm_prep_zoom.translate(ssz * 0.5f);
    tm_prep_zoom.scale(zoom, zoom);
    tm_prep_zoom.translate(ssz * (-0.5f));

    tm_screen_to_tex = sf::Transform::Identity;
    tm_screen_to_tex.scale(1.f / psize.x, 1.f / psize.y);
    tm_screen_to_tex.translate(pan.x, -pan.y);
    tm_screen_to_tex.combine(tm_prep_zoom);

    tm_screen_to_data = sf::Transform::Identity;
    tm_screen_to_data.translate(meta_info->xmin, meta_info->ymin);
    tm_screen_to_data.scale(meta_info->ax(), -meta_info->ay());
    tm_screen_to_data.translate(0.f, -1.f);
    tm_screen_to_data.scale(1.f / psize.x, 1.f / psize.y);
    tm_screen_to_data.translate(pan.x, pan.y);
    tm_screen_to_data.combine(tm_prep_zoom);

    stringstream ss;
    ss << "Panning:     " << pan.x << " px, " << pan.y << " px\n";
    ss << "Zoom:        " << zoom << "\n";
    ss << "Window size: " << ssz.x << " px * " << ssz.y << " px\n\n";
    ss << "Map size:    " << meta_info->ax() << " * " << meta_info->ay() << "\n";
    ss << "Map cells:   " << meta_info->nx << " * " << meta_info->ny << "\n";
    ss << "Map range:   [" << meta_info->xmin << ", " << meta_info->xmax << "]\n";
    ss << "             [" << meta_info->ymin << ", " << meta_info->ymax << "]";
    info_text.setString(ss.str());
    sf::FloatRect text_bounds = info_text.getGlobalBounds();
    info_rect.setSize(sf::Vector2f(text_bounds.width + 10.f, text_bounds.height + 10.f));

    shader.setParameter("transform", tm_screen_to_tex);
}

bool DataRenderer::on_button_press_event(GdkEventButton* event)
{
    sf::Vector2f ssz = (sf::Vector2f)widget.renderWindow.getSize();
    sf::Vector2f tp = tm_screen_to_tex * sf::Vector2f(event->x, ssz.y - event->y);
    if(tp.x < 0 || tp.x > 1 || tp.y < 0 || tp.y > 1) return false;
    if((event->type == GDK_2BUTTON_PRESS) && (event->button == 1))
    {
        sf::Vector2f s2d = tm_screen_to_data * sf::Vector2f(event->x, event->y);
        string name = unique_name();
        probe::DataProbe probe(s2d.x, s2d.y, this);
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
        return false;
    }
    return false;
}

bool DataRenderer::on_scroll_event(GdkEventScroll* event)
{
    if(event->direction == GdkScrollDirection::GDK_SCROLL_DOWN) 
    {
        if(!(zoom * 1.05f > 1.5f)) zoom *= 1.05f;
    }
    else if(event->direction == GdkScrollDirection::GDK_SCROLL_UP)
    {
        zoom *= 0.95;
    } 
    update_transform();
    invalidate();
    return true;
}

bool DataRenderer::on_motion_notify_event(GdkEventMotion* event)
{
    if((event->type == GDK_MOTION_NOTIFY) 
        && (event->state & GDK_BUTTON2_MASK || event->state & GDK_BUTTON3_MASK))
    {
        sf::Vector2i delta = sf::Vector2i(0, 0);
        sf::Vector2i now = sf::Vector2i(event->x, event->y);
        if(!pan_active) pan_active = true;
        else delta = last_mouse - now;
        last_mouse = now;
        pan += ((sf::Vector2f)delta * zoom);
        update_transform();
        invalidate();
    }
    return false;
}

bool DataRenderer::on_button_release_event(GdkEventButton* event)
{
    if((event->type == GDK_BUTTON_RELEASE) 
        && (event->button == 2 || event->button == 3))
    {
        pan_active = false;
    }
    return true;
}
