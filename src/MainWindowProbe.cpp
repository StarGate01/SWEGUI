/**
 * @file MainWindowProbe.cpp
 * 
 * @brief Implements path of the functionality defined in MainWindow.hpp, which is related to probes
 * 
 * Implementation of the general methods are implemented in MainWindow.cpp
 * Implementation of some event handler related methods are outsourced into MainWindowActions.cpp
*/

#include "MainWindow.hpp"

using namespace swegui;

void MainWindow::prepare_add_edit(bool clear)
{
    dialog_probe_edit->set_min_max(data_renderer->meta_info->xmin, data_renderer->meta_info->xmax, 
        data_renderer->meta_info->ymin, data_renderer->meta_info->ymax);
    if(!clear)
    {
        Gtk::TreeModel::iterator iter = probelist->get_selection()->get_selected();
        if(iter)
        {
            Gtk::TreeModel::Row row = *iter;
            string name = row[probelist_columns.col_name];
            probe::DataProbe* probe = data_renderer->probes[name];
            if(probe != nullptr)
            {
                dialog_probe_edit->set_name(name);
                dialog_probe_edit->set_x(probe->x);
                dialog_probe_edit->set_y(probe->y);
                dialog_probe_edit->existing = true;
                dialog_probe_edit->set_title("Edit probe");
                return;
            }
        }
    }
    dialog_probe_edit->set_name("");
    dialog_probe_edit->set_x(0.f);
    dialog_probe_edit->set_y(0.f);
    dialog_probe_edit->existing = false;
    dialog_probe_edit->set_title("Add probe");
}

void MainWindow::handle_add_edit()
{
    bool added = false;
    string name = dialog_probe_edit->get_name();
    if(name == "") return;
    if (data_renderer->probes.find(name) == data_renderer->probes.end())
    {
        if(dialog_probe_edit->existing) on_probe_remove();
        probe::DataProbe* probe = new probe::DataProbe(dialog_probe_edit->get_x(), dialog_probe_edit->get_y(), data_renderer);
        data_renderer->probes[name] = probe;
        added = true;
    }
    else
    {
        probe::DataProbe* probe = data_renderer->probes[name];
        if(probe != nullptr)
        {
            probe->x = dialog_probe_edit->get_x();
            probe->y = dialog_probe_edit->get_y();
            probe->reset_data(data_renderer);
        }
    }
    data_renderer->active_probe_name = name;
    data_renderer->invalidate();
    update_probe_ui(data_renderer->active_probe_name);
    on_probe_update(added);
    on_probe_select();
}

Gtk::TreeStore::iterator MainWindow::search_probelist(std::string name)
{
    Gtk::TreeModel::Children children = probelist_store->children();
    for(Gtk::TreeStore::iterator iter = children.begin(); iter != children.end(); ++iter)
    {
        Gtk::TreeModel::Row row = *iter;
        string name = row[probelist_columns.col_name];
        if(name == data_renderer->active_probe_name) return iter;
    }
    Gtk::TreeStore::iterator iter;
    return iter;
}

void MainWindow::on_probe_update(bool added)
{
    probe::DataProbe* probe = data_renderer->probes[data_renderer->active_probe_name];
    if(probe == nullptr) return;
    if(added)
    {
        Gtk::TreeModel::Row row = *(probelist_store->append());
        row[probelist_columns.col_name] = data_renderer->active_probe_name;
        probe->fill_row(row);
    }
    else
    {
        Gtk::TreeStore::iterator iter = search_probelist(data_renderer->active_probe_name);
        if(iter)
        {
            Gtk::TreeModel::Row row = *iter;
            probe->fill_row(row);
        }
    }
}

void MainWindow::update_probe_ui(string name)
{
    probedata->name = name;
    probedata->update_ui();
    probe::DataProbe* probe = data_renderer->probes[name];
    if(probe != nullptr)
    {
        probe::ProbeDetailsWindow* window = probe->window;
        if(window != nullptr) window->update_ui();
    }
}


void MainWindow::open_probe_ui()
{
    probe::DataProbe* probe = data_renderer->probes[data_renderer->active_probe_name];
    if(probe != nullptr)
    {
        probe::ProbeDetailsWindow** window = &(probe->window);
        if(*window == nullptr) *window = probe::ProbeDetailsWindow::create(this, data_renderer->active_probe_name);
        (*window)->update_ui();
        (*window)->show();
    }
}

void MainWindow::reset_probes()
{
    data_renderer->active_probe_name = "";
    probedata->name = "";
    probedata->reset_gui();
    for(auto& probe : data_renderer->probes)
    {
        if(probe.second == nullptr) continue;
        if(probe.second->window != nullptr)
        {
            probe.second->window->close();
            delete probe.second->window;
        }
        delete probe.second;
        probe.second = nullptr;
    }
    probelist_store->clear();
    data_renderer->probes.clear();
    data_renderer->invalidate();
}