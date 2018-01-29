/**
 * @brief Layer for displaying a single variable in the renderer
 * @file Layer.hpp
*/

#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <SFML/Graphics.hpp>
#include "NetCdfImageStream.hpp"

namespace renderer
{
    /**
     * @brief  Layer for displaying a single variable in the renderer
    */
    class Layer
    {

    public:

        NetCdfImageStream::Meta meta_info;          ///<

        int index;                                  ///<
        bool clip = false;                          ///<
        float clip_min = -1.f, clip_max = 1.f;      ///<
        bool enable = false;                        ///<
        bool computed = false;                      ///<
        sf::Texture texture;                        ///<
        sf::Shader* shader = nullptr;               ///<

        Layer() { }                                 ///<
        void update_shader(bool use_tex = false);   ///<
        
    };

}

#endif