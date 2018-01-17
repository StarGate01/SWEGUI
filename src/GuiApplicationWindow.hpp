#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>
#include <glib.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <gdk/gdk.h>
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
        Gtk::ListStore* probemodel = nullptr;
        
        Gtk::TreeModelColumn<Glib::ustring> probes_col_name;
        Gtk::TreeModelColumn<int> probes_col_x;
        Gtk::TreeModelColumn<int> probes_col_y;

        Gtk::Menu* contextmenu_probelist = nullptr;
        // Gtk::TreeModelColumn<gchararray>* probelist_name_col = nullptr;
        // Gtk::TreeModelColumn<gfloat>* probelist_x_col = nullptr, *probelist_y_col = nullptr;
        // Gtk::ListStore* probelist_liststore = nullptr;
        //SFML control
        sfml::SFMLWidget* sfml_area = nullptr;
        renderer::DataRenderer* data_renderer = nullptr;
        //Dialogs
        Gtk::FileChooserDialog* dialog_open = nullptr;
        Gtk::AboutDialog* dialog_about = nullptr;
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
        void on_action_probelist_context_delete();
        void on_sfml_click(float x, float y);
        void on_sfml_select();
        void on_layer_switch_changed();

    protected:
        Glib::RefPtr<Gtk::Builder> m_refBuilder;

        void addDataprobe(ToolDataprobe probe);
        ToolDataprobe* getDataprobe(std::string name);
        ToolDataprobe* getDataprobe(float x, float y);
        void removeDataprobe(ToolDataprobe probe);      //TODO: Implement
        void removeDataprobe(std::string name);         //TODO: Implement

        //Probe list
        void probelist_model_init();
        void probelist_model_update();
};

#endif