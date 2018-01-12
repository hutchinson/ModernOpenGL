#version 330 core
out vec4 color;

uniform vec4 ourColor;
in vec4 vertexColor; // this is linked to the output from above..
in vec2 texCoords;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    color = mix(texture(ourTexture, texCoords), texture(ourTexture2, texCoords), 0.2);
    //color = texture(ourTexture2, texCoords) * vec4(vertexColor);
}
