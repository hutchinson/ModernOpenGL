#version 330 core
out vec4 color;

uniform vec4 ourColor;
in vec4 vertexColor; // this is linked to the output from above..

void main()
{
    color = vertexColor;
}
