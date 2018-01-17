#ifndef NETCDFIMAGESTREAM_H_INCLUDED
#define NETCDFIMAGESTREAM_H_INCLUDED

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
                H = 0,
                Hu = 1,
                Hv = 2,
                B = 3
            };

            struct Meta
            {
                int nx = 1, ny = 1;
                float min = 0.0, max = 1.0;
                float dx = 2.f, dy = 1.f, originx = 0.f, originy = 0.f;
                float ax() { return dx * nx; }
                float ay() { return dy * ny; }
            };

            Meta meta_info;

            NetCdfImageStream() {}
            ~NetCdfImageStream();
            bool open(std::string filename);
            bool select(Variable var, uint32_t index);

            sf::Int64 read(void* data, sf::Int64 size);
            sf::Int64 seek(sf::Int64 position);
            sf::Int64 tell();
            sf::Int64 getSize();

        private:
        
            sf::Int64 stream_pos;
            io::NetCdfReader* reader = nullptr;
            float* current_data = nullptr;
            char header[BMP_HEADER_SIZE] = { 0 };
            sf::Int64 stream_size;
            bool generate_meta();
            bool find_minmax();
            void copy_le(int32_t value, char* target);

    };

}

#endif