#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

uniform float time;
uniform vec2 resolution;

out vec4 vertColor;
out vec2 vertUV;

void main()
{
    gl_Position = vec4(aPos * 2.0 / resolution, 0.0, 1.0);
    vertColor   = aColor;
    vertUV      = aUV;
}