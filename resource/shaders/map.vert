#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

// Pass output to fragment shader
out vec3 color;

// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Outputs the normal for the fragment shader
out vec3 Normal;

// Outputs the current position for the Fragment Shader
out vec3 crntPos;

// Input matrices needed for 3D viewing and perspective
uniform mat4 camMatrix;

uniform mat4 model;

void main()
{
    crntPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(crntPos, 1.0);
    //color = aColor;
    texCoord = aTex;
    Normal = aNormal;
}