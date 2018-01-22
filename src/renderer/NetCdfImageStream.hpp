#ifndef NETCDFIMAGESTREAM_H
#define NETCDFIMAGESTREAM_H

#include <SFML/System.hpp>
#include "BMP.hpp"
#include "reader/NetCdfReader.hh"

namespace renderer
{

    class NetCdfImageStream : public sf::InputStream
    {

        public:

            enum Variable: int
            {
                B = 0,
                H = 1,
                Hu = 2,
                Hv = 3
            };

            struct Meta
            {
                int nx = 1, ny = 1;
                float min = -1.f, max = 1.f;
                float dx = 2.f, dy = 1.f, originx = 0.f, originy = 0.f;
                int timestamps = 0;
                float ax() { return dx * nx; }
                float ay() { return dy * ny; }
            };

            Meta meta_info;

            NetCdfImageStream() {}
            ~NetCdfImageStream();
            bool open(std::string filename);
            bool select(Variable var, uint32_t index);
            float sample(Variable var, float x, float y, int timestamp);
            int get_num_timestamps();

            sf::Int64 read(void* data, sf::Int64 size);
            sf::Int64 seek(sf::Int64 position);
            sf::Int64 tell();
            sf::Int64 getSize();

        private:
        
            sf::Int64 stream_pos;
            io::NetCdfReader* reader = nullptr;
            float* current_data = nullptr;
            bool add_bathymetry = false;
            char header[BMP_HEADER_SIZE] = { 0 };
            sf::Int64 stream_size;
            bool generate_meta();
            bool find_minmax();
            void copy_le(int32_t value, char* target);

    };

}

#endif