#include <gtkmm.h>
#include <iostream>
#include <stdio.h>

#include "GuiApplication.hpp"

int main(int argc, char *argv[])
{
    auto application = GuiApplication::create();
    return application->run(argc, argv);
}