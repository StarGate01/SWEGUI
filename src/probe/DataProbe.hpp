/**
 * @brief Implementation of data probes
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

            float x = 0.f;              ///<X position of the probe
            float y = 0.f;              ///<Y position of the probe

            /**
             * @brief Returns the sprite which represents the visual for the probe
             * @return The probes corresponding sprite
            */
            sf::Sprite& getSprite();
            /**
             * @brief fills row with x and y value
             * 
             * @param row Row to fill
            */
            void fill_row(const Gtk::TreeRow& row);

            ProbeDetailsWindow* window = nullptr;           ///<Pointer to parent ProbeDetailsWindow
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
            /**
             * @brief Indicates if data is currently loaded
             * 
             * @return True, if data is currently loaded. False otherwise
            */
            bool loads_data();
            /**
             * @brief Gives all the probes data as a vector for the specified layer
             * @param layer Layer for which data should be retrieved
             * @return Vector with the probes data, or empty if no data or incorrect layer
            */
            std::vector<float> get_all_data(int layer);

            typedef sigc::signal<void> type_signal_done_fill_data;  ///<Signal that is fired, when async data loading is complete
            type_signal_done_fill_data signal_done_fill_data();     ///<Event handler when data loading is complete

        protected:
    
            type_signal_done_fill_data m_signal_done_fill_data;     ///<Signal fires, when data loading is complete

        private:
            static ProbeColumns cols;                   ///<Template for the columns of the probe list
            sf::Sprite sprite;                          ///<Sprite, that is drawn on the map at the probes position
            int timestamps = 0;                         ///<Number of timestamps of the simulation
            float** data = nullptr;                     ///<Contains a value for each layer for each datapoint
            bool data_loaded = false;                   ///<True if data is loaded, otherwise false
            bool data_loading = false;                  ///<True if data loading in progress, otherwise false

            void on_done_batch_sample(int result);      ///< @brief Executed when batch sampling is completed
    };
}

#endif