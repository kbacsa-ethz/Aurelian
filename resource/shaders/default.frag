#version 330 core
out vec4 FragColor;

// Color of vertex shader
in vec3 color;

// Texture coordinates of vertex shader
in vec2 texCoord;

// Texture unit from main function
uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, texCoord);
}
