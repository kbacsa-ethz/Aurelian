#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

// Pass output to fragment shader
out vec3 color;

// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Define uniform (modifiable at runtime)
uniform float scale;

// Input matrices needed for 3D viewing and perspective
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   color = aColor;
   texCoord = aTex;
}
