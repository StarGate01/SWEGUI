/**
 * @file DataRendererThreads.cpp
 * @brief Implements async data loading related functionality defined in DataRenderer.hpp
 * 
 * General functionality is defined in DataRenderer.cpp
 * Event handler related functionality is defined in DataRendererActions.cpp
 * Renderer related functionality is defined in DataRendererCore.cpp
*/

#include <math.h>
#include "DataRenderer.hpp"
#include "Errors.hpp"

using namespace renderer;

void DataRenderer::on_thread_select_timestamp_notify()
{
    if(t_select_timestamp)
    {
        if (t_select_timestamp->joinable()) t_select_timestamp->join();
        delete t_select_timestamp;
        t_select_timestamp = nullptr;
    }
    m_stream.unlock();
    m_signal_done_select_timestamp.emit(r_select_timestamp_async);
}

void DataRenderer::select_timestamp_async(int timestamp)
{
    if(!m_stream.try_lock()) 
    {
        m_signal_done_select_timestamp.emit(-1);
        return;
    }
    t_select_timestamp = new std::thread([this, timestamp] 
    {
        r_select_timestamp_async = select_timestamp(timestamp);
        dispatcher_select_timestamp.emit();
    });
}

DataRenderer::type_signal_done_select_timestamp DataRenderer::signal_done_select_timestep()
{
    return m_signal_done_select_timestamp;
}

int DataRenderer::select_timestamp(int timestamp)
{
    int res = select_load(NetCdfImageStream::Variable::B, 0, b);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::H, timestamp, h);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::Hu, timestamp, hu);
    if(res != ERROR_SUCCESS) return res;
    res = select_load(NetCdfImageStream::Variable::Hv, timestamp, hv);
    if(res != ERROR_SUCCESS) return res;
    hx.meta_info.min = 0.f;
    hx.meta_info.max = std::sqrt((hu.meta_info.max * hu.meta_info.max) + (hv.meta_info.max * hv.meta_info.max));
    current_timestamp = timestamp;
    return ERROR_SUCCESS;
}

int DataRenderer::select_load(NetCdfImageStream::Variable variable, int index, Layer& lay)
{
    bool ret = netcdf_stream.select(variable, index);
    if(!ret) return ERROR_SELECT;
    lay.meta_info = netcdf_stream.meta_info;
    sf::Image img;
    ret = img.loadFromStream(netcdf_stream);
    if(!ret) return ERROR_STREAM;
    ret = lay.texture.loadFromImage(img);
    if(!ret) return ERROR_IMAGE;
    return ERROR_SUCCESS;
}


void DataRenderer::on_thread_open_notify()
{
    if(t_open)
    {
        if (t_open->joinable()) t_open->join();
        delete t_open;
        t_open = nullptr;
    }
    if(r_open_async == ERROR_SUCCESS)
    {
        zoom = 1.f;
        pan = sf::Vector2f(0.f, 0.f);
        update_transform();
        update_shader();
    }
    m_stream.unlock();
    m_signal_done_select_timestamp.emit(r_open_async);
}

DataRenderer::type_signal_done_open DataRenderer::signal_done_open()
{
    return m_signal_done_open;
}

void DataRenderer::open_async(std::string filename)
{
    if(!m_stream.try_lock())
    {
        m_signal_done_select_timestamp.emit(-1);
        return;
    }
    bool ret = netcdf_stream.open(filename);
    if(!ret)
    {
        r_open_async = ERROR_FILE;
        m_signal_done_select_timestamp.emit(r_open_async);
        return;
    }
    t_open = new std::thread([this] 
    {
        r_open_async = select_timestamp(0);
        dispatcher_open.emit();
    });
}


void DataRenderer::on_thread_sample_batch_notify()
{
    if(t_sample_batch)
    {
        if (t_sample_batch->joinable()) t_sample_batch->join();
        delete t_sample_batch;
        t_sample_batch = nullptr;
    }
    m_signal_done_sample_batch.emit(r_sample_batch);
}

void DataRenderer::sample_batch_async(float x, float y, float**& data)
{
    t_sample_batch = new std::thread([this, x, y, data] 
    {
        {
            //std::lock_guard<std::mutex> lock(m_stream);
            for(int i=0; i<meta_info->timestamps; i++)
            {
                data[i][0] = sample(NetCdfImageStream::Variable::H, x, y, i);
                data[i][1] = sample(NetCdfImageStream::Variable::Hu, x, y, i);
                data[i][2] = sample(NetCdfImageStream::Variable::Hv, x, y, i);
            }
            r_sample_batch = 0;
        }
        dispatcher_sample_batch.emit();
    });
}

DataRenderer::type_signal_done_sample_batch DataRenderer::signal_done_sample_batch()
{
    return m_signal_done_sample_batch;
}

float DataRenderer::sample(NetCdfImageStream::Variable var, float x, float y, int timestamp)
{
    if(timestamp == -1) timestamp = current_timestamp;
    if(timestamp == -1) return 0.f;
    return netcdf_stream.sample(var, x, y, timestamp);
}


float DataRenderer::get_current_time()
{
    return netcdf_stream.get_time(current_timestamp);
}
