/**
 * @file Application.hpp
 * @brief Main class of the SWEGUI application
*/


#ifndef GTKMM_SWE_APPLICATION_H
#define GTKMM_SWE_APPLICATION_H

#include <iostream>
#include <gtkmm.h>
#include "MainWindow.hpp"

namespace swegui
{
    /**
     * @brief Main class of the SWEGUI application
     * swegui::Application serves als framework class, that initializes and launches the main gui of the application
    */
    class Application : public Gtk::Application
    {

        public:
            /**
             * @brief Creates a new instance of swegui::Application
             * @return Glib:RefPtr to new Instance of swegui_Application
            */
            static Glib::RefPtr<Application> create(); 

        protected:
            Application();                  ///<Default constructor
            void on_activate() override;    ///<Event handler on activation. Creates and shows the main GUI

        private:
            /**
             * @brief Creates a new window by calling Application::create, presents it and attaches the Application::on_hide_window() event handler on closing
             * @return Pointer to new instance of MainWindow
            */
            MainWindow* create_appwindow();
            void on_hide_window(Gtk::Window* window);
    };

}

#endif