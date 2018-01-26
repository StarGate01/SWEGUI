/**
 * @file scale.frag
 * @brief The shader for displaying a color scale
 * @namespace shader
 * @class Scale
 */

#define NUM_LAYERS      5       ///< The number of layers
#define LUT_BW_I        0.25    ///< The y in the LUT for black and white
#define LUT_BLR_I       0.75    ///< The y in the LUT for blue and red
#define B_I             0       ///< Index of the bathymetry layer
#define H_I             1       ///< Index of the water height layer
#define HU_I            2       ///< Index of the horizontal flux layer
#define HV_I            3       ///< Index of the vertical flux layer
#define HX_I            4       ///< Index of the total flux layer

uniform sampler2D lut;  ///< The color lookup tables
uniform mat4 transform; ///< Screen space to texture space transformation
uniform float layer;    ///< The layer id

/**
 * @brief The main fragment routine
 */
void main()
{
    float[NUM_LAYERS] lutmap;
    lutmap[B_I] = LUT_BW_I; lutmap[H_I] = LUT_BLR_I; lutmap[HU_I] = LUT_BLR_I; 
    lutmap[HV_I] = LUT_BLR_I; lutmap[HX_I] = LUT_BLR_I;

    float luty = lutmap[int(layer)];
    vec2 pos = (transform * vec4(gl_FragCoord.x, gl_FragCoord.y, 1.0, 1.0)).xy;
    gl_FragColor = vec4(texture2D(lut, vec2(pos.x, luty)).rgb, 1.0);
}