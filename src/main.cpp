#include <stdlib.h>
#include <gtkmm.h>
#include "Application.hpp"

int main(int argc, char *argv[])
{
    //putenv("DISPLAY=127.0.0.1:0");
    auto application = swegui::Application::create();
    return application->run(argc, argv);
}