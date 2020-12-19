#version 330 core

uniform sampler2D NoiseTexture;
uniform sampler2D Texture_1;

in vec2 exTexcoord;
out vec4 FragmentColor;
const vec3 colour1 = vec3(0.5, 0.25, 0.3);
const vec3 colour2 = vec3(0.85, 0.7, 0.75);

void main(void)
{
	vec4 tex = texture(Texture_1, exTexcoord);
	float noise = texture(NoiseTexture, exTexcoord).r*0.5+0.5;
	float intensity = clamp(noise, 0.0, 1.0);
	vec3 color = mix(colour1, colour2, intensity*2);
	FragmentColor = mix(tex, vec4(color,1.0), 0.1);

}
