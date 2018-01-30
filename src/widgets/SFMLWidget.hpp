/**
 * @file SFMLWidget.hpp
 * 
 * @brief Widget to draw the rendered map of the SWE simulation
*/

#ifndef SFML_WIDGET_H
#define SFML_WIDGET_H

#include <gtkmm.h>
#include <SFML/Graphics.hpp>

#define PATH_TO_SFML_GUI "/main/src/ui/sfml.glade"

namespace swegui
{
  /**
   * @brief Main GUI window
  */
  class MainWindow;

}

namespace widgets
{

  /**
   * @brief Widget to draw the rendered map of the SWE simulation
  */
  class SFMLWidget : public Gtk::Bin
  {

    public:

      sf::RenderWindow renderWindow;                              ///<sf::RenderWindow, where the map is rendered

      /**
       * @brief Inherited constructor of Gtk::Bin
       * 
       * @param cobject Calling object
       * @param refBuilder Pointer to Gtk::Builder object of the parent window
      */
      SFMLWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

      /**
       * @brief Creates a new instance of widgets::SFMLWidget
       * 
       * @param pa Parent swegui::MainWindow
      */
      static SFMLWidget* create(swegui::MainWindow* pa);

      void invalidate();                                          ///<Invalidates the SFMLWidget::m_refGdkWindow
      void display();                                             ///<Displays the SFML::m_refGdkWindow

    protected:

      Glib::RefPtr<Gtk::Builder> m_refBuilder;                    ///<Gtk::Builder object of the associated window
      sf::VideoMode m_vMode;                                      ///<Defines the render width, height, bpp

      /**
       * @brief Allocates the render window according to /p allocation
      */
      virtual void on_size_allocate(Gtk::Allocation &allocation);
      virtual void on_realize();                                  //Sets the initial attributes for the window and draws the window
      virtual void on_unrealize();                                //Clears the drawn window

      Glib::RefPtr<Gdk::Window> m_refGdkWindow;                   ///<Reference to associated Gdk::Window

    private:

      swegui::MainWindow* parent = nullptr;                       ///<Pointer to parent swegui::MainWindow

      void setup_gui_elements();                                  ///<Initializes the GUI elements

  };

}

#endif