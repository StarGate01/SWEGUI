/**
 * @file DataFieldWidget.cpp
 * 
 * @brief Implements the functionality defined in DataFieldWidget.hpp
*/

#include <sstream>
#include <math.h>
#include <vector>
#include <limits>
#include "DataFieldWidget.hpp"
#include "../MainWindow.hpp"

using namespace widgets;

DataFieldWidget::DataFieldWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Notebook(cobject), m_refBuilder(refBuilder)
{ }

DataFieldWidget::~DataFieldWidget()
{
    signal_done_fill_data_handler.disconnect();
}

DataFieldWidget* DataFieldWidget::create(swegui::MainWindow* pa, std::string na)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_DATAFIELD_GUI);
    DataFieldWidget* widget = nullptr;
    refBuilder->get_widget_derived("notebook_main", widget);
    widget->parent = pa;
    widget->name = na;
    widget->setup_gui_elements();
    return widget;
}

void DataFieldWidget::setup_gui_elements()
{
    //Raw data tab
    for(int i = 0; i < 5; i++) m_refBuilder->get_widget("lbl_"+layer_names[i], labels[i]);
    
    m_refBuilder->get_widget("cb_layer", cb_layer);
    m_refBuilder->get_widget("btn_save_graph", btn_save_graph);
    m_refBuilder->get_widget("sfd_graph_export", sfd_save);
    m_refBuilder->get_widget("drawingarea_chart", drawingarea_chart);

    sfd_save->set_transient_for(*parent);

    drawingarea_chart->signal_draw().connect(sigc::mem_fun(this, &DataFieldWidget::on_chart_draw));
    cb_layer->signal_changed().connect(sigc::mem_fun(this, &DataFieldWidget::on_dataset_change));
    btn_save_graph->signal_clicked().connect(sigc::mem_fun(this, &DataFieldWidget::on_graph_export));

    update_ui();
}

