/**
 * @brief Stream parser for SWE netCDF files
 * @file NetCdfImageStream.hpp
*/


#ifndef NETCDFIMAGESTREAM_H
#define NETCDFIMAGESTREAM_H

#include <SFML/System.hpp>
#include "BMP.hpp"
#include "reader/NetCdfReader.hh"

namespace renderer
{
    /**
     * @brief Stream parser for SWE netCDF files
    */
    class NetCdfImageStream : public sf::InputStream
    {

        public:

            /**
             * @brief Associates each layer with an int to simplify access
            */
            enum Variable: int
            {
                B = 0, ///<Bathymetry
                H = 1, ///<Water height
                Hu = 2, ///<Horizontal flux
                Hv = 3 ///<Vertical flux
            };

            /**
             * @brief Meta information of the SWE netCDF files
            */
            struct Meta
            {
                int nx = 1, ny = 1; ///<Coordinates in file
                float min = -1.f, max = 1.f; ///<Minimum and maximum values of variable
                float xmin = -1.f, xmax = 1.f, ymin = -0.5f, ymax = 0.5f; ///<Minimum and maximum coordinates
                int timestamps = 0; ///<Simulation time
                float ax() { return xmax - xmin; } ///<Difference between maximum and minimum x coordinates
                float ay() { return ymax - ymin; } ///<Difference between maximum and minimum y coordinates
            };

            Meta meta_info;                                                 ///<Meta info of the processed SWE netCDF file

            NetCdfImageStream() {}                                          ///<Default constructor
            ~NetCdfImageStream();                                           ///<Deletes the NetCdfImageStream::reader if necessary
            bool open(std::string filename);                                ///<Opens a SWE netCDF file
            /**
             * @brief Loads selected dataset 
             * @param var Which variable type to load: B for bathymetry, H for water height, Hu for horizontal flux, Hv for vertical flux
             * @param index Index in the cdf file from which data should be read
             * @return true if sucessfully loaded and find_minmax() also returns true
            */
            bool select(Variable var, uint32_t index);                     
            /**
             * @brief Retrieve a single value for a variable 
             * @param var Which variable type to load: B for bathymetry, H for water height, Hu for horizontal flux, Hv for vertical flux
             * @param x X-position of the variable to retrieve
             * @param y Y-position of the variable to retrieve
             * @param timestamp Timestep for which the variable is requested
             * @return the value of the variable for the given input of the cdf file, otherwise 0.f
            */
            float sample(Variable var, float x, float y, int timestamp);
             /**
             * @brief Retrieve the amount of timestamps in the given cdf file
             * @return  Number of timestamps, otherwise 0
            */
            int get_num_timestamps();
             /**
             * @brief Retrieve the actual simulation time for a given timestamp
             * @return The simulation time for the given timestamp, otherwise 0.f
            */
            float get_time(int timestamp);

            /**
             * @brief TODO
             * @param data
             * @param size
             * @return
            */
            sf::Int64 read(void* data, sf::Int64 size);
            /**
             * @brief Set the data streaming position to the given value
             * @param position where to set the stream_pos to
             * @return the new position, -1 if failed
            */
            sf::Int64 seek(sf::Int64 position);
            /**
             * @brief Gives the current data streaming position
             * @return the current position, -1 if failed
            */
            sf::Int64 tell();
            /**
             * @brief Gives the current data streaming position
             * @return the current position, -1 if failed
            */
            sf::Int64 getSize();

        private:
        
            sf::Int64 stream_pos;       ///<Current position in the cdf file to retrieve data from
            io::NetCdfReader* reader = nullptr; ///<cdf reader which reads the input file 
            float* current_data = nullptr; ///<Pointer to the currently read data from the cdf file
            bool add_bathymetry = false; ///<Select if bathymetry has be be added to water height for output
            char header[BMP_HEADER_SIZE] = { 0 }; ///<Header of input file
            sf::Int64 stream_size; ///<Size of data to stream from the input file
            bool generate_meta(); ///<Fills in the information defined in struct Meta, false in unsuccessful, otherwise true
            bool find_minmax(); ///<Finds minimum and maximum vales 
            void copy_le(int32_t value, char* target);ue, char* target); ///<TODO

    };

}

#endif