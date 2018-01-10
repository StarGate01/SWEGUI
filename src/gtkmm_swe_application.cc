#include "gtkmm_swe_application.h"
#include "gtkmm_swe_guiapplicationwindow.h"

GuiApplication::GuiApplication() : Gtk::Application("de.tum.in.www5.tssim.g03.swegui", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<GuiApplication> GuiApplication::create()
{
    return Glib::RefPtr<GuiApplication>(new GuiApplication());
}

GuiApplication* GuiApplication::create_appwindow()
{
    auto appwindow = new GuiApplicationWindow();

    //Run the application as long as the window is open
    add_window(*appwindow);

    //TODO: Adapt code!!
    //---- Load GtkBuilder ----
    auto refBuilder = Gtk::Builder::create();
    try
    {
        refBuilder->add_from_file("ui/main.gtk");
    }
    catch (const Glib::FileError &ex)
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Glib::MarkupError &ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Gtk::BuilderError &ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    //Setup GUI widgets
    setup_gui_elements(refBuilder);

    //Delete the window when its hidden
    appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this, &GuiApplication::on_hide_window), appwindow));

    return appwindow;
}

void GuiApplication::on_activate()
{
    //Show the window
    auto appwindow = create_appwindow();
    appwindow->present();
}

void GuiApplication::on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint)
{
    //Create a window if no window already exists
    GuiApplicationWindow* appwindow = nullptr;
    auto windows = get_windows();
    if(windows.size() > 0)
        appwindow = dynamic_cast<GuiApplicationWindow*>(windows[0]);

    if(!appwindow)
        appwindow = create_appwindow();
    
    //For each file, open a window
    for(const auto& file : files)
        appwindow->open_file_view(file);

    appwindow->present();
}

void GuiApplication::on_hide_window(Gtk::Window* window)
{
    delete window;
}

void GuiApplication::check_gui_initialized()
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
        if(!menuitementry_open)
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

    return true;
}

void GuiApplication::setup_gui_elements(Glib::RefPtr<Builder> refbuilder)
{
    //Get window
    refBuilder->get_widget("window_main", pWindow);

    //Get menu bar
    refBuilder->get_widget("main_menubar", menubar_main);
    refBuilder->get_widget("imagemenuitem1", menuitementry_open);

    //Get toolbar
    refBuilder->get_widget("main_toolbar", toolbar_main);
    refBuilder->get_widget("tbtn_new", tb_openfile);
    refBuilder->get_widget("bbtn_quit", tb_quit);
    refBuilder->get_widget("tbtn_test1", tb_test1);
    refBuilder->get_widget("tbtn_test2", tb_test2);

    //Get raw data label
    refBuilder->get_widget("lbl_info", lbl_raw_data);

    //Check correct setup
    if!(check_gui_initialized())
    {
        perror("Failed to initialize gui");
        return;
    }

    //--- Event handler ---
    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::ptr_fun(GuiApplication::on_action_fileopen));
    tb_quit->signal_clicked().connect(sigc::ptr_fun(GuiApplication::on_action_quit));
    tb_test1->signal_clicked().connect(sigc::ptr_fun(GuiApplication::on_action_test1));
    tb_test2->signal_clicked().connect(sigc::ptr_fun(GuiApplication::on_action_test2));
    //Event handlers for menubar
    menuitementry_open->signal_activate().connect(sigc::ptr_fun(on_button_clicked_newfile));

    //Initialize gui elements
    initialize_gui_elements();
}

GuiApplication::initialize_gui_elements()
{
    check_gui_initialized();
    //TESTING: Change label of textfield
    //TODO: Remove the following line after proove of concept
    lbl_raw_data->set_width_chars(30);
    lbl_raw_data->set_text("Hello world");
}

void GuiApplication::on_action_fileopen()
{
    std::cout << "Action: file open clicked" << std::endl; 
}

void GuiApplication::on_action_quit()
{
    std::cout << "Action: quit clicked" << std::endl;
}

void GuiApplication::on_action_test1()
{
    std::cout << "Action: test1 clicked" << std::endl;
}

void GuiApplication::on_action_test2()
{
    std::cout << "Action: test2 clicked" << std::endl;
}