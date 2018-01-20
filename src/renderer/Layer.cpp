#include <iostream>
#include <sstream>
#include "Layer.hpp"

using namespace renderer;
using namespace std;

Layer::Layer()
{
    colors[0] = sf::Color::Blue;
    colors[0].a = 0;
    colors[1] = sf::Color::White;
    colors[1].a = 128;
    for(int i=2; i<8; i++) colors[i] = sf::Color::Red;
}

void Layer::update_shader(bool use_tex)
{
    stringstream ss;
    if(use_tex && !computed) 
    {
        ss << name << "_tex";
        shader->setParameter(ss.str(), texture);
        ss.str(string());
        ss.clear();
    }
    for(int i=0; i<NUM_LERP_POINTS; i++) 
    {
        ss << name << "_colors[" << i << "]";
        shader->setParameter(ss.str(), colors[i]);
        ss.str(string());
        ss.clear();
    }
    ss << "enable_layers[" << index << "]";
    shader->setParameter(ss.str(), enable);
    ss.str(string());
    ss.clear();
    ss << name << "_clip";
    if(clip)
    {
        shader->setParameter(ss.str(), sf::Vector2f((clip_min - meta_info.min) / abs(meta_info.max - meta_info.min), 
            (clip_max - meta_info.min) / abs(meta_info.max - meta_info.min)));
    }
    else shader->setParameter(ss.str(), sf::Vector2f(0.f, 1.f));
}
