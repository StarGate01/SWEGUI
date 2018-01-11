#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>
#include <iostream>
#include "sfml/SFMLWidget.hpp"
#include "MovingCircle.hpp"

#define PATH_TO_MAIN_GUI "ui/main.glade"

class GuiApplicationWindow : public Gtk::ApplicationWindow
{

    public:

        GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

        static GuiApplicationWindow* create();

        void open_file_view(const Glib::RefPtr<Gio::File>& file);

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
        //SFML control
        SFMLWidget* sfml_area = nullptr;
        MovingCircle* moving_circle = nullptr;


        //GUI helper functions
        bool check_gui_initialized();
        //--- event handler ---
        void setup_gui_elements();
        void initialize_gui_elements();     //Called by setup_gui_elements
        static void register_custom_gui_elements();
        //Actual event handler
        static void on_action_fileopen();
        static void on_action_quit();
        static void on_action_test1();
        static void on_action_test2();

    protected:

        Glib::RefPtr<Gtk::Builder> m_refBuilder;

};

#endif