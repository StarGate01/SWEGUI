/**
 * @file EditProbeDialog.hpp
 * 
 * @brief Editing dialog for probes
*/

#ifndef EDIT_PROBE_DIALOG_H
#define EDIT_PROBE_DIALOG_H

#include <gtkmm.h>
#include <string>

#define PATH_TO_PROBE_GUI "/main/src/ui/probe.glade"

namespace swegui
{
    
    class MainWindow;

    /**
     * @brief Dialog for editing probes
     * 
     * This dialog allows editing of the name and position of probes
    */
    class EditProbeDialog : public Gtk::Dialog
    {

        public:
        
            /**
             * @brief Inherited constructor of Gtk::Dialog
             * 
             * @param cobject
             * 
             * @refBuilder Gtk::Builder of the parent window
            */
            EditProbeDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
            
            /**
             * @brief Creates a new instance of EditProbeDialog
             * 
             * @param pa parent MainWindow
             * 
             * @return Pointer to new instance of EditProbeDialog
            */
            static EditProbeDialog* create(MainWindow* pa);

            /**
             * @brief Adjust the minimum and maximum accepted value for x and y
             * 
             * @param xmin Minimum for x value
             * @param xmax Maximum for x value
             * @param ymin Minimum for y value
             * @param ymax Maximum for y value
            */
            void set_min_max(float xmin, float xmax, float ymin, float ymax);

            /**
             * @brief Setter for the name of the probe
             * 
             * @param name Name of the probe
            */
            void set_name(std::string name);

            /**
             * @brief Getter for the name of the probe
             * 
             * @return Name of the probe
            */
            std::string get_name();

            /**
             * @brief Setter for the x value of the probe
             * 
             * @param x value
            */
            void set_x(float x);

            /**
             * @brief Getter for the x value of the probe
             * 
             * @param x value
            */
            float get_x();

            /**
             * @brief Setter for the y value of the probe
             * 
             * @param y value
            */
            void set_y(float y);

            /**
             * @brief Getter for the y value of the probe
             * 
             * @return y value
            */
            float get_y();

            bool existing = false;                                      ///<Specifies wether the probe already exists (editing) or not

        protected:

            Glib::RefPtr<Gtk::Builder> m_refBuilder;                    ///<Builder of the dialog

        private:

            MainWindow* parent = nullptr;                               ///<Pointer to the parent MainWindow instance
            Glib::RefPtr<Gtk::Adjustment> adjustment_x,                 ///<Pointer to the horizontal adjustment objects of EditProbeWindow::spin_x and EditProbeWindow::spin_y
            adjustment_y;                                               ///<Pointer to the vertical adjustment objects of EditProbeWindow::spin_x and EditProbeWindow::spin_y
            Gtk::Entry* entry_name = nullptr;                           ///<Pointer to the name Gtk::Entry of the GUI
            Gtk::SpinButton* spin_x = nullptr,                          ///<Pointer to the horizontal Gtk::SpinButton element of the GUI
            *spin_y = nullptr;                                          ///<Pointer to the vertical Gtk::SpinButton elements of the GUI
            Gtk::Button* button_okay = nullptr;                         ///<Pointer to the confirmation button of the GUI

            void setup_gui_elements();                                  ///<Initializes the individual elements of the GUI

    };

}

#endif