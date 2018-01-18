#include "GuiApplicationWindow.hpp"

using namespace swegui;

probe::ProbeColumns GuiApplicationWindow::probelist_columns;

GuiApplicationWindow::GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder)
{
    setup_gui_elements();
}

GuiApplicationWindow::~GuiApplicationWindow()
{
    delete data_renderer;
}

GuiApplicationWindow* GuiApplicationWindow::create()
{
    //Load builder and instantiate widget
    auto refBuilder = Gtk::Builder::create_from_file(PATH_TO_MAIN_GUI);
    GuiApplicationWindow* window = nullptr;
    refBuilder->get_widget_derived("window_main", window);
    //Add event mask
    window->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    return window;
}

void GuiApplicationWindow::register_custom_gui_elements()
{
    sfml::SFMLWidget::register_type();
}

void GuiApplicationWindow::setup_gui_elements()
{
    //Get window
    m_refBuilder->get_widget("window_main", window_main);
    m_refBuilder->get_widget("window_layers", window_layers);
    //Get menu bar
    m_refBuilder->get_widget("main_menubar", menubar_main);
    m_refBuilder->get_widget("file_open", menuitementry_file_open);
    m_refBuilder->get_widget("tools_crosssection", menuitementry_tools_crosssection);
    m_refBuilder->get_widget("help_about", menuitementry_help_about);
    //Get toolbar
    m_refBuilder->get_widget("main_toolbar", toolbar_main);
    m_refBuilder->get_widget("tbtn_open", tb_openfile);
    m_refBuilder->get_widget("bbtn_quit", tb_quit);
    m_refBuilder->get_widget("tbtn_test1", tb_test1);
    m_refBuilder->get_widget("tbtn_test2", tb_test2);
    //Get raw data label
    m_refBuilder->get_widget("lbl_info", lbl_raw_data);
    //probe list
    m_refBuilder->get_widget("treeview_probes", probelist);
    m_refBuilder->get_widget("context_menu_probelist", contextmenu_probelist);
    probelist_store = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(m_refBuilder->get_object("liststore_probes"));
    m_refBuilder->get_widget("button_probe_add", button_probe_add);
    //Its context menu
    m_refBuilder->get_widget("menuitem_probelist_edit", menuitem_probelist_edit);
    m_refBuilder->get_widget("menuitem_probelist_remove", menuitem_probelist_remove);
    //Get SFML control, init renderer
    m_refBuilder->get_widget("sfml_area", sfml_area);
    data_renderer = new renderer::DataRenderer(*sfml_area);
    //Dialogs
    m_refBuilder->get_widget("dialog_open", dialog_open);
    m_refBuilder->get_widget("dialog_about", dialog_about);
    m_refBuilder->get_widget("dialog_probe_edit", dialog_probe_edit);
    //Layer switches
    m_refBuilder->get_widget("switch_b", switch_b);
    m_refBuilder->get_widget("switch_h", switch_h);
    m_refBuilder->get_widget("switch_hu", switch_hu);
    m_refBuilder->get_widget("switch_hv", switch_hv);
    m_refBuilder->get_widget("switch_hx", switch_hx);

-
    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_fileopen));
    tb_quit->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_quit));
    tb_test1->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_test1));
    tb_test2->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_test2));
    //Event handlers for menubar
    menuitementry_file_open->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_fileopen));
    menuitementry_tools_crosssection->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_crosssection));
    menuitementry_help_about->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_about));
    //Event handlers probe list
    probelist->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    probelist->signal_row_activated().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_activate));
    probelist->signal_button_press_event().connect_notify(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_button_press));
    probelist->get_selection()->signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_changed));
    button_probe_add->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_button_probe_add));
    //And its context menu
    menuitem_probelist_edit->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_context_edit));
    menuitem_probelist_remove->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_context_remove));
    //Event handlers for sfml widget
    data_renderer->signal_update().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_sfml_update));
    data_renderer->signal_select().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_sfml_select));
    //Layer switches
    switch_b->property_active().signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_layer_switch_changed));
    switch_h->property_active().signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_layer_switch_changed));
    switch_hu->property_active().signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_layer_switch_changed));
    switch_hv->property_active().signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_layer_switch_changed));
    switch_hx->property_active().signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_layer_switch_changed));

    //Initialize gui elements
    initialize_gui_elements();
}
    
void GuiApplicationWindow::initialize_gui_elements()
{
    //TESTING: Change label of textfield
    //TODO: Remove the following line after proof of concept
    lbl_raw_data->set_width_chars(30);
    lbl_raw_data->set_text("Hello world");
}

void GuiApplicationWindow::on_action_fileopen()
{
    if(dialog_open->run() == Gtk::RESPONSE_OK)
    {
        std::string filename = dialog_open->get_filename();
        int res = data_renderer->open(filename);
        cout << "Open result: " << res << endl;
    }
    dialog_open->hide();
}

void GuiApplicationWindow::open_file_view(const Glib::RefPtr<Gio::File>& file)
{
    //data_renderer->open(file->get_path());
}

void GuiApplicationWindow::on_action_quit()
{
    Gtk::Main::quit();
}

void GuiApplicationWindow::on_action_test1()
{
    window_layers->show();
}

void GuiApplicationWindow::on_action_test2()
{
    std::cout << "Lets get going" << std::endl;
}

void GuiApplicationWindow::on_action_crosssection()
{
    std::cout << "Action: crosssection clicked" << std::endl;
}

void GuiApplicationWindow::on_action_about()
{
    dialog_about->run();
    dialog_about->hide();
}

void GuiApplicationWindow::on_action_probelist_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    std::cout << "List: Self destruction enabled... standby for blast" << std::endl;
}

void GuiApplicationWindow::on_action_probelist_button_press(GdkEventButton *event)
{
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 3))
    {
        contextmenu_probelist->popup(event->button, event->time);
    }
}

void GuiApplicationWindow::on_action_probelist_changed()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        data_renderer->active_probe_name = row[probelist_columns.col_name];
        data_renderer->invalidate();
    }
}

void GuiApplicationWindow::on_action_probelist_context_edit()
{
    if(dialog_probe_edit->run() == Gtk::RESPONSE_OK) handle_add_edit();
    dialog_probe_edit->hide();
}

void GuiApplicationWindow::on_action_probelist_context_remove()
{
    Gtk::TreeModel::iterator iter = probelist->get_selection()->get_selected();
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        data_renderer->probes.erase(row[probelist_columns.col_name]);
        probelist_store->erase(iter);
        data_renderer->invalidate();
    }
}

void GuiApplicationWindow::on_action_button_probe_add()
{
    if(dialog_probe_edit->run() == Gtk::RESPONSE_OK) handle_add_edit();
    dialog_probe_edit->hide();
}

void GuiApplicationWindow::handle_add_edit()
{
    cout << "Add or modify probe" << endl;
}

Gtk::TreeStore::iterator GuiApplicationWindow::search_probelist(std::string name)
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

void GuiApplicationWindow::on_sfml_update(bool added)
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

void GuiApplicationWindow::on_sfml_select()
{
    Gtk::TreeStore::iterator iter = search_probelist(data_renderer->active_probe_name);
    if(iter)
    {
        Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
        selection->select(iter);
    }
}

void GuiApplicationWindow::on_layer_switch_changed()
{
    data_renderer->b.enable = switch_b->get_active();
    data_renderer->h.enable = switch_h->get_active();
    data_renderer->hu.enable = switch_hu->get_active();
    data_renderer->hv.enable = switch_hv->get_active();
    data_renderer->hx.enable = switch_hx->get_active();
    data_renderer->update_shader();
}