#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// NOTE: Add here your custom variables

const vec2 size = vec2(800, 480);   // render size
const float samples = 4.0;          // pixels per axis; higher = bigger glow, worse performance
const float quality = 2.0;             // lower = smaller glow, better quality

float offset = 0.0;
float frequency = 450.0/3.0;

uniform float time;

void main()
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/size*quality;

    // Texel color fetching from texture sampler
    vec4 source = texture2D(texture0, fragTexCoord);

    const int range = 2;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture2D(texture0, fragTexCoord + vec2(x, y)*sizeFactor);
        }
    }
    
    float globalPos = (fragTexCoord.y + offset) * frequency;
    float wavePos = cos((fract(globalPos) - 0.5)*3.14);

    vec4 color = texture2D(texture0, fragTexCoord);

    // Calculate final fragment color
    gl_FragColor = ((sum/(samples*samples)) + source)*colDiffuse;
}