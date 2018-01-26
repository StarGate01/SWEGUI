#ifndef PROBE_COLUMNS_H
#define PROBE_COLUMNS_H

#include <gtkmm.h>
#include <string>

namespace probe
{
    /**
     * @brief Columns in the probe list
    */
    class ProbeColumns: public Gtk::TreeModel::ColumnRecord 
    {

        public:
            /**
            * @brief Creates name, x and y position columns for probes
            */
            ProbeColumns();

            Gtk::TreeModelColumn<std::string> col_name;
            Gtk::TreeModelColumn<float> col_x;
            Gtk::TreeModelColumn<float> col_y;

    };

}

#endif