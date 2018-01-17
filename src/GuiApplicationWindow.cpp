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
    // m_refBuilder->get_widget("treeviewcolumn_name", probelist_name_col);
    // m_refBuilder->get_widget("treeviewcolumn_x", probelist_x_col);
    // m_refBuilder->get_widget("treeviewcolumn_y", probelist_y_col);
    probelist->get_model();
    if(probemodel == nullptr)
        std::cout << "Failed to init liststore_probes" << std::endl;

    //Get SFML control, init renderer
    m_refBuilder->get_widget("sfml_area", sfml_area);
    data_renderer = new renderer::DataRenderer(*sfml_area);
    //Dialogs
    m_refBuilder->get_widget("dialog_open", dialog_open);
    m_refBuilder->get_widget("dialog_about", dialog_about);
    //Layer switches
    m_refBuilder->get_widget("switch_b", switch_b);
    m_refBuilder->get_widget("switch_h", switch_h);
    m_refBuilder->get_widget("switch_hu", switch_hu);
    m_refBuilder->get_widget("switch_hv", switch_hv);
    m_refBuilder->get_widget("switch_hx", switch_hx);

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
    //Init probe list model
    probelist_model_init();

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
            int res = data_renderer->open(filename);
            cout << "Open result: " << res << endl;
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
    window_layers->show();
}

void GuiApplicationWindow::on_action_test2()
{
    std::cout << "Lets get going" << std::endl;
    probelist_model_init();
    std::cout << "Finished updates" << std::endl;
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

//TODO: Integrate this code into context menu
void GuiApplicationWindow::on_action_probelist_context_delete()
{
    Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(probelist->get_model());
    Glib::RefPtr<Gtk::TreeSelection> ref_selection = probelist->get_selection();
    Gtk::TreeModel::iterator iter = ref_selection->get_selected();
    if(iter)
    {
        std::cout << "Deleting row" << std::endl;
        model->erase(iter);
    }
    else
    {
        std::cout << "Nothing selected" << std::endl;
    }
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
    probelist_model_update();
}

void GuiApplicationWindow::on_sfml_select()
{
    std::cout << "sfml select: " << data_renderer->active_probe->getName() << std::endl;
    data_renderer->invalidate();
    probelist_model_update();
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

void GuiApplicationWindow::probelist_model_init()
{
    //TODO: Reuse the model from probelist instead of creating a new one
    //Recreate model
    Gtk::TreeModelColumnRecord columns;
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<int> col_x;
    Gtk::TreeModelColumn<int> col_y;
    columns.add(col_name);
    columns.add(col_x);
    columns.add(col_y);
    const Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(columns);
    probelist->set_model(model);

    //Save model for later use
    probes_col_name = col_name;
    probes_col_x = col_x;
    probes_col_y = col_y;

    //Set titles
    probelist->get_column(0)->set_title("Name");
    probelist->get_column(1)->set_title("X");
    probelist->get_column(2)->set_title("Y");

    probelist_model_update();
}

void GuiApplicationWindow::probelist_model_update()
{
    if(probelist == nullptr)
    {
        std::cout << "Failed to update probelist (nullptr)" << std::endl;
        return;
    }

    const Glib::RefPtr<Gtk::ListStore> model = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(probelist->get_model());      //Get model
    model->clear();     //Clear all lines
    //Add lines

    int index = 0;
    int active_index = -1;
    for(auto probe : data_renderer->probes)
    {
        Gtk::TreeModel::Row row = *(model->append());
        row[probes_col_name] = Glib::ustring(probe.getName());
        row[probes_col_x] = probe.getPosition().x;
        row[probes_col_y] = probe.getPosition().y;
        if(probe.equals(*(data_renderer->active_probe)))
        {
            //row[probes_col_name] = Glib::ustring(">" + probe.getName() + " <");
            active_index = index;
        }
        index++;
    }

    //Select active probe
    if(data_renderer->active_probe == nullptr || active_index < 0)
    {
        std::cout << "Nothing selected" << std::endl;
        return;
    }

    std::cout << "AI: " << active_index << std::endl;
    Gtk::TreeModel::iterator iter;
    int i = 0;
    for(iter = model->children().begin(); iter != model->children().end(); ++iter)
    {
        if(i == active_index)
            break;
        i++;
    } 
    
    Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
    selection->select(iter);
}