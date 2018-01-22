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
            probe::DataProbe& probe = data_renderer->probes[name];
            dialog_probe_edit->set_name(name);
            dialog_probe_edit->set_x(probe.x);
            dialog_probe_edit->set_y(probe.y);
            dialog_probe_edit->existing = true;
            dialog_probe_edit->set_title("Edit probe");
            return;
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
        probe::DataProbe probe(dialog_probe_edit->get_x(), dialog_probe_edit->get_y());
        data_renderer->probes[name] = probe;
        added = true;
    }
    else
    {
        data_renderer->probes[name].x = dialog_probe_edit->get_x();
        data_renderer->probes[name].y = dialog_probe_edit->get_y();
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
    probe::DataProbe& probe = data_renderer->probes[data_renderer->active_probe_name];
    if(added)
    {
        Gtk::TreeModel::Row row = *(probelist_store->append());
        row[probelist_columns.col_name] = data_renderer->active_probe_name;
        probe.fill_row(row);
    }
    else
    {
        Gtk::TreeStore::iterator iter = search_probelist(data_renderer->active_probe_name);
        if(iter)
        {
            Gtk::TreeModel::Row row = *iter;
            probe.fill_row(row);
        }
    }
}

void MainWindow::on_probe_select()
{
    Gtk::TreeStore::iterator iter = search_probelist(data_renderer->active_probe_name);
    if(iter)
    {
        Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
        selection->select(iter);
        update_probe_ui(data_renderer->active_probe_name);
    }
}

void MainWindow::update_probe_ui(string name)
{
    probedata->name = name;
    probedata->update_ui();
    probe::ProbeDetailsWindow* window = data_renderer->probes[name].window;
    if(window != nullptr) window->update_ui();
}

void MainWindow::reset_probes()
{
    //For each probe
    for(auto const& probe : data_renderer->probes)
        //Close probe window
        if(probe.second.window != nullptr)
            probe.second.window->close();

    data_renderer->probes.clear();

    //Update probelist
    probelist_store->clear();
    //Update raw data window
    probedata->reset_gui();
}