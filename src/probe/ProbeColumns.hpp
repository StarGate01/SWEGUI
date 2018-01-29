/**
 * @brief Template for the columns represented in the probe data window
 * @file ProbeColumns.hpp
*/

#ifndef PROBE_COLUMNS_H
#define PROBE_COLUMNS_H

#include <gtkmm.h>
#include <string>

namespace probe
{
    /**
<<<<<<< HEAD
     * @brief Columns in the probe list
=======
     * @brief Template for probe columns, consisting of a x and y coordinate and a name
>>>>>>> 43b3419e42dd269d11e02df68a580165aa0bd5cd
    */
    class ProbeColumns: public Gtk::TreeModel::ColumnRecord 
    {

        public:
<<<<<<< HEAD
            /**
            * @brief Creates name, x and y position columns for probes
            */
            ProbeColumns();
=======

            ProbeColumns();                                 ///<Initializes all columns
>>>>>>> 43b3419e42dd269d11e02df68a580165aa0bd5cd

            Gtk::TreeModelColumn<std::string> col_name;     ///<Name column
            Gtk::TreeModelColumn<float> col_x;              ///<X column
            Gtk::TreeModelColumn<float> col_y;              ///<Y column

    };

}

#endif