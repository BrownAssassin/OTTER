#version 410

in vec3 color;
in vec2 texUV;

out vec4 frag_color;

uniform sampler2D heightmapSampler;
uniform sampler2D sandSampler;
uniform sampler2D grassSampler;
uniform sampler2D stoneSampler;
uniform sampler2D snowSampler;

void main() {
	frag_color = mix(mix(mix(texture(sandSampler, texUV), texture(grassSampler, texUV), 
				smoothstep(0.25, 0.35, texture(heightmapSampler, texUV).r)), texture(stoneSampler, texUV), 
				smoothstep(0.5, 0.6, texture(heightmapSampler, texUV).r)), texture(snowSampler, texUV), 
				smoothstep(0.7, 0.8, texture(heightmapSampler, texUV).r));
}