void DataFieldWidget::update_ui()
{
    if(name != "")
    {
        probe::DataProbe* probe = parent->data_renderer->probes[name];
        if(probe == nullptr)
        {
            reset_gui();
            return;
        }

        //Set labels
        float bathy = parent->data_renderer->sample((renderer::NetCdfImageStream::Variable)0, probe->x, probe->y);
        labels[0]->set_text(std::to_string(bathy));

        //Set background for bathymetry
        if(bathy > 0) labels[0]->override_color (Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
        else labels[0]->override_color (Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);

        for(int i=1; i<4; i++)
            labels[i]->set_text(std::to_string(
                parent->data_renderer->sample((renderer::NetCdfImageStream::Variable)i, probe->x, probe->y)));

        //Compute hx
        float hx = std::abs(std::sqrt(std::pow(parent->data_renderer->sample(
            renderer::NetCdfImageStream::Variable::Hu, probe->x, probe->y), 2)
            + std::pow(parent->data_renderer->sample(
                renderer::NetCdfImageStream::Variable::Hv, probe->x, probe->y), 2)));
        labels[4]->set_text(std::to_string(hx));

        signal_done_fill_data_handler = probe->signal_done_fill_data().connect(sigc::mem_fun(this, &DataFieldWidget::update_ui));
        on_dataset_change();
    }
    else reset_gui();
}

void DataFieldWidget::reset_gui()
{
    for(auto label : labels) label->set_text("N/a");
    signal_done_fill_data_handler.disconnect();
    on_dataset_change();

}

void DataFieldWidget::on_dataset_change(void)
{
    if(drawingarea_chart == nullptr || !drawingarea_chart->get_window()) return;
    queue_draw();
}

void DataFieldWidget::on_graph_export(void)
{
    if(sfd_save->run() == Gtk::RESPONSE_OK)
    {
        std::string filename = sfd_save->get_filename();
        save_screenshot(filename);
    }
    sfd_save->hide();
}

bool DataFieldWidget::on_chart_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(name == "") return true;
    probe::DataProbe* probe = parent->data_renderer->probes[name];

    //Setup rendering
    Gtk::Allocation allocation = drawingarea_chart->get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    Gdk::Color color(COLOR_DEFAULT);

    if(probe == nullptr || !probe->has_data()) 
    {
        cr->set_source_rgb(0,0,0);
        Pango::FontDescription font;
        font.set_family("Monospace");
        font.set_weight(Pango::WEIGHT_BOLD);
        // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
        auto layout = create_pango_layout("Loading data...");
        int text_width;
        int text_height;
        //get the text dimensions (it updates the variables -- by reference)
        layout->get_pixel_size(text_width, text_height);
        // Position the text in the middle
        cr->move_to((width-text_width)/2, (height-text_height)/2);
        layout->show_in_cairo_context(cr);
        return true;
    }

    //Setup for data rendering
    int layer = cb_layer->get_active_row_number();          //Get current layer
    std::vector<float> data = probe->get_all_data(layer);   //Get data
    if(data.size() <= 0) return false;                      //Return if data is empty

    //Set rendering constraints
    float max_value = *std::max_element(data.begin(), data.end());
    float min_value = *std::min_element(data.begin(), data.end());

    int min_t = -1;
    int max_t = -1;

    //------------- Start drawing -------------//
    //Draw background
    cr->set_source_rgba(1.0, 1.0, 1.0, 1.0);
    cr->paint();

    //Do not draw graph if bathymetry > 0
    if(probe->is_dry()) //probe->get_data(0, 0) > 0)
    {
        cr->set_source_rgb(0,0,0);
        Pango::FontDescription font;
        font.set_family("Monospace");
        font.set_weight(Pango::WEIGHT_BOLD);
        // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
        auto layout = create_pango_layout("This probe is dry");
        int text_width;
        int text_height;
        //get the text dimensions (it updates the variables -- by reference)
        layout->get_pixel_size(text_width, text_height);
        // Position the text in the middle
        cr->move_to((width-text_width)/2, (height-text_height)/2);
        layout->show_in_cairo_context(cr);
        return true;
    }

    cr->set_line_width(2.0);

    //Set drawing color based on selected layer
    switch(layer)
    {
        case 0: cr->set_source_rgb(51.0/255.0, 204.0/255.0, 255.0/255.0); break; //Water height
        case 1: cr->set_source_rgb(204.0/255.0, 255.0/255.0, 102.0/255.0); break; //H flux
        case 2: cr->set_source_rgb(179.0/255.0, 255.0/255.0, 102.0/255.0); break; //V flux
        case 3: cr->set_source_rgb(140.0/255.0, 255.0/255.0, 102.0/255.0); break; //T flux
    }

    //Move to lower left corner
    cr->move_to(0, height);

    //Iterate though items
    if(min_value != max_value)
    {    
        for(int t = 0; t < (int)data.size(); t++)
        {
            cr->line_to(
                calculate_graph_width(t, (int) data.size(), width), 
                calculate_graph_height(data[t], min_value, max_value, GRAPH_SCALE, height));
            if(min_value == data[t]) min_t = t;
            else if(max_value == data[t]) max_t = t;
        }
    }
    else    //min_value == max_value
    {
        cr->line_to(0, height/2);
        cr->line_to(width, height/2);
        cr->line_to(width, height);
        cr->fill();
    }
    

    //Close path
    double c_x, c_y;
    cr->get_current_point(c_x, c_y);
    cr->line_to(c_x, height);
    cr->close_path();
    cr->fill();

    //Draw max_value
    cr->set_line_width(4);
    if(max_t >= 0)
    {
        cr->set_source_rgb(1,0.2,0.7);
        cr->move_to(
            calculate_graph_width(max_t, (int) data.size(), width), 
            calculate_graph_height(data[max_t], min_value, max_value, GRAPH_SCALE, height));
        cr->line_to(
            calculate_graph_width(max_t, (int) data.size(), width),
            height);
        cr->stroke();
    }
    //Draw min line (if not total flux)
    if(min_t >= 0 && layer != 3)
    {
        cr->set_source_rgb(0.3,0.3,1);
        cr->move_to(
            calculate_graph_width(min_t, (int) data.size(), width), 
            calculate_graph_height(data[min_t], min_value, max_value, GRAPH_SCALE, height));
        cr->line_to(
            calculate_graph_width(min_t, (int) data.size(), width),
            height);
        cr->stroke();
    }

    //Draw horizonal lines
    cr->set_line_width(1);
    cr->set_source_rgba(0.5, 0.5, 0.5, 0.8);
    for(int i = 1; i < LEGEND_H_DEVISION; i++)
    {
        cr->move_to(0, (height/LEGEND_H_DEVISION) * i);
        cr->line_to(width, (height/LEGEND_H_DEVISION) * i);
        cr->stroke();
    }
    
    //Draw vertical lines
    int vlines = std::min(LEGEND_V_MAX_DEVISION, (int) data.size());
    for(int i = 1; i < vlines; i++)
    {
        cr->move_to((width/vlines) * i, 0);
        cr->line_to((width/vlines) * i, height);
        cr->stroke();
    }
    cr->stroke();

    float zero_height = calculate_graph_height(0, min_value, max_value, GRAPH_SCALE, height);

    //Draw zero line
    cr->set_source_rgba(0.2, 0.2, 0.2, 1.0);
    cr->move_to(0, zero_height);
    cr->line_to(width, zero_height);
    cr->stroke();

    //Draw timestamp line
    cr->set_line_width(5);
    cr->set_source_rgb(1.0, 0.0, 0.0);
    float x = calculate_graph_width(parent->data_renderer->get_current_timestamp(), (int) data.size(), width);
    cr->move_to(x, height);
    cr->line_to(x, 0);
    cr->stroke();

    //Draw legend
    //maximum
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->set_font_size(LEGEND_FONT_SIZE);
    cr->move_to(5, (1 - GRAPH_SCALE) * height - MIN_OFFSET * height + 0.5 * LEGEND_FONT_SIZE);
    cr->show_text(std::to_string(max_value));
    //zero line
    cr->move_to(5, zero_height - 3);
    cr->show_text(std::to_string(0.0000));
    //minimum
    if(layer != 3) //total flux 0 is minimum
    {
        cr->move_to(5, (GRAPH_SCALE) * height + MIN_OFFSET * height + 0.5 * LEGEND_FONT_SIZE);
        cr->show_text(std::to_string(min_value));
    }
    return true;
}

float DataFieldWidget::calculate_graph_height(float data, float min, float max, float scale, int graph_height)
{
    return (1-MIN_OFFSET-(data-min) / (max-min) * scale) * (float) graph_height;
    //return graph_height - MIN_OFFSET * (float) graph_height - ((data-min) / (max - min)) * scale * (float) graph_height;
}

float DataFieldWidget::calculate_graph_width(int timestep, int timesteps_total, int graph_width)
{
    return ((float) timestep / ((float)timesteps_total - 1)) * (float)graph_width;
}

void DataFieldWidget::save_screenshot(std::string path)
{
    const Cairo::RefPtr<Cairo::Context>& cr = drawingarea_chart->get_window()->create_cairo_context();
    auto surface = cr->get_target();
    surface->write_to_png(path);
}