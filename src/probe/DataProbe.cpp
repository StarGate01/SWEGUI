/**
 * @file DataProbe.cpp
 * 
 * @brief Implements the functionality defined in DataProbe.hpp
*/

#include "DataProbe.hpp"
#include "../renderer/DataRenderer.hpp"

using namespace probe;

ProbeColumns DataProbe::cols;

DataProbe::DataProbe()
   : x(0), y(0)
{
    sprite.setOrigin(16.f, 16.f);
}

DataProbe::DataProbe(float px, float py, renderer::DataRenderer* data_renderer)
    : x(px), y(py)
{
    sprite.setOrigin(16.f, 16.f);
    fill_data_async(data_renderer);
}

DataProbe::~DataProbe()
{
    signal_done_sample_batch_handler.disconnect();
    if(data != nullptr) delete[] data;
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

void DataProbe::fill_data_async(renderer::DataRenderer* data_renderer)
{
    if(data_renderer == nullptr || data_renderer->meta_info == nullptr) return;
    if(has_data() || loads_data()) return;
    data_loading = true;
    timestamps = data_renderer->meta_info->timestamps;
    data = new float*[timestamps];
    for(int ts = 0; ts < timestamps; ts++) data[ts] = new float[4];
    signal_done_sample_batch_handler = data_renderer->signal_done_sample_batch().connect(sigc::mem_fun(this, &DataProbe::on_done_batch_sample));
    dry = data_renderer->sample(renderer::NetCdfImageStream::Variable::B, x, y, 0) > 0;
    data_renderer->sample_batch_async(x, y, data);
}

void DataProbe::reset_data(renderer::DataRenderer* data_renderer)
{
    if(loads_data()) return;
    data_loaded = false;
    if(data != nullptr) delete[] data;
    fill_data_async(data_renderer);
}

DataProbe::type_signal_done_fill_data DataProbe::signal_done_fill_data()
{
    return m_signal_done_fill_data;
}

void DataProbe::on_done_batch_sample(int result)
{
    for(int ts = 0; ts < timestamps; ts++) 
    {
        float hu = data[ts][1];
        float hv = data[ts][2];
        data[ts][3] = sqrtf((hu * hu) + (hv * hv));
    }
    data_loaded = true;
    data_loading = false;
    m_signal_done_fill_data.emit();
}

bool DataProbe::has_data()
{
    return data != nullptr && data_loaded;
}

bool DataProbe::is_dry()
{
    return dry;
}

bool DataProbe::loads_data()
{
    return data_loading;
}

std::vector<float> DataProbe::get_all_data(int layer)
{
    if(!has_data()) return vector<float>();
    if(layer < 0 || layer >= 4) return vector<float>(); 
    std::vector<float> vec;
    for(int i = 0; i < timestamps; i++) vec.push_back(data[i][layer]);
    return vec;
}