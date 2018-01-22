/**
 * @file ResourceHelper.cpp
 * @brief Implements the functionality defined in Resourcehelper.hpp
 */

#include <gtkmm.h>
#include <giomm/resource.h>
#include "ResourceHelper.hpp"

using namespace resources;

std::string ResourceHelper::global_to_string(const std::string& path)
{
    char* res = (char*)ResourceHelper::global_to_memory(path);
    if(res != nullptr) return std::string(res);
    return std::string();
}

void* ResourceHelper::global_to_memory(const std::string& path)
{
    Glib::RefPtr<Gio::InputStream> res = ResourceHelper::global_get_stream(path);
    if(res)
    {
        unsigned long size = ResourceHelper::global_get_size(path);
        char* buffer = new char[size + 1];
        res->read((void*)buffer, size);
        buffer[size] = '\0';
        return (void*)buffer;
    }
    return nullptr;
}

unsigned long ResourceHelper::global_get_size(const std::string& path)
{
    if(!Gio::Resource::get_file_exists_global_nothrow(path)) return -1;
    gsize size;
    Gio::ResourceFlags flags;
    Gio::Resource::get_info_global(path, size, flags);
    return size;
}

Glib::RefPtr<Gio::InputStream> ResourceHelper::global_get_stream(const std::string& path)
{
    if(!Gio::Resource::get_file_exists_global_nothrow(path)) return Glib::RefPtr<Gio::InputStream>();
    return Gio::Resource::open_stream_global(path);
}