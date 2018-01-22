#ifndef GTKMM_SWE_APPLICATION_H
#define GTKMM_SWE_APPLICATION_H

#include <iostream>
#include <gtkmm.h>
#include "MainWindow.hpp"

namespace swegui
{

    class Application : public Gtk::Application
    {

        public:
            static Glib::RefPtr<Application> create(); 

          
        protected:
            Application();
            void on_activate() override;
            void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;

        private:
            MainWindow* create_appwindow();
            void on_hide_window(Gtk::Window* window);
    };

}

#endif