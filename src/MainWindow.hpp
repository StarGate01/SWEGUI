#ifndef GUI_APPLICATION_WINDOW_H
#define GUI_APPLICATION_WINDOW_H

#include <gtkmm.h>
#include <gtkmm/adjustment.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include "LayerWindow.hpp"
#include "EditProbeDialog.hpp"
#include "widgets/SFMLWidget.hpp"
#include "renderer/DataRenderer.hpp"
#include "probe/ProbeColumns.hpp"
#include "widgets/DataFieldWidget.hpp"

#define PATH_TO_MAIN_GUI "/main/src/ui/main.glade"

namespace swegui
{

    class MainWindow : public Gtk::ApplicationWindow
    {

        public:

            MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            ~MainWindow();
            static MainWindow* create();

            void open_file_view(const Glib::RefPtr<Gio::File>& file);

            renderer::DataRenderer* data_renderer = nullptr;

        private:

            //Menubar
            Gtk::MenuBar* menubar_main = nullptr;
            Gtk::MenuItem* mb_file_open = nullptr;
            Gtk::MenuItem* mb_file_quit = nullptr;            
            Gtk::MenuItem* mb_view_layer = nullptr;
            Gtk::MenuItem* mb_view_zoom_in = nullptr;
            Gtk::MenuItem* mb_view_zoom_out = nullptr;
            Gtk::MenuItem* mb_view_reset = nullptr;
            Gtk::MenuItem* mb_simulation_goto_start = nullptr;
            Gtk::SpinButton* mb_spin_timestamp = nullptr;
            Glib::RefPtr<Gtk::Adjustment> adjustment_timestamp;
            Gtk::MenuItem* mb_simulation_play = nullptr;
            Gtk::MenuItem* mb_simulation_next = nullptr;
            Gtk::MenuItem* mb_simulation_prev = nullptr;         
            Gtk::MenuItem* mb_tool_cda = nullptr;
            Gtk::MenuItem* mb_tool_crosssection = nullptr;
            Gtk::MenuItem* mb_help_about = nullptr;
            //Toolbar
            Gtk::Toolbar* toolbar_main = nullptr;
            Gtk::ToolButton* tb_openfile = nullptr;
            Gtk::ToolButton* tb_simulation_goto_start = nullptr;
            Gtk::ToolButton* tb_simulation_prev = nullptr;
            Gtk::ToolButton* tb_simulation_play = nullptr;
            Gtk::ToolButton* tb_simulation_next = nullptr;            
            Gtk::ToolButton* tb_layer = nullptr;
            Gtk::ToolButton* tb_zoom_out = nullptr;
            Gtk::ToolButton* tb_zoom_reset = nullptr;
            Gtk::ToolButton* tb_zoom_in = nullptr;
            Gtk::ToolButton* tb_tool_cda = nullptr;
            Gtk::ToolButton* tb_crosssection = nullptr;
            
            //Raw data field
            Gtk::Alignment* alignment_frame_probedata = nullptr;
            widgets::DataFieldWidget* probedata = nullptr;
            //Probelist
            Gtk::TreeView* probelist = nullptr;
            Glib::RefPtr<Gtk::ListStore> probelist_store;
            static probe::ProbeColumns probelist_columns;
            Gtk::Menu* contextmenu_probelist = nullptr;
            Gtk::Button* button_probe_add = nullptr;
            //Its context menu
            Gtk::MenuItem* menuitem_probelist_open = nullptr;
            Gtk::MenuItem* menuitem_probelist_edit = nullptr;
            Gtk::MenuItem* menuitem_probelist_remove = nullptr;
            //SFML control
            Gtk::Alignment* alignment_frame_sfml = nullptr;
            widgets::SFMLWidget* sfml_area = nullptr;
            //Additional dialogs
            Gtk::FileChooserDialog* dialog_open = nullptr;
            Gtk::AboutDialog* dialog_about = nullptr;
            EditProbeDialog* dialog_probe_edit = nullptr;
            //Additional windows
            LayerWindow* window_layers = nullptr;

            //Handlers
            void setup_gui_elements();
            void initialize_gui_elements();     //Called by setup_gui_elements
            //Actual event handlers
            void on_action_fileopen();
            void on_action_quit();
            void on_action_simulation_goto_start();
            void on_action_simulation_prev();
            void on_action_simulation_play();
            void on_action_simulation_next();
            void on_action_simulation_set_timestamp();
            void on_action_layer();
            void on_action_zoom_out();
            void on_action_zoom_reset();
            void on_action_zoom_in();
            void on_action_cda();
            void on_action_crosssection();
            void on_action_about();
            void on_action_probelist_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
            void on_action_probelist_button_press(GdkEventButton *event);
            void on_action_probelist_changed();
            void on_action_probelist_context_edit();
            void on_probe_remove();
            void on_action_button_probe_add();
            void on_probe_update(bool added);
            void on_probe_select();

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;
            Gtk::TreeStore::iterator search_probelist(std::string name);
            void prepare_add_edit(bool clear);
            void handle_add_edit();
            void update_probe_ui(string name);
            void open_probe_ui();
            void handle_timestamp_change();
            void reset_probes();

    };

}

#endif