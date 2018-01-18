#include "EditProbeDialog.hpp"
#include "MainWindow.hpp"

using namespace swegui;

EditProbeDialog::EditProbeDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Dialog(cobject), m_refBuilder(refBuilder)
{
    setup_gui_elements();
}

EditProbeDialog* EditProbeDialog::create(MainWindow* pa)
{
    auto refBuilder = Gtk::Builder::create_from_file(PATH_TO_PROBE_GUI);
    EditProbeDialog* window = nullptr;
    refBuilder->get_widget_derived("dialog_probe_edit", window);
    window->parent = pa;
    window->set_transient_for(*(window->parent));
    return window;
}

void EditProbeDialog::setup_gui_elements()
{
   
}