#include "DataRenderer.hpp"

using namespace renderer;

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
    if(event->direction == GdkScrollDirection::GDK_SCROLL_DOWN && zoom < 1.5f) zoom *= 1.05f;
    else if(event->direction == GdkScrollDirection::GDK_SCROLL_UP) zoom *= 0.95;
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
        && (event->button == 2 || event->button == 3)) pan_active = false;
    return true;
}