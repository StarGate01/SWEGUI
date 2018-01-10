#ifndef GTKMM_SWE_APPLICATION_H
#define GTKMM_SWE_APPLICATION_H

#include <gtkmm.h>

//Implemented according to: https://developer.gnome.org/gtkmm-tutorial/stable/sec-buildapp-trivial-app.html.en

class GuiApplicationWindow;

class GuiApplication : public Gtk::GuiApplication
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
    GuiApplicationWindow* create_appwindow();       //May be unneccessary
    void on_hide_window(Gtk::Window* window);       //May be unneccessary
};

#endif