#include <string>
#include <cmath>
#include "Layer.hpp"

using namespace renderer;

void Layer::update_shader(bool use_tex)
{
    std::string str_index = std::to_string(index);
    if(use_tex && !computed) shader->setParameter("tex[" + str_index + "]", texture);
    shader->setParameter("enable[" + str_index + "]", enable);
    if(clip)
    {
        shader->setParameter("clip[" + str_index + "]", sf::Vector2f(
            (clip_min - meta_info.min) / abs(meta_info.max - meta_info.min), 
            (clip_max - meta_info.min) / abs(meta_info.max - meta_info.min)));
    }
    else shader->setParameter("clip[" + str_index + "]", sf::Vector2f(0.f, 1.f));
}
