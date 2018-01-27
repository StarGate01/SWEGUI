/**
 * @file main.cpp
 * @brief Creates a new instance of the application and runs it
*/

#include <stdlib.h>
#include <gtkmm.h>
#include "Application.hpp"

/**
 * @brief Main method of this application
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, 1 otherwise
*/
int main(int argc, char *argv[])
{
    //putenv("DISPLAY=127.0.0.1:0");
    auto application = swegui::Application::create();
    return application->run(argc, argv);
}