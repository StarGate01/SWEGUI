#ifndef LAYER_WINDOW_H
#define LAYER_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <map>

#define PATH_TO_LAYER_GUI "ui/layers.glade"

namespace renderer
{
    class Layer;
};

namespace swegui
{

    class MainWindow; //Forward declaration to resolve circular include dependencies

    class LayerWindow : public Gtk::Window
    {

        public:

            class LayerUI
            {

                public:

                    Gtk::Switch* switch_enable = nullptr, *switch_clip = nullptr;
                    Gtk::SpinButton* spin_min = nullptr, *spin_max = nullptr;
                    Glib::RefPtr<Gtk::Adjustment> adjustment_min, adjustment_max;
                    renderer::Layer* data_layer = nullptr;

                    LayerUI() { }

                    void update_data();
                    void update_ui();

                    typedef sigc::signal<void> type_signal_update;
                    type_signal_update signal_update();

                protected:

                    type_signal_update m_signal_update;

            };

            LayerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            ~LayerWindow();
            static LayerWindow* create(MainWindow* pa);

            void update_ui();

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;

        private:

            MainWindow* parent;
            std::string layer_names[5] = { "b", "h", "hu", "hv", "hx" };
            LayerUI* layer_uis[5];

            void setup_gui_elements();


    };

}

#endif