#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 MVP;

out vec2 TexCoord;

void main()
{
    gl_Position = MVP * vec4(aPos.xyz, 1.0f);
    TexCoord = aTexCoord;
}
