/**
 * @file RendererWindow.hpp
 * @brief Window to toggle and configure the renderer
*/

#ifndef LAYER_RENDERER_H
#define LAYER_RENDERER_H

#include <gtkmm.h>
#include <string>
#include <map>

#define PATH_TO_RENDERER_GUI "/main/src/ui/renderer.glade"

namespace swegui
{

    class MainWindow;

    /**
     * @brief Window to toggle the rendering of the individual layers of the GUI and set some rendering parameters
    */
    class RendererWindow : public Gtk::Window
    {

        public:
          
            /**
             * @brief Inherited constructor from Gtk::Window
             * @param cobject Calling object
             * @param refBuilder Gtk::Builder object of the parent window
            */
            RendererWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            ~RendererWindow() { }                                                           ///<Deletes all layers in RendererWindow::layer_uis
            /**
             * @brief Creates a new instance of RendererWindow and initializes them
             * @param pa Pointer to parent MainWindow
            */
            static RendererWindow* create(MainWindow* pa);

            void update_ui();                                                               ///<Updates the UI

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;                                        ///<Pointer to Gtk::Builder of this class

        private:

            MainWindow* parent;                                                             ///<Pointer to parent MainWindow
            
            std::string switch_names[8] = 
                { "probes", "names", "indicators", "coordinates", "zero", "gizmo", "scales", "info" };      ///<String array to simplify access to toggle controls                                
            Gtk::Switch* switches[8];                                                       ///<Gtk::Switch elements of the GUI
        
            void setup_gui_elements();                                                      ///<Extracts GUI elements from glade file and initializes them  
            void on_action_switch();                                                        ///<Event handler if a switch was triggered
            
    };

}

#endif