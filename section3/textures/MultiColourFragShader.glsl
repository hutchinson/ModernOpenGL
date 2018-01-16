#version 330 core
out vec4 color;

uniform vec4 ourColor;

uniform float mixLevel;

in vec4 vertexColor; // this is linked to the output from above..
in vec2 texCoords;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    vec2 flippedTexCoords = texCoords;
    // Flip the image.
    //flippedTexCoords.x *= -1.0f;

    // Cause wrapping.
    // flippedTexCoords *= 2.0f;

    color = mix(texture(ourTexture, texCoords), texture(ourTexture2, flippedTexCoords), mixLevel);
    //color = texture(ourTexture2, texCoords) * vec4(vertexColor);
}
