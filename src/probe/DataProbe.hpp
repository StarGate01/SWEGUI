#ifndef DATAPROBE_H
#define DATAPROBE_H

#include <iostream>
#include <gtkmm.h>
#include <string>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "ProbeColumns.hpp"
#include "ProbeDetailsWindow.hpp"

namespace renderer
{
    class DataRenderer;
}

namespace probe
{

    class DataProbe
    {

        public:

            DataProbe();
            DataProbe(float px, float py, renderer::DataRenderer* data_renderer);
            ~DataProbe();

            float x = 0.f;
            float y = 0.f;

            sf::Sprite& getSprite();
            void fill_row(const Gtk::TreeRow& row);

            ProbeDetailsWindow* window = nullptr;
            void fill_data(renderer::DataRenderer* data_renderer);
            bool has_data();
            std::vector<float> get_all_data(int layer);
            float get_data(int timestamp, int layer);
        private:

            static ProbeColumns cols;
            sf::Sprite sprite;
            int timestamps = 0;
            float** data = nullptr;
    };
}

#endif