#version 330 core
out vec4 FragColor;

// Color of vertex shader
//in vec3 color;

// Texture coordinates of vertex shader
in vec2 texCoord;

// Imports normals
in vec3 Normal;

// Imports current position
in vec3 crntPos;

// Texture unit from main function
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform vec2 texSizeVal;

void main()
{
    float ambient = 0.05f;

    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 0.3;
    float b = 0.05;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.05f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    vec2 texCoordLocal = crntPos.xz / texSizeVal;
//    texCoordLocal[0] = mod(crntPos.x, texModuloVal[0]);
//    texCoordLocal[1] = mod(crntPos.z, texModuloVal[1]);


    FragColor = (texture(tex0, texCoordLocal) * (diffuse + ambient) + texture(tex1, texCoordLocal).r * specular) * lightColor;
//    FragColor = (texture(tex0, texCoord) * (diffuse + ambient)) * lightColor;
}