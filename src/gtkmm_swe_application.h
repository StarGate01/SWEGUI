#ifndef GTKMM_SWE_APPLICATION_H
#define GTKMM_SWE_APPLICATION_H

#include <gtkmm.h>

//Implemented according to: https://developer.gnome.org/gtkmm-tutorial/stable/sec-buildapp-trivial-app.html.en

class GuiApplicationWindow;

class GuiApplication : public Gtk::Application
{
protected:
        GuiApplication();

public:
    static Glib::RefPtr<GuiApplication> create(); 

protected:
    //Override default signal handlers
    //void on_activate() override;
    //void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;

private:
    //GUI Elements
    Gtk::Window* pWindow = nullptr;
    //Menubar
    Gtk::MenuBar* menubar_main = nullptr;
    Gtk::MenuItem* menuitementry_open = nullptr;
    //Toolbar
    Gtk::Toolbar* toolbar_main = nullptr;
    Gtk::ToolButton* tb_openfile = nullptr;
    Gtk::ToolButton* tb_quit = nullptr;
    Gtk::ToolButton* tb_test1 = nullptr;
    Gtk::ToolButton* tb_test2 = nullptr;
    //Raw data field
    Gtk::Label* lbl_raw_data = nullptr;

    GuiApplicationWindow* create_appwindow();
    void on_hide_window(Gtk::Window* window);

    //GUI helper functions
    bool check_gui_initialized();
    //--- event handler ---
    void setup_gui_elements();
    void initialize_gui_elements();     //Called by setup_gui_elements
    //Actual event handler
    void on_action_fileopen();
    void on_action_quit();
    void on_action_test1();
    void on_action_test2();
};

#endif