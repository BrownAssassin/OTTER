#version 410

in vec3 color;
in vec2 texUV;

out vec4 frag_color;

uniform sampler2D waterSampler;

void main() {
	frag_color = texture(waterSampler, texUV) * vec4(1.0, 1.0, 1.0, 0.75);
}