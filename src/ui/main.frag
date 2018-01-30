/**
 * @file main.frag
 * @brief Implements the main shader for topographical rendering
 * @namespace shader
 * @class Main
 */

#define NUM_LAYERS      5       ///< The number of layers
#define LUT_BW_I        0.25    ///< The y in the LUT for black and white
#define LUT_BLR_I       0.75    ///< The y in the LUT for blue and red
#define B_I             0       ///< Index of the bathymetry layer
#define H_I             1       ///< Index of the water height layer
#define HU_I            2       ///< Index of the horizontal flux layer
#define HV_I            3       ///< Index of the vertical flux layer
#define HX_I            4       ///< Index of the total flux layer

#define NUM_TEXTURES    4       ///< The number of textures (non-computed layers)

uniform sampler2D[NUM_TEXTURES] tex;    ///< The data textures
uniform mat4 transform;                 ///< Screen space to data space transformation
uniform sampler2D lut;                  ///< The color lookup tables
uniform vec2[NUM_LAYERS] clip;          ///< The clipping values
uniform bool[NUM_LAYERS] enable;        ///< What layers to enable

/**
 * @brief Computes the blending factor needed based on the number of active layers
 *
 * @param[out] factor The computed blending factor
 */
void blend_factor(out float factor)
{
    float num = 0.0;
    for(int i=0; i<NUM_LAYERS; i++) if(enable[i]) num += 1.0;
    factor = 1.0 / num;
}

/**
 * @brief Recombines a color to a relative value
 * 
 * This undoes the transport data compression.
 *
 * @param[in] col The color
 * @param[out] val The computed relative value
 */
void recombine(in vec4 col, out float val)
{
    val = col.b + (col.g / 255.0) + (col.r / (255.0 * 255.0));
}

/**
 * @brief Computes the color of a sampled value based on a color map
 *
 * @param[in] tval The relative value
 * @param[in] luty The color lookup table y offset
 * @param[in] clip The clipping range
 * @param[out] pmacol The computed color
 */
void compute_color(in float tval, in float luty, in vec2 clip, out vec3 pmacol)
{
    pmacol = texture2D(lut, vec2(smoothstep(clip.x, clip.y, tval), luty)).rgb;
}

/**
 * @brief The main fragment routine
 */
void main()
{
    //apply screen - texture affine transformation
    vec2 pos = (transform * vec4(gl_FragCoord.x, gl_FragCoord.y, 1.0, 1.0)).xy;

    //test for out of bounds
    if(pos.x < 0.0 || pos.x > 1.0 || pos.y < 0.0 || pos.y > 1.0)
    {
        gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
    }
    else
    {
        //compute blend factor
        float factor; 
        blend_factor(factor);

        //set up lut - layer association
        float[NUM_LAYERS] lutmap;
        lutmap[B_I] = LUT_BW_I; lutmap[H_I] = LUT_BLR_I; lutmap[HU_I] = LUT_BLR_I; 
        lutmap[HV_I] = LUT_BLR_I; lutmap[HX_I] = LUT_BLR_I;

        //declare values and colors
        float[NUM_LAYERS] values_real;
        vec3[NUM_LAYERS] values_color;
        vec3 sum = vec3(0.0, 0.0, 0.0);

        //unpack pixel data to actual values
        #define RECOM(TI) recombine(texture2D(tex[TI], pos), values_real[TI])
        RECOM(B_I); RECOM(H_I); RECOM(HU_I); RECOM(HV_I);

        //compute total flux 0.5 * (hu^2 + hv^2)
        values_real[HX_I] = ((values_real[HU_I] * values_real[HU_I]) + (values_real[HV_I] *  values_real[HV_I])) / 2.0;

        //compute and enable pixel color
        for(int i=0; i<NUM_LAYERS; i++)
        {
            compute_color(values_real[i], lutmap[i], clip[i], values_color[i]);
            if(enable[i]) sum += values_color[i] * factor;
        }
        
        gl_FragColor = vec4(sum, 1.0);
    }
}