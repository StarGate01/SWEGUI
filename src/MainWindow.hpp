/**
 * @file MainWindow.hpp
 * @brief Main GUI-Window of the application
*/

#ifndef GUI_APPLICATION_WINDOW_H
#define GUI_APPLICATION_WINDOW_H

#include <gtkmm.h>
#include <gtkmm/adjustment.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include "LayerWindow.hpp"
#include "RendererWindow.hpp"
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
            /**
             * @brief Overloaded constructor from Gtk::ApplicationWindow
             * @param cobject Calling object
             * @param refBuilder Gtk::Builder, from which the window is created
            */
            MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

            /**
             * @brief Overloaded destructor from Gtk::ApplicationWindow
            */
            ~MainWindow();

            /**
             * @brief Creates a new Gtk::MainWindow
             * @return New instance of Gtk::MainWindow
            */
            static MainWindow* create();

            //TODO: Remove method
            void open_file_view(const Glib::RefPtr<Gio::File>& file);

            renderer::DataRenderer* data_renderer = nullptr; ///< DataRenderer of this window

        private:

            //Menubar
            Gtk::MenuBar* menubar_main = nullptr;                   ///<Object pointer to menubar
            Gtk::MenuItem* mb_file_open = nullptr;                  ///<Object pointer to file open button in menubar
            Gtk::MenuItem* mb_file_save_screenshot = nullptr;       ///<Object pointer to save screenshot button in menubar
            Gtk::MenuItem* mb_file_quit = nullptr;                  ///<Object pointer to quit button in menubar
            Gtk::MenuItem* mb_view_layer = nullptr;                 ///<Object pointer to layer button in menubar
            Gtk::MenuItem* mb_view_renderer = nullptr;              ///<Object pointer to renderer button in menubar
            Gtk::MenuItem* mb_view_zoom_in = nullptr;               ///<Object pointer to zoom in button in menubar
            Gtk::MenuItem* mb_view_zoom_out = nullptr;              ///<Object pointer to zoom out button in menubar
            Gtk::MenuItem* mb_view_reset = nullptr;                 ///<Object pointer to reset button in menubar
            Gtk::MenuItem* mb_simulation_goto_start = nullptr;      ///<Object pointer to Go to Start button in menubar
            Gtk::SpinButton* mb_spin_timestamp = nullptr;           ///<Object pointer to timestamp Gtk::SpinButton in menubar
            Glib::RefPtr<Gtk::Adjustment> adjustment_timestamp;     ///<Object pointer to Gtk::Adjustment object used for spin_timestamp in menubar
            Gtk::MenuItem* mb_simulation_play = nullptr;            ///<Object pointer to play button in menubar
            Gtk::MenuItem* mb_simulation_next = nullptr;            ///<Object pointer to next frame button in menubar
            Gtk::MenuItem* mb_simulation_prev = nullptr;            ///<Object pointer to previous frame button in menubar
            Gtk::MenuItem* mb_tool_cda = nullptr;                   ///<Object pointer to coastal damage analysis tool button in menubar
            Gtk::MenuItem* mb_tool_crosssection = nullptr;          ///<Object pointer to crosssection tool button in menubar
            Gtk::MenuItem* mb_help_about = nullptr;                 ///<Object pointer to About button in menubar
            //Toolbar
            Gtk::Toolbar* toolbar_main = nullptr;                   ///<Object pointer to toolbar           //TODO: Remove object
            Gtk::ToolButton* tb_openfile = nullptr;                 ///<Object pointer to open file button in toolbar
            Gtk::ToolButton* tb_simulation_goto_start = nullptr;    ///<Object pointer to simulation go to start button in toolbar
            Gtk::ToolButton* tb_simulation_play = nullptr;          ///<Object pointer to play button in toolbar
            Gtk::ToolButton* tb_layer = nullptr;                    ///<Object pointer to layer selection button in toolbar
            Gtk::ToolButton* tb_renderer = nullptr;                 ///<Object pointer to renderer button in toolbar
            Gtk::ToolButton* tb_zoompan_reset = nullptr;            ///<Object pointer to zoom and pan reset button in toolbar
            Gtk::ToolButton* tb_tool_cda = nullptr;                 ///<Object pointer to coastal damage analysis tool button in toolbar
            Gtk::ToolButton* tb_crosssection = nullptr;             ///<Object pointer to crosssection tool button in toolbar
            Gtk::ToolButton* tb_screenshot = nullptr;               ///<Object pointer to screenshot export button in toolbar
            //Raw data field
            Gtk::Alignment* alignment_frame_probedata = nullptr;    ///<Object pointer to frame of raw-data-field
            widgets::DataFieldWidget* probedata = nullptr;          ///<Object pointer to widgets::DataFieldWidget of raw-data-field
            //Probelist
            Gtk::TreeView* probelist = nullptr;                     ///<Object pointer to Gtk::TreeView of probe list
            Glib::RefPtr<Gtk::ListStore> probelist_store;           ///<Glib::RefPtr to data model, which is a Gtk::ListStore of probe list
            static probe::ProbeColumns probelist_columns;           ///<Column definition of probe list
            Gtk::Menu* contextmenu_probelist = nullptr;             ///<Object pointer to context menu of probe list
            Gtk::Button* button_probe_add = nullptr;                ///<Object pointer to add button of probe list
            //Footer
            Gtk::Label* lbl_realtime = nullptr;                     ///<Object pointer to time label in footer
            //Its context menu
            Gtk::MenuItem* menuitem_probelist_open = nullptr;       ///<Object pointer to file open button of the context menu
            Gtk::MenuItem* menuitem_probelist_edit = nullptr;       ///<Object pointer to edit  button of the context menu
            Gtk::MenuItem* menuitem_probelist_remove = nullptr;     ///<Object pointer to remove button of the context menu
            //SFML control
            Gtk::Alignment* alignment_frame_sfml = nullptr;         ///<Object pointer to Gtk::Alignment information of the sfml widget (where the map is rendered)
            widgets::SFMLWidget* sfml_area = nullptr;               ///<Object pointer to the sfml widget (where the map is rendered)
            //Additional dialogs
            Gtk::FileChooserDialog* dialog_open = nullptr;          ///<Object pointer to the Gtk::FileChooserDialog, where the user can select a file to open
            Gtk::FileChooserDialog* dialog_save = nullptr;          ///<Object pointer to the Gtk::FileChooserDialog, where the user can select a file to save
            Gtk::AboutDialog* dialog_about = nullptr;               ///<Object pointer to the about dialog
            EditProbeDialog* dialog_probe_edit = nullptr;           ///<Object pointer to the probe edit dialog
            //Additional windows
            LayerWindow* window_layers = nullptr;                   ///<Object pointer to the layer window
            RendererWindow* window_renderer = nullptr;              ///<Object pointer to the renderer window

            //Handlers
            void setup_gui_elements();                              ///<Grabs all objects from PATH_TO_MAIN_GUI and initilizes event handler 
            //Actual event handlers
            void on_action_fileopen();                              ///<Event handler to open a file
            void on_action_quit();                                  ///<Event handler to close the application
            void on_action_simulation_goto_start();                 ///<Event handler to reset the simulation to the timestamp 0
            void on_action_simulation_prev();                       ///<Event handler to decrease the rendered timestamp by 1
            void on_action_simulation_play();                       ///<Event handler to run the simulation
            void on_action_simulation_next();                       ///<Event handler to increase the rendered timestamp by 1
            void on_action_simulation_set_timestamp();              ///<Event handler to set the rendered timestamp to the value specified in data_renderer::get_current_timestamp()
            void on_action_layer();                                 ///<Event handler to open the layer window
            void on_action_renderer();                              ///<Event handler to open the renderer window
            void on_action_zoompan_reset();                         ///<Event handler to reset zoom and pan
            void on_action_cda();                                   ///<Event handler to execute the coastal damage analysis tool
            void on_action_crosssection();                          ///<Event handler to execute the crosssection tool
            void on_action_screenshot();                            ///<Event handler to export a screenshot
            void on_action_about();                                 ///<Event handler to open the about window
            //TODO: Implement documentation here
            /**
             * @brief Overloaded event handler to open the MainWindow::dialog_probe_edit dialog
             * @param path
             * @param column
            */
            void on_action_probelist_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
            //TODO: Implement documentation here
            /**
             * @brief Event handler to call the context menu in the MainWindow::probelist
             * @param event
            */
            void on_action_probelist_button_press(GdkEventButton* event);
            void on_action_probelist_changed();                     ///<Event handler to update the selected probe in the map and the list
            void on_action_probelist_context_edit();                ///<Event handler to update the probes when edited
            void on_probe_remove();                                 ///<Event handler to remove the currently selected probe in data_renderer->active_probe
            void on_action_button_probe_add();                      ///<Event handler to open the MainWindow::dialog_probe_edit to add a new probe
            //TODO: Implement documentation here
            /**
             * @brief 
            */
            void on_probe_update(bool added);                                                                   
            void on_probe_select();                                 ///<Event handler to handle selection of new item in probe list and raw-data-window

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;                ///<Glib::RefPtr Builder of this MainWindow
            /**
             * @brief Searches probelist for entry specified by \p name
             * @param name Name of the probe to search for
             * @return Gtk::TreeStore::Iterator to probe
            */
            Gtk::TreeStore::iterator search_probelist(std::string name);
            /**
             * @brief Prepares the MainWindow::dialog_probe_edit
             * @param clear Specifies if the dialog should be initialized with default values
            */
            void prepare_add_edit(bool clear);
            void handle_add_edit();                                 ///<Calls the MainWindow::dialog_probe_edit, shows it and updates/inserts the probe after editing
            /**
             * @brief Updates MainWindow::probedata with a selected probe
             * @param name of the probe, of which the data is presented
            */
            void update_probe_ui(string name);
            /**
             * @brief Opens a new window, similar to MainWindow::dialog_probe_edit with data of the active probe, specified in  data_renderer::active_probe_name
            */
            void open_probe_ui();
            void handle_timestamp_change();                         ///<Handles switching to another timestamp by updating the UI and the shader
            void reset_probes();                                    ///<Removes all probes from the list and the map, as well clearing the data_renderer::active_probe
    };
}

#endif