#include <iostream>
#include <sstream>
#include "Layer.hpp"

using namespace renderer;
using namespace std;

Layer::Layer()
{
    colors[0] = sf::Color::Blue;
    colors[0].a = 0;
    colors[1] = sf::Color::Yellow;
    colors[1].a = 128;
    colors[2] = sf::Color::Red;
    colors[2].a = 255;
}

void Layer::update_shader(sf::Shader& shader, bool use_tex)
{
    stringstream ss;
    if(use_tex && !computed) 
    {
        ss << name << "_tex";
        shader.setParameter(ss.str(), texture);
        ss.str(string());
        ss.clear();
    }
    for(int i=0; i<NUM_LERP_POINTS; i++) 
    {
        ss << name << "_colors[" << i << "]";
        shader.setParameter(ss.str(), colors[i]);
        ss.str(string());
        ss.clear();
    }
    ss << "enable_layers[" << index << "]";
    shader.setParameter(ss.str(), enable);
}