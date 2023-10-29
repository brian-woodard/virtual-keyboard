#version 330

out vec4 fragColor;


flat in vec3 startPos;
in vec3 vertPos;

uniform vec2 resolution;
uniform vec4 color;
uniform float dashSize;
uniform float gapSize;

void main()
{
    vec2  dir  = (vertPos.xy-startPos.xy) * resolution/2.0;
    float dist = length(dir);

    if (fract(dist / (dashSize + gapSize)) > dashSize/(dashSize + gapSize))
        discard; 


    fragColor = color;
}
