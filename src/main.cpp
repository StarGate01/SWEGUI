#include <gtkmm.h>
#include <iostream>
#include <stdio.h>

#include "gtkmm_swe_application.h"

int main(int argc, char *argv[])
{
    auto application = GuiApplication::create();
    return application->run(argc, argv);

    /*
    auto app = Gtk::Application::create(argc, argv, "de.tum.in.www5.tssim.g03.swegui");
    // ---- Widgets ----
    //Main application & window
    Gtk::ApplicationWindow* pWindow = nullptr;
    //Menubar
    Gtk::MenuBar* menubar_main = nullptr;
    Gtk::MenuItem* menuitementry_open = nullptr;
    //Toolbar
    Gtk::Toolbar* toolbar_main = nullptr;
    Gtk::ToolButton* tb_openfile = nullptr;
    Gtk::ToolButton* tb_quit = nullptr;
    Gtk::ToolButton* tb_test1 = nullptr;
    Gtk::ToolButton* tb_test2 = nullptr;
    //Raw data field
    Gtk::Label* lbl_raw_data = nullptr;

    //---- Load GtkBuilder ----
    auto refBuilder = Gtk::Builder::create();
    try
    {
        refBuilder->add_from_file("ui/main.gtk");
    }
    catch (const Glib::FileError &ex)
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Glib::MarkupError &ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Gtk::BuilderError &ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    //---- Instantiate widgets ----
    //Get the GtkBuilder-instantiated Dialog:
    refBuilder->get_widget("window_main", pWindow);
    if (pWindow)
    {
        //Get menu bar
        refBuilder->get_widget("main_menubar", menubar_main);
        if(menubar_main)
        {
            refBuilder->get_widget("imagemenuitem1", menuitementry_open);
 
            if(!(menuitementry_open))
            {
                perror("Entries of menubar not found");
                return EXIT_FAILURE;
            }

            //Event handlers for menubar
            menuitementry_open->signal_activate().connect(sigc::ptr_fun(on_button_clicked_newfile));
        }
        else
        {
            perror("menubar_main not found");
            return EXIT_FAILURE;
        }

        //Get toolbar
        refBuilder->get_widget("main_toolbar", toolbar_main);
        if(toolbar_main)
        {
            //Get toolbar items
            refBuilder->get_widget("tbtn_new", tb_openfile);
            refBuilder->get_widget("bbtn_quit", tb_quit);
            refBuilder->get_widget("tbtn_test1", tb_test1);
            refBuilder->get_widget("tbtn_test2", tb_test2);

            if(!(tb_openfile && tb_quit && tb_test1 && tb_test2))
            {
                perror("Buttons of main_toolbar not found");
                return EXIT_FAILURE;
            }

            //Event handlers for toolbar
            tb_openfile->signal_clicked().connect(sigc::ptr_fun(on_button_clicked_newfile));
            tb_quit->signal_clicked().connect(sigc::ptr_fun(on_button_clicked_quit));
            tb_test1->signal_clicked().connect(sigc::ptr_fun(on_button_clicked_test1));
            tb_test2->signal_clicked().connect(sigc::ptr_fun(on_button_clicked_test2));
        }
        else
        {
            perror("main_toolbar not found");
            return EXIT_FAILURE;
        }

        //Get raw_data field
        refBuilder->get_widget("lbl_info", lbl_raw_data);
        if(lbl_raw_data)
        {
            //TESTING: Change label of textfield
            lbl_raw_data->set_width_chars(30);
            lbl_raw_data->set_text("Hello world");
        }
        else
        {
            perror("lbl_raw_data not found");
            return EXIT_FAILURE;
        }

        app->run(*pWindow);
    }

    delete pWindow;

    return 0;

    */
}