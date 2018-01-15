#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>
#include <iostream>
#include <list>
#include <stdexcept>
#include <stdlib.h>         //Temporary include for rand
#include "ToolDataprobe.hpp"
#include "sfml/SFMLWidget.hpp"
#include "renderer/DataRenderer.hpp"

#define PATH_TO_MAIN_GUI "ui/main.glade"

class GuiApplicationWindow : public Gtk::ApplicationWindow
{

    public:

        GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

        static GuiApplicationWindow* create();

        void open_file_view(const Glib::RefPtr<Gio::File>& file);
        static void register_custom_gui_elements();

    private:
        //GUI Elements
        Gtk::Window* pWindow = nullptr;
        //Menubar
        Gtk::MenuBar* menubar_main = nullptr;
        Gtk::MenuItem* menuitementry_file_open = nullptr;
        Gtk::MenuItem* menuitementry_tools_dataprobe = nullptr;
        Gtk::MenuItem* menuitementry_tools_crosssection = nullptr;
        
        //Toolbar
        Gtk::Toolbar* toolbar_main = nullptr;
        Gtk::ToolButton* tb_openfile = nullptr;
        Gtk::ToolButton* tb_quit = nullptr;
        Gtk::ToolButton* tb_test1 = nullptr;
        Gtk::ToolButton* tb_test2 = nullptr;
        //Raw data field
        Gtk::Label* lbl_raw_data = nullptr;
        //Probelist
        Gtk::TreeView* probelist = nullptr;
        //Context menu
        Gtk::Menu* contextmenu_probelist = nullptr;
        //SFML control
        sfml::SFMLWidget* sfml_area = nullptr;
        renderer::DataRenderer* data_renderer = nullptr;

        //GUI helper functions
        bool check_gui_initialized();

        //--- event handler ---
        void setup_gui_elements();
        void initialize_gui_elements();     //Called by setup_gui_elements
        //Actual event handlers
        void on_action_fileopen();
        void on_action_quit();
        void on_action_test1();
        void on_action_test2();
        void on_action_dataprobe();
        void on_action_crosssection();
        void on_action_probelist_button_press();

    protected:
        Glib::RefPtr<Gtk::Builder> m_refBuilder;

        //List of dataprobes
        std::list<ToolDataprobe> tool_dataprobes;
        void addDataprobe(ToolDataprobe probe);
        ToolDataprobe* getDataprobe(std::string name);
        ToolDataprobe* getDataprobe(float x, float y);
        void removeDataprobe(ToolDataprobe probe);      //TODO: Implement
        void removeDataprobe(std::string name);         //TODO: Implement
};

#endif