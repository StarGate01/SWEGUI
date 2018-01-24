#include "DataProbe.hpp"
#include "../renderer/DataRenderer.hpp"
using namespace probe;

ProbeColumns DataProbe::cols;

DataProbe::DataProbe()
    : DataProbe(0.f, 0.f, nullptr)
{
}

DataProbe::DataProbe(float px, float py, renderer::DataRenderer* data_renderer)
    : x(px), y(py)
{
    sprite.setOrigin(16.f, 16.f);
    fill_data(data_renderer);
}

DataProbe::~DataProbe()
{
    if(window != nullptr) delete window;
}

sf::Sprite& DataProbe::getSprite()
{
    return sprite;
}

void DataProbe::fill_row(const Gtk::TreeRow& row)
{
    row[cols.col_x] = x;
    row[cols.col_y] = y;
}

void DataProbe::fill_data(renderer::DataRenderer* data_renderer)
{
    if(data_renderer == nullptr || data_renderer->meta_info == nullptr) return;

    int nr_timestamps = data_renderer->meta_info->timestamps;
    data = new float*[nr_timestamps];
    for(int ts = 0; ts < nr_timestamps; ts++)
    {
        data[ts] = new float[5];
        data[ts][0] = data_renderer->sample(renderer::NetCdfImageStream::Variable::B, x, y, ts);
        data[ts][1] = data_renderer->sample(renderer::NetCdfImageStream::Variable::H, x, y, ts);
        float hu = data_renderer->sample(renderer::NetCdfImageStream::Variable::Hu, x, y, ts);
        float hv = data_renderer->sample(renderer::NetCdfImageStream::Variable::Hv, x, y, ts);
        data[ts][2] = hu;
        data[ts][3] = hv;
        data[ts][4] = sqrtf(std::pow(hu, 2) + std::pow(hv, 2));
    }
    timestamps = nr_timestamps;
}

bool DataProbe::has_data()
{
    return data != nullptr;
}

std::vector<float> DataProbe::get_all_data(int layer)
{
    if(!has_data()) return vector<float>();
    if(layer < 0 || layer >= 5) return vector<float>(); 
    std::vector<float> vec;
    for(int i = 0; i < timestamps; i++) vec.push_back(data[i][layer]);
    return vec;
}

float DataProbe::get_data(int timestamp, int layer)
{
    if(!has_data()) return 0.f;
    if(timestamp < 0 || timestamp >= timestamps) return 0.f;
    if(layer < 0 || layer >= 5) return 0.f;
    return data[timestamp][layer];
}