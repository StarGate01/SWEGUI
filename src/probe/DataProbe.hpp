/**
 * Implementation of data probes
 * @file DataProbe.hpp
*/

#ifndef DATAPROBE_H
#define DATAPROBE_H

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
     * @brief Manages the probe points and their data and 
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
            DataProbe(float px, float py);
            ~DataProbe();               ///<Deletes the associates ProbeDetailsWindow, if it exists

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
            void fill_data_async(renderer::DataRenderer* data_renderer);
            /**
             * @brief Returns if a probe has data associated with it
             * @return True if probe has data associated with it
            */
            bool has_data();
            bool loads_data();
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

            typedef sigc::signal<void> type_signal_done_fill_data;
            type_signal_done_fill_data signal_done_fill_data();

        protected:
    
            type_signal_done_fill_data m_signal_done_fill_data;

        private:
            static ProbeColumns cols;           ///<Template for the columns of the probe list
            sf::Sprite sprite;                  ///<Sprite, that is drawn on the map at the probes position
            int timestamps = 0;                 ///<Number of timestamps of the simulation
            float** data = nullptr;             ///<Contains a value for each layer for each datapoint
            bool data_loaded = false;
            bool data_loading = false;

            void on_done_batch_sample(int result);
    };
}

#endif