#ifndef PROBE_COLUMNS_H
#define PROBE_COLUMNS_H

#include <gtkmm.h>
#include <string>

namespace probe
{

    class ProbeColumns: public Gtk::TreeModel::ColumnRecord 
    {

        public:

            ProbeColumns();

            Gtk::TreeModelColumn<std::string> col_name;
            Gtk::TreeModelColumn<float> col_x;
            Gtk::TreeModelColumn<float> col_y;

    };

}

#endif