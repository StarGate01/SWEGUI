#include "MainWindow.hpp"

using namespace swegui;

probe::ProbeColumns MainWindow::probelist_columns;

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder)
{
    setup_gui_elements();
}

MainWindow::~MainWindow()
{
    delete data_renderer;
}

MainWindow* MainWindow::create()
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_MAIN_GUI);
    MainWindow* window = nullptr;
    refBuilder->get_widget_derived("window_main", window);
    window->add_events(Gdk::EventMask::BUTTON_PRESS_MASK | Gdk::EventMask::BUTTON_RELEASE_MASK
        | Gdk::EventMask::BUTTON2_MOTION_MASK | Gdk::EventMask::BUTTON3_MOTION_MASK | Gdk::EventMask::SCROLL_MASK);
    return window;
}

void MainWindow::setup_gui_elements()
{
    //Get menu bar
    m_refBuilder->get_widget("main_menubar", menubar_main);
    m_refBuilder->get_widget("mb_file_open", mb_file_open);
    m_refBuilder->get_widget("mb_file_quit", mb_file_quit);
    m_refBuilder->get_widget("mb_view_layer", mb_view_layer);
    m_refBuilder->get_widget("mb_view_renderer", mb_view_renderer);
    m_refBuilder->get_widget("mb_view_reset", mb_view_reset);
    m_refBuilder->get_widget("mb_simulation_goto_start", mb_simulation_goto_start);
    m_refBuilder->get_widget("mb_simulation_prev", mb_simulation_prev);
    m_refBuilder->get_widget("spin_timestamp", mb_spin_timestamp);
    adjustment_timestamp = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(m_refBuilder->get_object("adjustment_timestamp"));
    m_refBuilder->get_widget("lbl_realtime", lbl_realtime);
    m_refBuilder->get_widget("mb_simulation_play", mb_simulation_play);
    m_refBuilder->get_widget("mb_simulation_next", mb_simulation_next); 
    m_refBuilder->get_widget("mb_tools_shoreanalysis", mb_tool_cda);
    m_refBuilder->get_widget("mb_tools_crosssection", mb_tool_crosssection);
    m_refBuilder->get_widget("mb_help_about", mb_help_about);
    //Get toolbar
    m_refBuilder->get_widget("main_toolbar", toolbar_main);
    m_refBuilder->get_widget("tbtn_open", tb_openfile);
    m_refBuilder->get_widget("tbtn_simulation_goto_start", tb_simulation_goto_start);
    m_refBuilder->get_widget("tbtn_simulation_play", tb_simulation_play);
    m_refBuilder->get_widget("tbtn_layer", tb_layer);
    m_refBuilder->get_widget("tbtn_renderer", tb_renderer);
    m_refBuilder->get_widget("tbtn_zoompan_reset", tb_zoompan_reset);
    m_refBuilder->get_widget("tbtn_tool_cda", tb_tool_cda);
    m_refBuilder->get_widget("tbtn_tool_crosssection", tb_crosssection);
    m_refBuilder->get_widget("tbtn_screenshot", tb_screenshot);
    //Get raw data label
    //m_refBuilder->get_widget("lbl_info", lbl_raw_data);
    m_refBuilder->get_widget("alignment_frame_probedata", alignment_frame_probedata);
    probedata = widgets::DataFieldWidget::create(this, "");
    alignment_frame_probedata->add(*probedata);
    //probe list
    m_refBuilder->get_widget("treeview_probes", probelist);
    m_refBuilder->get_widget("context_menu_probelist", contextmenu_probelist);
    probelist_store = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(m_refBuilder->get_object("liststore_probes"));
    m_refBuilder->get_widget("button_probe_add", button_probe_add);
    //Its context menu
    m_refBuilder->get_widget("menuitem_probelist_open", menuitem_probelist_open);
    m_refBuilder->get_widget("menuitem_probelist_edit", menuitem_probelist_edit);
    m_refBuilder->get_widget("menuitem_probelist_remove", menuitem_probelist_remove);
    //Get SFML control, init renderer
    m_refBuilder->get_widget("alignment_frame_sfml", alignment_frame_sfml);
    sfml_area = widgets::SFMLWidget::create(this);
    alignment_frame_sfml->add(*sfml_area);
    data_renderer = new renderer::DataRenderer(*sfml_area);
    //Dialogs
    m_refBuilder->get_widget("dialog_open", dialog_open);
    m_refBuilder->get_widget("dialog_about", dialog_about);
    dialog_probe_edit = EditProbeDialog::create(this);
    //Windows
    window_layers = LayerWindow::create(this);
    window_renderer = RendererWindow::create(this);

    //Event handlers for menubar
    mb_file_open->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_fileopen));
    mb_file_quit->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_quit));
    mb_view_layer->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_layer));
    mb_view_renderer->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_renderer));
    mb_view_reset->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_zoompan_reset));
    mb_simulation_goto_start->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_goto_start));
    mb_simulation_prev->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_prev));
    mb_spin_timestamp->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_set_timestamp));
    mb_simulation_play->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_play));
    mb_simulation_next->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_next));
    mb_tool_cda->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_cda));
    mb_tool_crosssection->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_crosssection));
    mb_help_about->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_about));
    //Event handlers for toolbar
    tb_openfile->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_fileopen));
    tb_simulation_goto_start->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_goto_start));
    tb_simulation_play->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_simulation_play));
    tb_layer->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_layer));
    tb_renderer->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_renderer));
    tb_zoompan_reset->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_zoompan_reset));
    tb_tool_cda->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_cda));
    tb_screenshot->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_screenshot));
    tb_crosssection->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_crosssection));
    
    //Event handlers probe list
    probelist->add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    probelist->signal_row_activated().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_activate));
    probelist->signal_button_press_event().connect_notify(sigc::mem_fun(this, &MainWindow::on_action_probelist_button_press));
    probelist->get_selection()->signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_changed));
    button_probe_add->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_action_button_probe_add));
    //And its context menu
    menuitem_probelist_open->signal_activate().connect(sigc::mem_fun(this, &MainWindow::open_probe_ui));
    menuitem_probelist_edit->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_action_probelist_context_edit));
    menuitem_probelist_remove->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_probe_remove));
    //Event handlers for sfml widget
    data_renderer->signal_update().connect(sigc::mem_fun(this, &MainWindow::on_probe_update));
    data_renderer->signal_select().connect(sigc::mem_fun(this, &MainWindow::on_probe_select));
}