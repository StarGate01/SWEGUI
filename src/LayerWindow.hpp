#ifndef LAYER_WINDOW_H
#define LAYER_WINDOW_H

#include <gtkmm.h>

#define PATH_TO_LAYER_GUI "ui/layers.glade"

namespace swegui
{

    class GuiApplicationWindow; //Forward declaration to resolve circular include dependencies

    class LayerWindow : public Gtk::Window
    {

        public:

            LayerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            static LayerWindow* create(GuiApplicationWindow* pa);

        private:

            GuiApplicationWindow* parent = nullptr;

            Gtk::Switch* switch_b = nullptr, *switch_h = nullptr, 
                *switch_hu = nullptr, *switch_hv = nullptr, *switch_hx = nullptr;

            void setup_gui_elements();
            void on_layer_switch_changed();

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;

    };

}

#endif