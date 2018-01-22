#include <cmath>
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
    if(reader->success) return generate_meta();
    return false;
}

bool NetCdfImageStream::select(Variable var, uint32_t index)
{
    if(reader == nullptr || !reader->success) return false;
    add_bathymetry = false;
    switch(var)
    {
        case H: 
            current_data = reader->hData;
            add_bathymetry = true;
            break;
        case Hu: current_data = reader->huData; break;
        case Hv: current_data = reader->hvData; break;
        case B: current_data = reader->bData; break;
        default: return false;
    }
    if(var != B)
    {
        bool res = reader->selectTimestep(index);
        if(!res) return false;
    }
    stream_pos = 0;
    return find_minmax();
}

float NetCdfImageStream::sample(Variable var, float x, float y, int timestamp)
{
    if(reader == nullptr || !reader->success) return 0.f;
    int xindex = round(((x - meta_info.originx) / meta_info.ax()) * meta_info.nx);
	int yindex = round(((y - meta_info.originy) / meta_info.ay()) * meta_info.ny);
    switch(var)
    {
        case B: return reader->getCellValue(reader->bVar, xindex, yindex, timestamp);
        case H: return reader->getCellValue(reader->hVar, xindex, yindex, timestamp);
        case Hu: return reader->getCellValue(reader->huVar, xindex, yindex, timestamp);
        case Hv: return reader->getCellValue(reader->hvVar, xindex, yindex, timestamp);
    }
    return 0.f;
}

bool NetCdfImageStream::generate_meta()
{
    if(reader == nullptr || !reader->success) return false;
    meta_info.nx = reader->getGlobalIntAttribute("nx");
    meta_info.ny = reader->getGlobalIntAttribute("ny");
    meta_info.dx = reader->getGlobalFloatAttribute("dx");
    meta_info.dy = reader->getGlobalFloatAttribute("dy");
    meta_info.originx = reader->getGlobalFloatAttribute("originx");
    meta_info.originy = reader->getGlobalFloatAttribute("originy");
    meta_info.timestamps = (int)reader->timeLength;
    //BMP Header
    header[BMP_OFFSET_BF_TYPE] = BMP_MAGIC_1;
    header[BMP_OFFSET_BF_TYPE + 1] = BMP_MAGIC_2;
    stream_size = BMP_HEADER_SIZE + (meta_info.nx * meta_info.ny * BMP_BYTECOUNT);
    copy_le(stream_size, header + BMP_OFFSET_BF_SIZE);
    header[BMP_OFFSET_BF_OFFBITS] = BMP_HEADER_SIZE;
    //Info header
    header[BMP_OFFSET_BI_SIZE] = BMP_INFOHEADER_SIZE;
    copy_le(meta_info.nx, header + BMP_OFFSET_BI_WIDTH);
    copy_le(-meta_info.ny, header + BMP_OFFSET_BI_HEIGHT); //top down
    header[BMP_OFFSET_BI_PLANES] = BMP_PLANES;
    header[BMP_OFFSET_BI_BITCOUNT] = BMP_BITCOUNT;
    copy_le((stream_size) - BMP_HEADER_SIZE, header + BMP_OFFSET_BI_SIZEIMAGE);
    return true;
}

void NetCdfImageStream::copy_le(int32_t value, char* target)
{
    target[0] = value & 0xFF;
    target[1] = (value >> 8) & 0xFF;
    target[2] = (value >> 16) & 0xFF;
    target[3] = (value >> 24) & 0xFF;
}

bool NetCdfImageStream::find_minmax()
{
    if(reader == nullptr || !reader->success 
        || current_data == nullptr) return false;
    meta_info.min = current_data[0];
    meta_info.max = current_data[0];
    for(int i=1; i<meta_info.nx * meta_info.ny; i++)
    {
        float fdata = current_data[i];
        if(add_bathymetry) fdata += reader->bData[i];
        if(fdata < meta_info.min) meta_info.min = fdata;
        if(fdata > meta_info.max) meta_info.max = fdata;
    }
    return true;
}

//format (color scale 0.0 - 1.0): blue * 100 + green * 100000 + red * 1000000, mapped between min and max
sf::Int64 NetCdfImageStream::read(void* data, sf::Int64 size)
{
    if(size < 0 || reader == nullptr || !reader->success || current_data == nullptr) return -1;
    uint64_t ri = 0;
    while(stream_pos < BMP_HEADER_SIZE && ri < (uint64_t)size)
    {
        ((uint8_t*)data)[ri] = header[stream_pos];
        stream_pos++;
        ri++;
    }
    float diff = meta_info.max - meta_info.min;
    while(stream_pos < stream_size && ri < (uint64_t)size)
    {
        uint64_t data_pos = stream_pos - BMP_HEADER_SIZE;
        float fdata = current_data[data_pos / 3];
        if(add_bathymetry) fdata += reader->bData[data_pos / 3];
        fdata = (fdata - meta_info.min) / diff;
        float intp = 0.f;

        for(int i=0; i<=(int)(data_pos % 3); i++)
        {
            fdata *= 100.f;
            fdata = modf(fdata, &intp);
        }
        ((uint8_t*)data)[ri] = intp * 2.55f;
        //((uint8_t*)data)[ri] = fdata * 255.0f;
        stream_pos++;
        ri++;
    }
    return ri;
}

sf::Int64 NetCdfImageStream::seek(sf::Int64 position)
{
    if(reader == nullptr || !reader->success || current_data == nullptr) return -1;
    stream_pos = position;
    return stream_pos;
}

sf::Int64 NetCdfImageStream::tell()
{
    if(reader == nullptr || !reader->success || current_data == nullptr) return -1;
    return stream_pos;
}

sf::Int64 NetCdfImageStream::getSize()
{
    if(reader == nullptr || !reader->success || current_data == nullptr) return -1;
    return stream_size;
}