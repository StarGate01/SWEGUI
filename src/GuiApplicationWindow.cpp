#include <stdexcept>
#include <gtkmm.h>
#include "GuiApplicationWindow.hpp"
#include "sfml/SFMLWidget.hpp"
#include "renderer/DataRenderer.hpp"


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
    if(!window)
    {
        throw std::runtime_error("window_main not found in ui/main.gtk");
    }

    return window;
}

void GuiApplicationWindow::open_file_view(const Glib::RefPtr<Gio::File>& file)
{
    
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
    m_refBuilder->get_widget("imagemenuitem1", menuitementry_file_open);
    m_refBuilder->get_widget("tools_dataprobe", menuitementry_tools_dataprobe);
    m_refBuilder->get_widget("tools_crosssection", menuitementry_tools_crosssection);


    //Get toolbar
    m_refBuilder->get_widget("main_toolbar", toolbar_main);
    m_refBuilder->get_widget("tbtn_new", tb_openfile);
    m_refBuilder->get_widget("bbtn_quit", tb_quit);
    m_refBuilder->get_widget("tbtn_test1", tb_test1);
    m_refBuilder->get_widget("tbtn_test2", tb_test2);

    //Get raw data label
    m_refBuilder->get_widget("lbl_info", lbl_raw_data);

    //Context menu of probe list
    m_refBuilder->get_widget("context_menu_probelist", contextmenu_probelist);

    //Get SFML control, init demo
    m_refBuilder->get_widget("sfml_area", sfml_area);
    data_renderer = new renderer::DataRenderer(*sfml_area);

    //Check correct setup
    if(!check_gui_initialized())
    {
        perror("Failed to initialize gui");
        return;
    }

    //--- Event handler ---
    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_fileopen));
    tb_quit->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_quit));
    tb_test1->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_test1));
    tb_test2->signal_clicked().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_test2));
    //Event handlers for menubar
    menuitementry_file_open->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_fileopen));
    menuitementry_tools_dataprobe->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_dataprobe));
    menuitementry_tools_crosssection->signal_activate().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_crosssection)); 
    //Event handlers for context menu
    //probelist->signal_row_activated().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_button_press));          //Does not compile
    Glib::RefPtr<Gtk::TreeSelection> selection = probelist->get_selection();
    selection->signal_changed().connect(sigc::mem_fun(this, &GuiApplicationWindow::on_action_probelist_button_press));
    //Initialize gui elements
    initialize_gui_elements();
}
    
void GuiApplicationWindow::initialize_gui_elements()
{
    check_gui_initialized();
    //TESTING: Change label of textfield
    //TODO: Remove the following line after proove of concept
    lbl_raw_data->set_width_chars(30);
    lbl_raw_data->set_text("Hello world");
}

bool GuiApplicationWindow::check_gui_initialized()
{
    //Check window
    if(!pWindow)
    {
        perror("pWindow not initilized");
        return false;
    }

    //Check menubar
    if(menubar_main)
    {
        if(!(menuitementry_file_open && menuitementry_tools_dataprobe && menuitementry_tools_crosssection))
        {
            perror("Menubar buttons not initialized");
            return false;
        }
    }
    else
    {
        perror("Menubar not initialized");
        return false;
    }

    //Check toolbar
    if(toolbar_main)
    {
        if(!(tb_openfile && tb_quit && tb_test1 && tb_test2))
            {
                perror("Buttons of toolbar_main not initialized");
                return false;
            }
    }
    else
    {
        perror("Toolbar not initialized");
        return false;
    }

    //Check raw data label
    if(!lbl_raw_data)
    {
        perror("lbl_raw_data not initialized");
        return false;
    }

    if(!contextmenu_probelist)
    {
        perror("Context menu not initialized");
        return false;
    }

    //Check sfml control
    if(!sfml_area)
    {
        perror("sfml_area not initilized");
        return false;
    }

    return true;
}

//Event handler
void GuiApplicationWindow::on_action_fileopen()
{
    std::cout << "Action: file open clicked" << std::endl; 
}

void GuiApplicationWindow::on_action_quit()
{
    std::cout << "Action: quit clicked" << std::endl;
}

void GuiApplicationWindow::on_action_test1()
{
    std::cout << "Action: test1 clicked" << std::endl;
    //data_renderer->open("/mnt/c/Users/Christoph/Documents/Studium/5_WS1718/TSISIM/Tutorium/SWE/build/data/swe_prod_00.nc");
}

void GuiApplicationWindow::on_action_test2()
{
    std::cout << "Action: test2 clicked" << std::endl;
}

void GuiApplicationWindow::on_action_dataprobe()
{
    std::cout << "Adding random probe to list" << std::endl;
    ToolDataprobe probe("probe_" + to_string(tool_dataprobes.size()));
    probe.setPosition(((float)rand()/RAND_MAX), ((float)rand()/RAND_MAX));
    addDataprobe(probe);

    std::cout << " --- Dataprobe list ---" << std::endl;    
    for(ToolDataprobe& p : tool_dataprobes)
    {
        std::cout << p.getName() << " (x:" << p.getPosition().x << " y:" << p.getPosition().y << ")" << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}

void GuiApplicationWindow::on_action_crosssection()
{
    std::cout << "Action: crosssection clicked" << std::endl;
}

void GuiApplicationWindow::addDataprobe(ToolDataprobe probe)
{
    //Check if name already exists
    if(getDataprobe(probe.getName()) != nullptr)
            throw std::runtime_error("Probe with this name already exists");
    
    tool_dataprobes.push_back(probe);
}

void GuiApplicationWindow::on_action_probelist_button_press()
{
    std::cout << "List: Self destruction enabled... standby for blast" << std::endl;
}

ToolDataprobe* GuiApplicationWindow::getDataprobe(std::string name)
{
    for(ToolDataprobe& p : tool_dataprobes)
        if(p.getName().compare(name) == 0)
            return &p;
    return nullptr;
}

ToolDataprobe* GuiApplicationWindow::getDataprobe(float x, float y)
{
    Coordinate c;
    for(ToolDataprobe& p : tool_dataprobes)
    {
        c = p.getPosition();
        if(c.x == x && c.y == y)
            return &p;
    }
    return nullptr;
}