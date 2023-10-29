#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D textureUnit;
uniform float Alpha;

void main()
{
    FragColor = vec4(texture(textureUnit, TexCoord).xyz, Alpha);
}
