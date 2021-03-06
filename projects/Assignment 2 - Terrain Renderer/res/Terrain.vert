#version 410

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 color;
out vec2 texUV;

uniform mat4 MVP;
uniform sampler2D heightmapSampler;

void main() {
	color = vertex_color;
	texUV = vertex_uv;

	vec3 vert = vertex_pos;
	vert.y = texture(heightmapSampler, vertex_uv).r;

	gl_Position = MVP * vec4(vert, 1.0);
}