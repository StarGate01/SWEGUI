#include "NetCdfImageStream.hpp"

using namespace renderer;

NetCdfImageStream::~NetCdfImageStream()
{
    if(reader != nullptr) delete reader;
}

bool NetCdfImageStream::open(std::string filename)
{
    if(reader != nullptr) delete reader;
    reader = new io::NetCdfReader(filename, true, false);
    return reader->success;
}

bool NetCdfImageStream::select(Variable var, uint32_t index)
{
    switch(var)
    {
        case H: current_data = reader->hData; break;
        case Hu: current_data = reader->huData; break;
        case Hv: current_data = reader->hvData; break;
        case B: current_data = reader->bData; break;
    }
    if(var != B)
    {

    }
    float minmax[2];
    find_minmax(minmax);
    min = minmax[0];
    max = minmax[1];
    return true;
}

void NetCdfImageStream::generate_header()
{

}

void NetCdfImageStream::find_minmax(float* minmax)
{

}


sf::Int64 NetCdfImageStream::read(void* data, sf::Int64 size)
{
    if(!reader->success) return -1;
    return 0;
}

sf::Int64 NetCdfImageStream::seek(sf::Int64 position)
{
    if(!reader->success) return -1;
    return 0;
}

sf::Int64 NetCdfImageStream::tell()
{
    if(!reader->success) return -1;
    return 0;
}

sf::Int64 NetCdfImageStream::getSize()
{
    if(!reader->success) return -1;
    return 0;
}