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
     * @brief Window to display the details of a probe, contains widgets::DataFieldWidget
    */
    class ProbeDetailsWindow : public Gtk::Window
    {

        public:

            ProbeDetailsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            static ProbeDetailsWindow* create(swegui::MainWindow* pa, std::string na);
            
            void update_ui();               ///<Updates the UI elements of the window

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;                ///<Builder of the window

        private:

            swegui::MainWindow* parent;                             ///<Pointer to parent swegui::MainWindow
            std::string name;                                       ///<Name of the window, respectivly the associated probe

            Gtk::Window* window_probe = nullptr;                    ///<GTK:Window object that is represented by this class
            widgets::DataFieldWidget* probedata = nullptr;          ///<widgets::DataFieldWidget of the ProbeDetailsWindow::window_probe

            void setup_gui_elements();                              ///<Initilializes the GUI elements
    };

}

#endif