#version 410 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 camera;

void main()
{
    gl_Position = projection * camera * model * vec4(position.x, position.y, position.z, 1.0);
}