#include "SFMLWidget.hpp"
#include <gdk/gdkx.h>

using namespace sfml;

SFMLWidget::SFMLWidget(sf::VideoMode mode, int size_request)
{
    initialize(mode, size_request);
}

void SFMLWidget::initialize(sf::VideoMode mode, int size_request)
{
    if (size_request <= 0) size_request = std::max<int>(1, std::min<int>(mode.width, mode.height) / 2);
    set_size_request(size_request, size_request);
    set_has_window(false);
}

SFMLWidget::~SFMLWidget()
{ }

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
        //Create the GdkWindow:
        GdkWindowAttr attributes;
        memset(&attributes, 0, sizeof(attributes));
        Gtk::Allocation allocation = get_allocation();

        //Set initial position and size of the Gdk::Window:
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

        //make the widget receive expose events
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