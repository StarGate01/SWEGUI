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

    class ProbeDetailsWindow : public Gtk::Window
    {

        public:

            ProbeDetailsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            static ProbeDetailsWindow* create(swegui::MainWindow* pa, std::string na);

            void update_ui();

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;

        private:

            swegui::MainWindow* parent;
            std::string name;

            Gtk::Window* window_probe = nullptr;
            widgets::DataFieldWidget* probedata = nullptr;

            void setup_gui_elements();
    };

}

#endif