#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / resolution.xy;

    // Generate a simple starfield
    float stars = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
    float intensity = smoothstep(0.99, 1.0, stars);

    // Add a subtle cosmic background
    vec3 cosmicColor = vec3(0.05, 0.05, 0.2);
    vec3 backgroundColor = mix(cosmicColor, vec3(0.0), pow(1.0 - uv.y, 4.0));

    // Combine the starfield and cosmic background
    vec3 finalColor = mix(backgroundColor, vec3(1.0), intensity);

    // Output the final color
    fragColor = vec4(finalColor, 1.0);
}

void main()
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}