#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <SFML/Graphics.hpp>
#include "ShaderConstants.hpp"

namespace renderer
{

    class Layer
    {

    public:

        std::string name;
        int index;
        sf::Color colors[NUM_LERP_POINTS] = { };
        bool enable = true;
        bool computed = false;
        sf::Texture texture;

        Layer();
        void update_shader(sf::Shader&, bool use_tex = false);
        
    };

}

#endif