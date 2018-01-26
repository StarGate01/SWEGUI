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
    /**
     * @brief Manages the probe points and their data
    */
    class DataProbe
    {

        public:
            /**
             * @brief Creates a empty data probe
            */
            DataProbe();
            /**
             * @brief Creates a probe at the given coordinates
             * @param px X Position of the probe 
             * @param py Y Position of the probe
             * @param data_renderer The renderer in which the probe is located
            */
            DataProbe(float px, float py, renderer::DataRenderer* data_renderer);
            ~DataProbe();

            float x = 0.f;
            float y = 0.f;

            /**
             * @brief Returns the sprite which represents the visual for the probe
             * @return The probes corresponding sprite
            */
            sf::Sprite& getSprite();
            void fill_row(const Gtk::TreeRow& row);

            ProbeDetailsWindow* window = nullptr;
            /**
             * @brief Retrieves the values of the different properties for the probe 
             * and inserts them in to the probe data 
             * @param data_renderer The renderer in which the probe is located
            */
            void fill_data(renderer::DataRenderer* data_renderer);
            /**
             * @brief Returns if a probe has data associated with it
             * @return True if probe has data associated with it
            */
            bool has_data();
            /**
             * @brief Gives all the probes data as a vector for the specified layer
             * @param layer Layer for which data should be retrieved
             * @return Vector with the probes data, or empty if no data or incorrect layer
            */
            std::vector<float> get_all_data(int layer);
            /**
             * @brief Gives a single data value for a given layer and timestamp
             * @param timestamp Time for which data should be retreved
             * @param layer Layer for which data should be retrieved
             * @return float with the probes data value for given time and layer, or zero if no data or incorrect layer/timestamp
            */
            float get_data(int timestamp, int layer);
        private:

            static ProbeColumns cols;
            sf::Sprite sprite;
            int timestamps = 0;
            float** data = nullptr;
    };
}

#endif