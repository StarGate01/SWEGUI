uniform sampler2D bath_tex;
// uniform vec2 circlepos;
uniform vec2 screensize;

void main()
{
    // vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    // float xdif = abs(circlepos.x - gl_FragCoord.x);
    // float ydif = abs(screensize.y - circlepos.y - gl_FragCoord.y);
    // float dist = (xdif * xdif + ydif * ydif) / 20000.0;
    // float scaler = gl_FragCoord.x * gl_FragCoord.y / 15000.f;
    // gl_FragColor = vec4((sin(dist * scaler) / 2.0) + 0.5, (sin((1.0 - dist) * scaler) / 2.0) + 0.5, 0.0, 1.0);
    vec4 pixel = texture2D(bath_tex, gl_TexCoord[0].xy);
    gl_FragColor = pixel;
}