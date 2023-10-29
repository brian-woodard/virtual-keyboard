#version 330 core
#
layout (location = 0) in vec3 aPos;

uniform mat4 MVP;

flat out vec3 startPos;
out vec3 vertPos;

void main()
{
   vec4 pos    = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   gl_Position = pos; 
   vertPos     = pos.xyz / pos.w;
   startPos    = vertPos;
}
