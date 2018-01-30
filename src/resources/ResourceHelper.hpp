/**
 * @file ResourceHelper.hpp
 * 
 * @brief Provides easy access to GTK resources
 */

#ifndef RESOURCEHELPER_H
#define RESOURCEHELPER_H

#include <gtkmm.h>
#include <string>

/**
 * @brief Resource handling related functionality
 */
namespace resources
{

    /**
     * @brief This class provides static methods to deal with GTK resources
     */
    class ResourceHelper
    {

        public:

            /**
             * @brief Extracts a resource to a string
             * 
             * @param path The path of the resource
             * 
             * @return The text value of the resource, or an empty string if it does not exits
             */
            static std::string global_to_string(const std::string& path);

             /**
             * @brief Extracts a resource to a block of memory
             * 
             * @param path The path of the resource
             * 
             * @return A pointer to the extracted memory, or a null-pointer if it does not exist
             */
            static void* global_to_memory(const std::string& path);

             /**
             * @brief Gets the size of a resource
             * 
             * @param path The path of the resource
             * 
             * @return The size of the resource, in bytes; or -1 if it does not exist
             */
            static unsigned long global_get_size(const std::string& path);

        private:

             /**
             * @brief Accesses a resource by opening a stream
             * 
             * @param path The path of the resource
             * 
             * @return A pointer to an input stream, or a empty pointer if it does not exist
             */
            static Glib::RefPtr<Gio::InputStream> global_get_stream(const std::string& path);

    };

}

#endif