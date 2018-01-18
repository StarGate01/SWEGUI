/**
 * @file main.glsl
 * @brief The main shader for topographical rendering
 * @namespace GLSL
 * @class MainShader
 */

#define NUM_LERP_POINTS 8  ///< How many points there are in a color map
#define SQRT2           1.41421356237

uniform sampler2D b_tex; ///< The bathymetry texture
uniform sampler2D h_tex; ///< The water height texture
uniform sampler2D hu_tex; ///< The horizontal flux texture
uniform sampler2D hv_tex; ///< The vertical flux texture
uniform vec2 screensize; ///< The screen size in pixels
uniform vec2 padding; ///< Padding for aspect ratio
uniform vec4[NUM_LERP_POINTS] b_colors; ///< The color map for the bathymetry
uniform vec4[NUM_LERP_POINTS] h_colors; ///< The color map for the water height
uniform vec4[NUM_LERP_POINTS] hu_colors; ///< The color map for the horizontal flux
uniform vec4[NUM_LERP_POINTS] hv_colors; ///< The color map for the vertical flux
uniform vec4[NUM_LERP_POINTS] hx_colors; ///< The color map for the total flux
uniform bool[5] enable_layers; ///< What layers to enable

/**
 * @brief Computes the blending factor needed based on the number of active layers
 *
 * @param[out] factor The computed blending factor
 */
void blend_factor(out float factor)
{
    float num = 0.0;
    for(int i=0; i<5; i++) if(enable_layers[i]) num += 1.0;
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
    val = col.b + (col.g / 100.0) + (col.r / 10000.0);
}

/**
 * @brief Lerps the color of a relative value by iterating a color map
 *
 * @param[in] val The relative value
 * @param[in] colors The color map
 * @param[out] col The computed color
 */
void multi_lerp(in float val, in vec4[NUM_LERP_POINTS] colors, out vec3 col)
{
    int si = 0, ei = 0;
    for(int i=0; i<NUM_LERP_POINTS; i++)
    {
        if(colors[i].w <= val) si = i;
        if(ei == 0 && colors[i].w >= val) ei = i;
    }
    col = mix(colors[si].xyz, colors[ei].xyz, 
        (val  - colors[si].w) / (colors[ei].w - colors[si].w));
}

/**
 * @brief Computes the color of a sampled value based on a color map
 *
 * @param[in] sval The sampled value
 * @param[in] colors The color map
 * @param[out] pmacol The computed color
 */
void compute_color(in vec4 sval, in vec4[NUM_LERP_POINTS] colors, out vec3 pmacol)
{
    float tval; recombine(sval, tval);
    multi_lerp(tval, colors, pmacol);
}

/**
 * @brief The main fragment routine
 */
void main()
{
    if(gl_FragCoord.x < padding.x || gl_FragCoord.x > (screensize.x - padding.x)
        || gl_FragCoord.y < padding.y || gl_FragCoord.y > (screensize.y - padding.y))
    {
        gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
    }
    else
    {
        vec2 pos = vec2((gl_FragCoord.x - padding.x) / (screensize.x - (padding.x * 2.0)), 
            (gl_FragCoord.y - padding.y) / (screensize.y - (padding.y * 2.0)));
        float factor; blend_factor(factor);

        vec3 b, h, hu, hv, hx;
        compute_color(texture2D(b_tex, pos), b_colors, b);
        compute_color(texture2D(h_tex, pos), h_colors, h);
        compute_color(texture2D(hu_tex, pos), hu_colors, hu);
        compute_color(texture2D(hv_tex, pos), hv_colors, hv);
        multi_lerp(length(abs(hu) + abs(hv)) / SQRT2, hx_colors, hx);

        vec3 sum = vec3(0.0, 0.0, 0.0);
        if(enable_layers[0]) sum += b * factor;
        if(enable_layers[1]) sum += h * factor;
        if(enable_layers[2]) sum += hu * factor;
        if(enable_layers[3]) sum += hv * factor;
        if(enable_layers[4]) sum += hx * factor;

        gl_FragColor = vec4(sum, 1.0);
    }
}