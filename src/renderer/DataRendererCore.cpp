#include "DataRenderer.hpp"

using namespace renderer;

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
    if(settings.zero)
    {
        sf::Vector2f d2s = tm_data_to_screen * sf::Vector2f(0, 0);
        zero_rect.setPosition(d2s.x, d2s.y);
        widget.renderWindow.draw(zero_rect);
    }
    if(settings.coordinates)
    {
        widget.renderWindow.draw(coordinates);
        for(const auto& label: coordinate_labels) 
        {
            coordinate_text.setRotation(0);
            coordinate_text.setString(float_to_string(label.value));
            sf::FloatRect text_bounds = coordinate_text.getGlobalBounds();
            coordinate_text.setOrigin(0, text_bounds.height);
            if(label.orientation == 0 || label.orientation == 2)
            {
                coordinate_text.setPosition(0, 0);
                coordinate_text.rotate(90);
            }
            coordinate_text.setPosition(label.position);
            switch(label.orientation)
            {
                case 0: coordinate_text.move(0, -15 - text_bounds.width); break;
                case 1: coordinate_text.move(15, 0); break;
                case 2: coordinate_text.move(0, 15); break;
                case 3: coordinate_text.move(-15 - text_bounds.width, 0); break;
            }
            widget.renderWindow.draw(coordinate_text);
        }
    }
    if(settings.probes)
    {
        for(auto& probe: probes) 
        {
            sf::Vector2f d2s = tm_data_to_screen * sf::Vector2f(probe.second.x, probe.second.y);
            probe.second.getSprite().setPosition(d2s.x, d2s.y);
            if(probe.first == active_probe_name) probe.second.getSprite().setTexture(crosshair_active_tex);
            else probe.second.getSprite().setTexture(crosshair_tex);
            widget.renderWindow.draw(probe.second.getSprite());
            if(settings.probe_names)
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
    if(settings.gizmo)
    {
        widget.renderWindow.draw(gizmo_rect);
    }
    if(settings.scales)
    {
        int active_layers = 0;
        for(int i=0; i<5; i++)
        {
            if(!layers[i]->enable) continue;
            active_layers++;
        }
        scale_bg_rect.setSize(sf::Vector2f(SCALE_WIDTH + 10, (active_layers * 30.f)));
        widget.renderWindow.draw(scale_bg_rect);
        int current_layer = 0;
        for(int i=0; i<5; i++)
        {
            if(!layers[i]->enable) continue;
            float yoff = 10 + (current_layer * 30.f);
            sf::Vector2f cpos = scale_rect.getPosition();
            scale_rect.setPosition(cpos.x, yoff);
            sf::Transform trans = sf::Transform::Identity;
            trans.combine(tm_scale_to_tex);
            trans.translate(0, -yoff);
            shader_scale.setParameter("layer", i);
            shader_scale.setParameter("transform", trans);
            widget.renderWindow.draw(scale_rect, &shader_scale);
            current_layer++;
        }
    }
    if(settings.info)
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

    tm_data_to_screen = tm_screen_to_data.getInverse();

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
    gizmo_rect.setPosition(10, ssz.y - 42);

    scale_rect.setPosition(ssz.x - SCALE_WIDTH - 10, 0);
    scale_bg_rect.setPosition(ssz.x - SCALE_WIDTH - 15, 5);
    tm_scale_to_tex = sf::Transform::Identity;
    tm_scale_to_tex.scale(1.f / (float)SCALE_WIDTH, 1.f / 20.f);
    tm_scale_to_tex.translate(-(ssz.x - SCALE_WIDTH - 10), 0);

    update_coordinates();

    shader.setParameter("transform", tm_screen_to_tex);
}

void DataRenderer::update_coordinates()
{
    int num_markers = 11 / zoom;
    float dx = meta_info->ax() / (float)(num_markers - 1);
    float dy = meta_info->ay() / (float)(num_markers - 1);
    sf::Vector2f y_offset = sf::Vector2f(0, 10);
    sf::Vector2f x_offset = sf::Vector2f(10, 0);
    int num_vertices = (num_markers * 4) + (num_markers * 4) + 8;
    coordinates = sf::VertexArray(sf::Lines, num_vertices);
    coordinate_labels.clear();
    for(int i=0; i<num_markers*4; i+=4)
    {
        float xpos = meta_info->xmin + (dx * (i / 4));
        sf::Vector2f pos = sf::Vector2f(xpos, meta_info->ymax);
        sf::Vector2f d2s = tm_data_to_screen * pos;
        coordinates[i] = sf::Vertex(d2s);
        coordinates[i+1] = sf::Vertex(d2s - y_offset);
        coordinate_labels.push_back({xpos, d2s, 0});
        pos = sf::Vector2f(xpos, meta_info->ymin);
        d2s = tm_data_to_screen * pos;
        coordinates[i+2] = sf::Vertex(d2s);
        coordinates[i+3] = sf::Vertex(d2s + y_offset);
        coordinate_labels.push_back({xpos, d2s, 2});
    }
    for(int i=num_markers*4; i<num_vertices - 8; i+=4)
    {
        float ypos = meta_info->ymin + (dy * ((i - (num_markers*4)) / 4));
        sf::Vector2f pos = sf::Vector2f(meta_info->xmax, ypos);;
        sf::Vector2f d2s = tm_data_to_screen * pos;
        coordinates[i] = sf::Vertex(d2s);
        coordinates[i+1] = sf::Vertex(d2s + x_offset);
        coordinate_labels.push_back({ypos, d2s, 1});
        pos = sf::Vector2f(meta_info->xmin, ypos);
        d2s = tm_data_to_screen * pos;
        coordinates[i+2] = sf::Vertex(d2s);
        coordinates[i+3] = sf::Vertex(d2s - x_offset);
        coordinate_labels.push_back({ypos, d2s, 3});
    }
    sf::Vector2f l_up = tm_data_to_screen * sf::Vector2f(meta_info->xmin, meta_info->ymax);
    sf::Vector2f r_up = tm_data_to_screen * sf::Vector2f(meta_info->xmax, meta_info->ymax);
    sf::Vector2f r_lo = tm_data_to_screen * sf::Vector2f(meta_info->xmax, meta_info->ymin);
    sf::Vector2f l_lo = tm_data_to_screen * sf::Vector2f(meta_info->xmin, meta_info->ymin);
    int k = num_vertices - 8;
    coordinates[k] = sf::Vertex(l_up);
    coordinates[k+1] = sf::Vertex(r_up);
    coordinates[k+2] = sf::Vertex(r_up);
    coordinates[k+3] = sf::Vertex(r_lo);
    coordinates[k+4] = sf::Vertex(r_lo);
    coordinates[k+5] = sf::Vertex(l_lo);
    coordinates[k+6] = sf::Vertex(l_lo);
    coordinates[k+7] = sf::Vertex(l_up);
}