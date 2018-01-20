#ifndef EDIT_PROBE_DIALOG_H
#define EDIT_PROBE_DIALOG_H

#include <gtkmm.h>
#include <string>

#define PATH_TO_PROBE_GUI "ui/probe.glade"

namespace swegui
{

    class MainWindow; //Forward declaration to resolve circular include dependencies

    class EditProbeDialog : public Gtk::Dialog
    {

        public:

            EditProbeDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            static EditProbeDialog* create(MainWindow* pa);

            void set_min_max(float xmin, float xmax, float ymin, float ymax);
            void set_name(std::string name);
            std::string get_name();
            void set_x(float x);
            float get_x();
            void set_y(float y);
            float get_y();
            bool existing = false;

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;

        private:

            MainWindow* parent = nullptr;

            Glib::RefPtr<Gtk::Adjustment> adjustment_x, adjustment_y;
            Gtk::Entry* entry_name = nullptr;
            Gtk::SpinButton* spin_x = nullptr, *spin_y = nullptr;
            Gtk::Button* button_okay = nullptr;

            void setup_gui_elements();

    };

}

#endif