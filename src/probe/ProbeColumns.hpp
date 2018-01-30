/**
 * @brief Template for the columns represented in the probe data window
 * 
 * @file ProbeColumns.hpp
*/

#ifndef PROBE_COLUMNS_H
#define PROBE_COLUMNS_H

#include <gtkmm.h>
#include <string>

namespace probe
{
    
    /**
     * @brief Template for probe columns, consisting of a x and y coordinate and a name
    */
    class ProbeColumns: public Gtk::TreeModel::ColumnRecord 
    {

        public:

            ProbeColumns();                                 ///<Initializes all columns
            Gtk::TreeModelColumn<std::string> col_name;     ///<Name column
            Gtk::TreeModelColumn<float> col_x;              ///<X column
            Gtk::TreeModelColumn<float> col_y;              ///<Y column

    };

}

#endif