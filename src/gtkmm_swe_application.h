#ifndef GTKMM_SWE_APPLICATION_H
#define GTKMM_SWE_APPLICATION_H

#include <iostream>
#include <gtkmm.h>
#include "gtkmm_swe_guiapplicationwindow.h"
#include "GuiEnums.hpp"

//Implemented according to: https://developer.gnome.org/gtkmm-tutorial/stable/sec-buildapp-trivial-app.html.en



class GuiApplication : public Gtk::Application
{
protected:
        GuiApplication();

public:
    static Glib::RefPtr<GuiApplication> create(); 

protected:
    //Override default signal handlers
    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;

private:
    GuiApplicationWindow* create_appwindow();
    void on_hide_window(Gtk::Window* window);

    //Window signal handler
    void on_window_action(Signal signal);
};

#endif