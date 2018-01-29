/**
 * @brief Window to display the details of a probe
 * @file ProbeDetailsWindow.hpp
*/

#ifndef PROBE_DETAILS_WINDOW_H
#define PROBE_DETAILS_WINDOW_H

#include <gtkmm.h>
#include <string>
#include "../widgets/DataFieldWidget.hpp"

#define PATH_TO_PROBE_DETAIL_GUI "/main/src/ui/probe_detail.glade"

namespace swegui
{

    class MainWindow;

}

namespace probe
{
    /**
<<<<<<< HEAD
     * @brief Window that contains data values and graphs for probes
=======
     * @brief Window to display the details of a probe, contains widgets::DataFieldWidget
>>>>>>> 43b3419e42dd269d11e02df68a580165aa0bd5cd
    */
    class ProbeDetailsWindow : public Gtk::Window
    {

        public:
            /**
<<<<<<< HEAD
            * @brief Creates a new details window
            * @param cobject Window wihtin which this window is located
            * @param refBuilder Builder for gui objects
            */
            ProbeDetailsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            static ProbeDetailsWindow* create(swegui::MainWindow* pa, std::string na);
            /**
            * @brief Data for the probe is updated
            */
            void update_ui();
=======
             * @brief Inherited constructor from Gtk::Window
             * @param cobject Calling object
             * @param refBuilder Gtk::Builder, from which the window is created
            */
            ProbeDetailsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

            /**
             * @brief Creates a new ProbeDetailsWindow
             * @param pa Parent swegui::MainWindow
             * @param na Intended name of the window
             * @return Instance of new ProbeDetailsWindow
            */
            static ProbeDetailsWindow* create(swegui::MainWindow* pa, std::string na);
            
            void update_ui();               ///<Updates the UI elements of the window
>>>>>>> 43b3419e42dd269d11e02df68a580165aa0bd5cd

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;                ///<Builder of the window

        private:

            swegui::MainWindow* parent;                             ///<Pointer to parent swegui::MainWindow
            std::string name;                                       ///<Name of the window, respectivly the associated probe

<<<<<<< HEAD
            Gtk::Window* window_probe = nullptr;
            widgets::DataFieldWidget* probedata = nullptr;
            /**
            * @brief Crates the data field for the probes values in the gui
            */
            void setup_gui_elements();
=======
            Gtk::Window* window_probe = nullptr;                    ///<GTK:Window object that is represented by this class
            widgets::DataFieldWidget* probedata = nullptr;          ///<widgets::DataFieldWidget of the ProbeDetailsWindow::window_probe

            void setup_gui_elements();                              ///<Initilializes the GUI elements
>>>>>>> 43b3419e42dd269d11e02df68a580165aa0bd5cd
    };

}

#endif