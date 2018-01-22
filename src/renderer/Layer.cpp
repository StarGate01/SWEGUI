#include <iostream>
#include <sstream>
#include "Layer.hpp"

using namespace renderer;
using namespace std;

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
