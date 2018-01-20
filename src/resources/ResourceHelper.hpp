#ifndef RESOURCEHELPER_H
#define RESOURCEHELPER_H

#include <gtkmm.h>
#include <string>

namespace resources
{

    class ResourceHelper
    {

        public:

            static std::string global_to_string(const std::string& path);
            static void* global_to_memory(const std::string& path);
            static unsigned long global_get_size(const std::string& path);

        private:

            static Glib::RefPtr<Gio::InputStream> global_get_stream(const std::string& path);

    };

}

#endif