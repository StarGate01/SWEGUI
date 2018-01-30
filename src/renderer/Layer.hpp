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

        NetCdfImageStream::Meta meta_info;          ///<Meta information about this layer

        int index;                                  ///<Numerical index of this layer
        bool clip = false;                          ///<Wether clipping is active
        float   clip_min = -1.f,                    ///<Clipping minimum
                clip_max = 1.f;                     ///<Clipping maximum
        bool enable = false;                        ///<Wether this layer should be displayed
        bool computed = false;                      ///<Wether this layer consists purely of dynamic computed data
        sf::Texture texture;                        ///<The texture used as data
        sf::Shader* shader = nullptr;               ///<Reference to the shader to update its uniforms

        Layer() { }                                 ///<Constructor
        void update_shader(bool use_tex = false);   ///<Updates the shader by setting its uniforms
        
    };

}

#endif