#ifndef GTKMM_SWE_APPLICATION_H
#define GTKMM_SWE_APPLICATION_H

#include <iostream>
#include <gtkmm.h>
#include "GuiApplicationWindow.hpp"
#include "GuiEnums.hpp"

namespace swegui
{

    class GuiApplication : public Gtk::Application
    {

        public:

            static Glib::RefPtr<GuiApplication> create(); 

          
        protected:

            GuiApplication();
            void on_activate() override;
            void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;

        private:

            GuiApplicationWindow* create_appwindow();
            void on_hide_window(Gtk::Window* window);
            void on_window_action(Signal signal);

    };

}

#endif