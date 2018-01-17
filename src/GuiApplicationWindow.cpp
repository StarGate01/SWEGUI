#include "GuiApplicationWindow.hpp"

GuiApplicationWindow::GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder)
{
    setup_gui_elements();
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
    m_refBuilder->get_widget("window_main", pWindow);
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
    // m_refBuilder->get_widget("treeviewcolumn_name", probelist_name_col);
    // m_refBuilder->get_widget("treeviewcolumn_x", probelist_x_col);
    // m_refBuilder->get_widget("treeviewcolumn_y", probelist_y_col);
    // m_refBuilder->get_widget("liststore1", probelist_liststore);
    //Get SFML control, init renderer
    m_refBuilder->get_widget("sfml_area", sfml_area);
    data_renderer = new renderer::DataRenderer(*sfml_area);
    //Dialogs
    m_refBuilder->get_widget("dialog_open", dialog_open);
    m_refBuilder->get_widget("dialog_about", dialog_about);

    //--- Event handler ---
    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_fileopen));
    tb_quit->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_quit));
    tb_test1->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_test1));
    tb_test2->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_test2));
    //Event handlers for menubar
    menuitementry_file_open->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_fileopen));
    menuitementry_tools_crosssection->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_crosssection));
    menuitementry_help_about->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_about));
    //Event handlers for context menu
    probelist->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    probelist->signal_row_activated().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_activate));
    probelist->signal_button_press_event().connect_notify(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_button_press));
    probelist->get_selection()->signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_changed));
    //Event handlers for sfml widget
    data_renderer->signal_click().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_sfml_click));
    data_renderer->signal_select().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_sfml_select));
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

//Event handler
void GuiApplicationWindow::on_action_fileopen()
{
    int result = dialog_open->run();
    switch(result)
    {
        case Gtk::RESPONSE_OK:
        {
            std::string filename = dialog_open->get_filename();
            data_renderer->open(filename);
            break;
        }
        default: break;
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
    std::cout << "Action: test1 clicked" << std::endl;
}

void GuiApplicationWindow::on_action_test2()
{
    std::cout << "Action: test2 clicked" << std::endl;
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

void GuiApplicationWindow::addDataprobe(ToolDataprobe probe)
{
    //Check if name already exists
    if(getDataprobe(probe.getName()) != nullptr)
        throw std::runtime_error("Probe with this name already exists");
    
    data_renderer->probes.push_back(probe);
}

ToolDataprobe* GuiApplicationWindow::getDataprobe(std::string name)
{
    for(ToolDataprobe& p : data_renderer->probes)
        if(p.getName().compare(name) == 0) return &p;
    return nullptr;
}

ToolDataprobe* GuiApplicationWindow::getDataprobe(float x, float y)
{
    Coordinate c;
    for(ToolDataprobe& p : data_renderer->probes)
    {
        c = p.getPosition();
        if(c.x == x && c.y == y) return &p;
    }
    return nullptr;
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
    // data_renderer->active_probe = getDataprobe("probe_0");
    // data_renderer->invalidate();
}

void GuiApplicationWindow::on_sfml_click(float x, float y)
{
    std::cout << "sfml add please: " << x << ", " << y << std::endl;
    string name = "probe_" + to_string(data_renderer->probes.size());
    ToolDataprobe probe(name);
    probe.setPosition(x, y);
    addDataprobe(probe);
    data_renderer->active_probe = getDataprobe(name);
    data_renderer->invalidate();
}

void GuiApplicationWindow::on_sfml_select()
{
    std::cout << "sfml select: " << data_renderer->active_probe->getName() << std::endl;
    data_renderer->invalidate();
}