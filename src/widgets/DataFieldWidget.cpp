#include "DataFieldWidget.hpp"
#include "../MainWindow.hpp"

using namespace widgets;

DataFieldWidget::DataFieldWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Notebook(cobject), m_refBuilder(refBuilder)
{ }

DataFieldWidget* DataFieldWidget::create(swegui::MainWindow* pa, std::string na)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_DATAFIELD_GUI);
    DataFieldWidget* widget = nullptr;
    refBuilder->get_widget_derived("notebook_main", widget);
    widget->parent = pa;
    widget->name = na;
    // widget->set_transient_for(*(widget->parent));
    widget->setup_gui_elements();
    return widget;
}

void DataFieldWidget::setup_gui_elements()
{

}

void DataFieldWidget::update_ui()
{
    if(name != "")
    {
        cout << "probe WIDGET: " << name << " updates ui" << endl;
        //put data from parent->data_renderer->probes[name] into ui elements
        //use coordinates from probe
        //sample data with parent->data_renderer->sample
        //does not work for hx (not in file), calculate by hand
        //note: sample is not yet implemented, always returns 0
    }
}