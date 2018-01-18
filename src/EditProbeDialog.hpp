#ifndef EDIT_PROBE_DIALOG_H
#define EDIT_PROBE_DIALOG_H

#include <gtkmm.h>

#define PATH_TO_PROBE_GUI "ui/probe.glade"

namespace swegui
{

    class MainWindow; //Forward declaration to resolve circular include dependencies

    class EditProbeDialog : public Gtk::Dialog
    {

        public:

            EditProbeDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            static EditProbeDialog* create(MainWindow* pa);

        private:

            MainWindow* parent = nullptr;

            void setup_gui_elements();

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;

    };

}

#endif