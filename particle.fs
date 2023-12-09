#version 420 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform float lifetime;

void main() {
    float level = smoothstep(0, 5, lifetime);
    vec4 texColor = texture(sprite, TexCoords);
    texColor.a = texColor.a * level;
    if(texColor.a < 0.05)
        discard;
    vec4 color1 = vec4(0.7, 0.86, 1.0, 1.0);
    vec4 color2 = vec4(0.0, 0.45, 1.0, 1.0);
    color = texColor * mix(color1, color2, level);
}