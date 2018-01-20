#include "EditProbeDialog.hpp"
#include "MainWindow.hpp"

using namespace swegui;

EditProbeDialog::EditProbeDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Dialog(cobject), m_refBuilder(refBuilder)
{ }

EditProbeDialog* EditProbeDialog::create(MainWindow* pa)
{
    auto refBuilder = Gtk::Builder::create_from_file(PATH_TO_PROBE_GUI);
    EditProbeDialog* window = nullptr;
    refBuilder->get_widget_derived("dialog_probe_edit", window);
    window->parent = pa;
    window->set_transient_for(*(window->parent));
    window->setup_gui_elements();
    return window;
}

void EditProbeDialog::setup_gui_elements()
{
    adjustment_x = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(m_refBuilder->get_object("adjustment_x"));
    adjustment_y = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(m_refBuilder->get_object("adjustment_y"));
    m_refBuilder->get_widget("entry_name", entry_name);
    m_refBuilder->get_widget("spin_x", spin_x);
    m_refBuilder->get_widget("spin_y", spin_y);
    m_refBuilder->get_widget("button_okay", button_okay);
}

void EditProbeDialog::set_min_max(float xmin, float xmax, float ymin, float ymax)
{
    adjustment_x->set_lower(xmin);
    adjustment_x->set_upper(xmax);
    adjustment_y->set_lower(ymin);
    adjustment_y->set_upper(ymax);
}

void EditProbeDialog::set_name(std::string name)
{
    entry_name->set_text(name);
}

std::string EditProbeDialog::get_name()
{
    return entry_name->get_text();
}

void EditProbeDialog::set_x(float x)
{
    spin_x->set_value(x);
}

float EditProbeDialog::get_x()
{
    return spin_x->get_value();
}

void EditProbeDialog::set_y(float y)
{
    spin_y->set_value(y);
}

float EditProbeDialog::get_y()
{
    return spin_y->get_value();
}