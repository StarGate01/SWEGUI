/**
 * @file LayerWindow.hpp
 * @brief Window to toggle and configure the rendered layers
*/

#ifndef LAYER_WINDOW_H
#define LAYER_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <map>

#define PATH_TO_LAYER_GUI "/main/src/ui/layers.glade"

namespace renderer
{
    class Layer;
};

namespace swegui
{

    class MainWindow;

    /**
     * @brief Window to toggle the rendering of the individual layers of the GUI and set some rendering parameters
    */
    class LayerWindow : public Gtk::Window
    {

        public:
            /**
             * @brief Subclass of swegui::LayerWindow to simplify management of some GUI elements
             * Contains pointer to all relevent GUI elements for one layer and methods for setup and event handling
            */
            class LayerUI
            {

                public:
                    Gtk::Switch* switch_enable = nullptr,                           ///<Pointer to layer toggle switch
                    *switch_clip = nullptr;                                         ///<Pointer to clipping toggle switch
                    Gtk::SpinButton* spin_min = nullptr,                            ///<Pointer to minimum selection Gtk::SpinButton
                    *spin_max = nullptr;                                            ///<Pointer to maximum selection Gtk::SpinButton
                    Glib::RefPtr<Gtk::Adjustment> adjustment_min,                   ///<Glib::RefPtr to Gtk::Adjustment object of spin_min
                    adjustment_max;                                                 ///<Glib::RefPtr to Gtk::Adjustment object of spin_max
                    renderer::Layer* data_layer = nullptr;                          ///<Pointer to data layer

                    LayerUI() { }                                                   ///<Default constructor

                    void update_data();                                             ///<Updates the layer data with the values of the GUI elements and triggers the LayerUI::m_signal_update
                    void update_ui();                                               ///<Updates the GUI elements with the values of layer data

                    typedef sigc::signal<void> type_signal_update;                  ///<Declearation of the update sigc::signal
                    type_signal_update signal_update();                             ///<Event handler for the update sigc::signal

                protected:

                    type_signal_update m_signal_update;                             ///<Update sigc::signal

            };

            /**
             * @brief Inherited constructor from Gtk::Window
             * @param cobject Calling object
             * @param refBuilder Reference to Gtk::Builder object of the parent window
            */
            LayerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            ~LayerWindow();                                                         ///<Deletes all layers in LayerWindow::layer_uis
            /**
             * @brief Creates a new instance of LayerWindow and initilializes them
             * @param pa Pointer to parent MainWindow
            */
            static LayerWindow* create(MainWindow* pa);

            void update_ui();                                                       ///<Updates the UI by triggering the LayerUI::update_ui() function of each layer

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;                                ///<Pointer to Gtk::Builder of this class

        private:

            MainWindow* parent;                                                     ///<Pointer to parent MainWindow
            std::string layer_names[5] = { "b", "h", "hu", "hv", "hx" };            ///<Helper array containing the names of the layers
            LayerUI* layer_uis[5];                                                  ///<Arrays of LayerUI objects

            void setup_gui_elements();                                              ///<Extracts GUI elements from glade file and initializes them  


    };

}

#endif