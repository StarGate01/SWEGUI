#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include "sfml/SFMLWidget.hpp"
#include "renderer/DataRenderer.hpp"
#include "probe/ProbeColumns.hpp"

#define PATH_TO_MAIN_GUI "ui/main.glade"

class GuiApplicationWindow : public Gtk::ApplicationWindow
{

    public:

        GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
        ~GuiApplicationWindow();

        static GuiApplicationWindow* create();
        void open_file_view(const Glib::RefPtr<Gio::File>& file);
        static void register_custom_gui_elements();

    private:
        //GUI Elements
        Gtk::Window* window_main = nullptr;
        Gtk::Window* window_layers = nullptr;
        //Menubar
        Gtk::MenuBar* menubar_main = nullptr;
        Gtk::MenuItem* menuitementry_file_open = nullptr;
        Gtk::MenuItem* menuitementry_tools_crosssection = nullptr;
        Gtk::MenuItem* menuitementry_help_about = nullptr;
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
        Glib::RefPtr<Gtk::ListStore> probelist_store;
        static probe::ProbeColumns probelist_columns;
        Gtk::Menu* contextmenu_probelist = nullptr;
        Gtk::Button* button_probe_add = nullptr;
        //Its context menu
        Gtk::MenuItem* menuitem_probelist_edit = nullptr;
        Gtk::MenuItem* menuitem_probelist_remove = nullptr;
        //SFML control
        sfml::SFMLWidget* sfml_area = nullptr;
        renderer::DataRenderer* data_renderer = nullptr;
        //Dialogs
        Gtk::FileChooserDialog* dialog_open = nullptr;
        Gtk::AboutDialog* dialog_about = nullptr;
        Gtk::Dialog* dialog_probe_edit = nullptr;
        //Layer switches
        Gtk::Switch* switch_b = nullptr, *switch_h = nullptr, 
            *switch_hu = nullptr, *switch_hv = nullptr, *switch_hx = nullptr;

        //Handlers
        void setup_gui_elements();
        void initialize_gui_elements();     //Called by setup_gui_elements
        //Actual event handlers
        void on_action_fileopen();
        void on_action_quit();
        void on_action_test1();
        void on_action_test2();
        void on_action_crosssection();
        void on_action_about();
        void on_action_probelist_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void on_action_probelist_button_press(GdkEventButton *event);
        void on_action_probelist_changed();
        void on_action_probelist_context_edit();
        void on_action_probelist_context_remove();
        void on_action_button_probe_add();
        void on_sfml_update(bool added);
        void on_sfml_select();
        void on_layer_switch_changed();

    protected:

        Glib::RefPtr<Gtk::Builder> m_refBuilder;
        Gtk::TreeStore::iterator search_probelist(std::string name);
        void handle_add_edit();

        // void addDataprobe(ToolDataprobe probe);
        // ToolDataprobe* getDataprobe(std::string name);
        // ToolDataprobe* getDataprobe(float x, float y);
        // void removeDataprobe(ToolDataprobe probe);      //TODO: Implement
        // void removeDataprobe(std::string name);         //TODO: Implement

        //Probe list
        // void probelist_model_init();
        // void probelist_model_update();
};

#endif