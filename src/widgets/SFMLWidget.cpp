/**
 * @file SFMLWidget.cpp
 * 
 * @brief Implements the functionality defined in SFMLWidget.hpp
*/

#include "SFMLWidget.hpp"
#include "../MainWindow.hpp"
#include <gdk/gdkx.h>

using namespace widgets;

SFMLWidget::SFMLWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Bin(cobject), m_refBuilder(refBuilder)
{ }

SFMLWidget* SFMLWidget::create(swegui::MainWindow* pa)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_SFML_GUI);
    SFMLWidget* widget = nullptr;
    refBuilder->get_widget_derived("sfml_bin", widget);
    widget->parent = pa;
    // widget->set_transient_for(*(widget->parent));
    widget->setup_gui_elements();
    return widget;
}

void SFMLWidget::setup_gui_elements()
{
    set_size_request(300, 200);
    set_has_window(false);
}

void SFMLWidget::on_size_allocate(Gtk::Allocation &allocation)
{
    this->set_allocation(allocation);
    if (m_refGdkWindow)
    {
        m_refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height());
        renderWindow.setSize(sf::Vector2u(allocation.get_width(), allocation.get_height()));
    }
}

void SFMLWidget::on_realize()
{
    Gtk::Widget::on_realize();
    if (!m_refGdkWindow)
    {
        GdkWindowAttr attributes;
        memset(&attributes, 0, sizeof(attributes));
        Gtk::Allocation allocation = get_allocation();
        attributes.x = allocation.get_x();
        attributes.y = allocation.get_y();
        attributes.width = allocation.get_width();
        attributes.height = allocation.get_height();
        attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
        attributes.window_type = GDK_WINDOW_CHILD;
        attributes.wclass = GDK_INPUT_OUTPUT;
        m_refGdkWindow = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);
        set_has_window(true);
        set_window(m_refGdkWindow);
        this->unset_background_color();
        this->set_double_buffered(false);
        m_refGdkWindow->set_user_data(gobj());
        renderWindow.create(GDK_WINDOW_XID(m_refGdkWindow->gobj()));
    }
}

void SFMLWidget::on_unrealize()
{
    m_refGdkWindow.clear();
    Gtk::Widget::on_unrealize();
}

void SFMLWidget::display()
{
    if (m_refGdkWindow) renderWindow.display();
}

void SFMLWidget::invalidate()
{
    if (m_refGdkWindow) m_refGdkWindow->invalidate(true);
}