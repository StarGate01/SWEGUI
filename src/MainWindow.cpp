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
    auto refBuilder = Gtk::Builder::create_from_file(PATH_TO_MAIN_GUI);
    MainWindow* window = nullptr;
    refBuilder->get_widget_derived("window_main", window);
    window->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    return window;
}

void MainWindow::register_custom_gui_elements()
{
    sfml::SFMLWidget::register_type();
}

void MainWindow::setup_gui_elements()
{
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
    dialog_probe_edit = EditProbeDialog::create(this);
    //Windows
    window_layers = LayerWindow::create(this);

    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_fileopen));
    tb_quit->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_quit));
    tb_test1->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_test1));
    tb_test2->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_test2));
    //Event handlers for menubar
    menuitementry_file_open->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_fileopen));
    menuitementry_tools_crosssection->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_crosssection));
    menuitementry_help_about->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_about));
    //Event handlers probe list
    probelist->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    probelist->signal_row_activated().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_activate));
    probelist->signal_button_press_event().connect_notify(sigc::mem_fun(this, &MainWindow::on_action_probelist_button_press));
    probelist->get_selection()->signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_changed));
    button_probe_add->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_button_probe_add));
    //And its context menu
    menuitem_probelist_edit->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_context_edit));
    menuitem_probelist_remove->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_context_remove));
    //Event handlers for sfml widget
    data_renderer->signal_update().connect(sigc::mem_fun(this, &MainWindow::on_sfml_update));
    data_renderer->signal_select().connect(sigc::mem_fun(this, &MainWindow::on_sfml_select));

    //Initialize gui elements
    initialize_gui_elements();
}
    
void MainWindow::initialize_gui_elements()
{
    //TESTING: Change label of textfield
    //TODO: Remove the following line after proof of concept
    lbl_raw_data->set_width_chars(30);
    lbl_raw_data->set_text("Hello world");
}

void MainWindow::on_action_fileopen()
{
    if(dialog_open->run() == Gtk::RESPONSE_OK)
    {
        std::string filename = dialog_open->get_filename();
        int res = data_renderer->open(filename);
        cout << "Open result: " << res << endl;
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

void MainWindow::on_action_test1()
{
    window_layers->show();
}

void MainWindow::on_action_test2()
{
    std::cout << "Lets get going" << std::endl;
}

void MainWindow::on_action_crosssection()
{
    std::cout << "Action: crosssection clicked" << std::endl;
}

void MainWindow::on_action_about()
{
    dialog_about->run();
    dialog_about->hide();
}

void MainWindow::on_action_probelist_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
    std::cout << "List: Self destruction enabled... standby for blast" << std::endl;
}

void MainWindow::on_action_probelist_button_press(GdkEventButton *event)
{
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 3))
    {
        contextmenu_probelist->popup(event->button, event->time);
    }
}

void MainWindow::on_action_probelist_changed()
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

void MainWindow::on_action_probelist_context_edit()
{
    if(dialog_probe_edit->run() == Gtk::RESPONSE_OK) handle_add_edit();
    dialog_probe_edit->hide();
}

void MainWindow::on_action_probelist_context_remove()
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

void MainWindow::on_action_button_probe_add()
{
    if(dialog_probe_edit->run() == Gtk::RESPONSE_OK) handle_add_edit();
    dialog_probe_edit->hide();
}

void MainWindow::handle_add_edit()
{
    cout << "Add or modify probe" << endl;
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

void MainWindow::on_sfml_update(bool added)
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

void MainWindow::on_sfml_select()
{
    Gtk::TreeStore::iterator iter = search_probelist(data_renderer->active_probe_name);
    if(iter)
    {
        Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
        selection->select(iter);
    }
}