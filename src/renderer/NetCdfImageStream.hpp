#ifndef NETCDFIMAGESTREAM_H_INCLUDED
#define NETCDFIMAGESTREAM_H_INCLUDED

#include <SFML/System.hpp>
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

            float min, max;

            NetCdfImageStream() {}
            ~NetCdfImageStream();
            bool open(std::string filename);
            bool select(Variable var, uint32_t index);

            sf::Int64 read(void* data, sf::Int64 size);
            sf::Int64 seek(sf::Int64 position);
            sf::Int64 tell();
            sf::Int64 getSize();

        private:
        
            io::NetCdfReader* reader = nullptr;
            float* current_data = nullptr;
            char header[];
            void generate_header();
            void find_minmax(float* minmax);

    };

}

#endif