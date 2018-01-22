#include <exception>
#include <iostream>
#include "Application.hpp"
#include "MainWindow.hpp"

using namespace swegui;

Application::Application() 
    : Gtk::Application("de.tum.in.www5.tssim.g03.swegui", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<Application> Application::create()
{
    return Glib::RefPtr<Application>(new Application());
}

MainWindow* Application::create_appwindow()
{
    auto appwindow = MainWindow::create();

    //Run the application as long as the window is open
    add_window(*appwindow);

    //Delete the window when its hidden
    appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this, &Application::on_hide_window), appwindow));

    return appwindow;
}

void Application::on_activate()
{
    try
    {
        //Show the window
        auto appwindow = create_appwindow();
        appwindow->present();    
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
    }
    
}

void Application::on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint)
{
    //Create a window if no window already exists
    MainWindow* appwindow = nullptr;
    auto windows = get_windows();
    if(windows.size() > 0)
        appwindow = dynamic_cast<MainWindow*>(windows[0]);


    try
    {
        if(!appwindow)
        appwindow = create_appwindow();
        //For each file, open a window
        for(const auto& file : files) appwindow->open_file_view(file);
        appwindow->present();
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "Application::on_open(): " + ex.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Application::on_open(): " << ex.what() << std::endl;
    }
}

void Application::on_hide_window(Gtk::Window* window)
{
    delete window;
}