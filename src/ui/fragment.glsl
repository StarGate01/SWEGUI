uniform sampler2D bath_tex;
// uniform vec2 circlepos;
uniform vec2 screensize;

void main()
{
    // vec4 pixel = texture2D(bath_tex, gl_TexCoord[0].xy);
    // float xdif = abs(circlepos.x - gl_FragCoord.x);
    // float ydif = abs(screensize.y - circlepos.y - gl_FragCoord.y);
    // float dist = (xdif * xdif + ydif * ydif) / 20000.0;
    // float scaler = gl_FragCoord.x * gl_FragCoord.y / 15000.0;
    // gl_FragColor = vec4((sin(dist * scaler) / 2.0) + 0.5, (sin((1.0 - dist) * scaler) / 2.0) + 0.5, 0.0, 1.0);
    vec4 pixel = texture2D(bath_tex, vec2(gl_FragCoord.x / screensize.x, gl_FragCoord.y / screensize.y));
    float height = pixel.b + (pixel.g / 100.0) + (pixel.r / 10000.0);
    gl_FragColor = vec4(height, height, height, 1.0);
}