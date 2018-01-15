#define NUM_LERP_POINTS 8
#define SQRT2           1.41421356237

uniform sampler2D b_tex;
uniform sampler2D h_tex;
uniform sampler2D hu_tex;
uniform sampler2D hv_tex;
uniform vec2 screensize;
uniform vec4[NUM_LERP_POINTS] b_colors;
uniform vec4[NUM_LERP_POINTS] h_colors;
uniform vec4[NUM_LERP_POINTS] hu_colors;
uniform vec4[NUM_LERP_POINTS] hv_colors;
uniform vec4[NUM_LERP_POINTS] hx_colors;
uniform bool[5] enable_layers;

void blend_factor(out float factor)
{
    float num = 0.0;
    for(int i=0; i<5; i++) if(enable_layers[i]) num += 1.0;
    factor = 1.0 / num;
}

void recombine(in vec4 col, out float val)
{
    val = col.b + (col.g / 100.0) + (col.r / 10000.0);
}

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

void compute_color(in vec4 sval, in vec4[NUM_LERP_POINTS] colors, out vec3 pmacol)
{
    float tval; recombine(sval, tval);
    multi_lerp(tval, colors, pmacol);
}

void main()
{
    vec2 pos = vec2(gl_FragCoord.x / screensize.x, gl_FragCoord.y / screensize.y);
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