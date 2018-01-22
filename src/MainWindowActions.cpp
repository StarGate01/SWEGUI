#include "MainWindow.hpp"

using namespace swegui;

void MainWindow::on_action_fileopen()
{
    if(dialog_open->run() == Gtk::RESPONSE_OK)
    {
        //Close all probes
        reset_probes();
        std::string filename = dialog_open->get_filename();
        int res = data_renderer->open(filename);
        //Show message box if opening file fails
        if(res != 0)
        {
            Gtk::MessageDialog d(*this, "Ooooops! Opening this file failed. Sorry!");
            d.run();
            dialog_open->hide();
            return;
        }
        cout << "Open result: " << res << endl;
        adjustment_timestamp->set_upper(data_renderer->meta_info->timestamps - 1);    //Set max frame of spin_timestamp
        lbl_realtime->set_text(std::to_string(data_renderer->get_current_time()));
        window_layers->update_ui();
    }
    dialog_open->hide();
}

void MainWindow::open_file_view(const Glib::RefPtr<Gio::File>& file)
{
    //data_renderer->open(file->get_path());
}

void MainWindow::on_action_quit()
{
    Gtk::Main::quit();
}

void MainWindow::on_action_simulation_goto_start()
{
    int current_ts = data_renderer->get_current_timestamp();
    if(current_ts != 0)
    {
        mb_spin_timestamp->set_value(0);
        handle_timestamp_change();
    }
}

void MainWindow::on_action_simulation_prev()
{
    int current_ts = data_renderer->get_current_timestamp();
    if(current_ts > 0)
    {
        mb_spin_timestamp->set_value(current_ts - 1);
        handle_timestamp_change();
    }
}

//Main handler for timestamp change
void MainWindow::on_action_simulation_set_timestamp()
{
    handle_timestamp_change();
}

void MainWindow::on_action_simulation_play()
{
    std::cout << "Clicked simulation play" << std::endl;
}

void MainWindow::on_action_simulation_next()
{
    int current_ts = data_renderer->get_current_timestamp();
    if(current_ts < data_renderer->meta_info->timestamps)
    {
        mb_spin_timestamp->set_value(current_ts + 1);
        handle_timestamp_change();
    }   
}

void MainWindow::handle_timestamp_change()
{
    data_renderer->select_timestamp(mb_spin_timestamp->get_value());
    probedata->update_ui();
    for (auto const& probe : data_renderer->probes)
    {
        if(probe.second.window != nullptr) probe.second.window->update_ui();
    }
    data_renderer->update_shader();
    lbl_realtime->set_text(std::to_string(data_renderer->get_current_time()));
}

void MainWindow::on_action_layer()
{
    window_layers->show();
}

void MainWindow::on_action_zoom_out()
{
    std::cout << "Clicked zoom out" << std::endl;
}

void MainWindow::on_action_zoom_reset()
{
    std::cout << "Clicked zoom reset" << std::endl;
}

void MainWindow::on_action_zoom_in()
{
    std::cout << "Clicked zoom in" << std::endl;
}

void MainWindow::on_action_cda()
{
    std::cout << "Clicked cda" << std::endl;
}

void MainWindow::on_action_crosssection()
{
    std::cout << "Clicked crossection" << std::endl;
}

void MainWindow::on_action_about()
{
    dialog_about->run();
    dialog_about->hide();
}

void MainWindow::on_action_probelist_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    open_probe_ui();
}

void MainWindow::open_probe_ui()
{
    probe::ProbeDetailsWindow** window = &(data_renderer->probes[data_renderer->active_probe_name].window);
    if(*window == nullptr) *window = probe::ProbeDetailsWindow::create(this, data_renderer->active_probe_name);
    (*window)->update_ui();
    (*window)->show();
}

void MainWindow::on_action_probelist_button_press(GdkEventButton *event)
{
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 3)
        && probelist->get_selection()->count_selected_rows() > 0)
            contextmenu_probelist->popup(event->button, event->time);
}

void MainWindow::on_action_probelist_changed()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        data_renderer->active_probe_name = row[probelist_columns.col_name];
        update_probe_ui(data_renderer->active_probe_name);
        data_renderer->invalidate();
    }
}

void MainWindow::on_action_probelist_context_edit()
{
    prepare_add_edit(false);
    if(dialog_probe_edit->run() == Gtk::RESPONSE_OK) handle_add_edit();
    dialog_probe_edit->hide();
}

void MainWindow::on_probe_remove()
{
    Gtk::TreeModel::iterator iter = probelist->get_selection()->get_selected();
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        string name = row[probelist_columns.col_name];
        probe::ProbeDetailsWindow* window = data_renderer->probes[name].window;
        if(window != nullptr) window->hide();
        data_renderer->probes.erase(name);
        probelist_store->erase(iter);
        data_renderer->invalidate();
    }
}

void MainWindow::on_action_button_probe_add()
{
    prepare_add_edit(true);
    if(dialog_probe_edit->run() == Gtk::RESPONSE_OK) handle_add_edit();
    dialog_probe_edit->hide();
}