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
     * @brief Window that contains data values and graphs for probes
    */
    class ProbeDetailsWindow : public Gtk::Window
    {

        public:
            /**
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

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;

        private:

            swegui::MainWindow* parent;
            std::string name;

            Gtk::Window* window_probe = nullptr;
            widgets::DataFieldWidget* probedata = nullptr;
            /**
            * @brief Crates the data field for the probes values in the gui
            */
            void setup_gui_elements();
    };

}

#endif