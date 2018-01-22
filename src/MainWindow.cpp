#include "MainWindow.hpp"

using namespace swegui;

probe::ProbeColumns MainWindow::probelist_columns;

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder)
{
    setup_gui_elements();
}

MainWindow::~MainWindow()
{
    delete data_renderer;
}

MainWindow* MainWindow::create()
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_MAIN_GUI);
    MainWindow* window = nullptr;
    refBuilder->get_widget_derived("window_main", window);
    window->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    return window;
}

void MainWindow::setup_gui_elements()
{
    //Get menu bar
    m_refBuilder->get_widget("main_menubar", menubar_main);
    m_refBuilder->get_widget("mb_file_open", mb_file_open);
    m_refBuilder->get_widget("mb_file_quit", mb_file_quit);
    m_refBuilder->get_widget("mb_view_layer", mb_view_layer);
    m_refBuilder->get_widget("mb_view_zoom_in", mb_view_zoom_in);
    m_refBuilder->get_widget("mb_view_zoom_out", mb_view_zoom_out);
    m_refBuilder->get_widget("mb_view_reset", mb_view_reset);
    m_refBuilder->get_widget("mb_simulation_goto_start", mb_simulation_goto_start);
    m_refBuilder->get_widget("mb_simulation_prev", mb_simulation_prev);
    m_refBuilder->get_widget("spin_timestamp", mb_spin_timestamp);
    adjustment_timestamp = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(m_refBuilder->get_object("adjustment_timestamp"));
    m_refBuilder->get_widget("lbl_realtime", lbl_realtime);
    m_refBuilder->get_widget("mb_simulation_play", mb_simulation_play);
    m_refBuilder->get_widget("mb_simulation_next", mb_simulation_next); 
    m_refBuilder->get_widget("mb_tools_shoreanalysis", mb_tool_cda);
    m_refBuilder->get_widget("mb_tools_crosssection", mb_tool_crosssection);
    m_refBuilder->get_widget("mb_help_about", mb_help_about);
    //Get toolbar
    m_refBuilder->get_widget("main_toolbar", toolbar_main);
    m_refBuilder->get_widget("tbtn_open", tb_openfile);
    m_refBuilder->get_widget("tbtn_simulation_goto_start", tb_simulation_goto_start);
    m_refBuilder->get_widget("tbtn_simulation_prev", tb_simulation_prev);
    m_refBuilder->get_widget("tbtn_simulation_play", tb_simulation_play);
    m_refBuilder->get_widget("tbtn_simulation_next", tb_simulation_next);
    m_refBuilder->get_widget("tbtn_layer", tb_layer);
    m_refBuilder->get_widget("tbtn_zoom_out", tb_zoom_out);
    m_refBuilder->get_widget("tbtn_zoom_reset", tb_zoom_reset);
    m_refBuilder->get_widget("tbtn_zoom_in", tb_zoom_in);
    m_refBuilder->get_widget("tbtn_tool_cda", tb_tool_cda);
    m_refBuilder->get_widget("tbtn_tool_crosssection", tb_crosssection);
    //Get raw data label
    //m_refBuilder->get_widget("lbl_info", lbl_raw_data);
    m_refBuilder->get_widget("alignment_frame_probedata", alignment_frame_probedata);
    probedata = widgets::DataFieldWidget::create(this, "");
    alignment_frame_probedata->add(*probedata);
    //probe list
    m_refBuilder->get_widget("treeview_probes", probelist);
    m_refBuilder->get_widget("context_menu_probelist", contextmenu_probelist);
    probelist_store = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(m_refBuilder->get_object("liststore_probes"));
    m_refBuilder->get_widget("button_probe_add", button_probe_add);
    //Its context menu
    m_refBuilder->get_widget("menuitem_probelist_open", menuitem_probelist_open);
    m_refBuilder->get_widget("menuitem_probelist_edit", menuitem_probelist_edit);
    m_refBuilder->get_widget("menuitem_probelist_remove", menuitem_probelist_remove);
    //Get SFML control, init renderer
    m_refBuilder->get_widget("alignment_frame_sfml", alignment_frame_sfml);
    sfml_area = widgets::SFMLWidget::create(this);
    alignment_frame_sfml->add(*sfml_area);
    data_renderer = new renderer::DataRenderer(*sfml_area);
    //Dialogs
    m_refBuilder->get_widget("dialog_open", dialog_open);
    m_refBuilder->get_widget("dialog_about", dialog_about);
    dialog_probe_edit = EditProbeDialog::create(this);
    //Windows
    window_layers = LayerWindow::create(this);

    //Event handlers for menubar
    mb_file_open->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_fileopen));
    mb_file_quit->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_quit));
    mb_view_layer->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_layer));
    mb_view_zoom_in->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_zoom_in));
    mb_view_zoom_out->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_zoom_out));
    mb_view_reset->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_zoom_reset));
    mb_simulation_goto_start->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_goto_start));
    mb_simulation_prev->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_prev));
    mb_spin_timestamp->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_set_timestamp));
    mb_simulation_play->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_play));
    mb_simulation_next->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_next));
    mb_tool_cda->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_cda));
    mb_tool_crosssection->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_crosssection));
    mb_help_about->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_about));
    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_fileopen));
    tb_simulation_goto_start->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_goto_start));
    tb_simulation_prev->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_prev));
    tb_simulation_play->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_play));
    tb_simulation_next->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_next));
    tb_layer->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_layer));
    tb_zoom_out->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_zoom_out));
    tb_zoom_reset->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_zoom_reset));
    tb_zoom_in->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_zoom_in));
    tb_tool_cda->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_cda));
    tb_crosssection->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_crosssection));
    
    //Event handlers probe list
    probelist->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    probelist->signal_row_activated().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_activate));
    probelist->signal_button_press_event().connect_notify(sigc::mem_fun(this, &MainWindow::on_action_probelist_button_press));
    probelist->get_selection()->signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_changed));
    button_probe_add->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_button_probe_add));
    //And its context menu
    menuitem_probelist_open->signal_activate().connect(sigc::mem_fun(this, &MainWindow::open_probe_ui));
    menuitem_probelist_edit->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_context_edit));
    menuitem_probelist_remove->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_probe_remove));
    //Event handlers for sfml widget
    data_renderer->signal_update().connect(sigc::mem_fun(this, &MainWindow::on_probe_update));
    data_renderer->signal_select().connect(sigc::mem_fun(this, &MainWindow::on_probe_select));

    //Initialize gui elements
    initialize_gui_elements();
}
    
void MainWindow::initialize_gui_elements()
{

}

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
        lbl_realtime->set_text("0 sekunden");       //TODO: Enter real value and move line to own method
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
        handle_timestamp_change();;
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
    lbl_realtime->set_text("x sekunden");       //TODO: Enter real value
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

void MainWindow::prepare_add_edit(bool clear)
{
    dialog_probe_edit->set_min_max(data_renderer->meta_info->originx, 
        data_renderer->meta_info->originx + data_renderer->meta_info->ax(), 
        data_renderer->meta_info->originy, 
        data_renderer->meta_info->originy + data_renderer->meta_info->ay());
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