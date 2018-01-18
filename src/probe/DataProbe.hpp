#ifndef DATAPROBE_H
#define DATAPROBE_H

#include <gtkmm.h>
#include <string>
#include <SFML/Graphics.hpp>
#include "ProbeColumns.hpp"

namespace probe
{

    class DataProbe
    {

        public:

            DataProbe();
            DataProbe(float px, float py);

            float x = 0.f;
            float y = 0.f;

            sf::Sprite& getSprite();
            void fill_row(const Gtk::TreeRow& row);


        private:

            static ProbeColumns cols;
            sf::Sprite sprite;

    };

}

#endif