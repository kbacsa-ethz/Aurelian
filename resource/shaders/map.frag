#version 330 core
out vec4 FragColor;

// Texture coordinates of vertex shader
in vec2 texCoord;

// Imports normals
in vec3 Normal;

// Imports current position
in vec3 crntPos;

// Texture unit from main function
uniform sampler2D tex0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
    float ambient = 0.0f;

    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 0.3;
    float b = 0.05;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    FragColor = texture(tex0, texCoord) * lightColor * (diffuse * inten + ambient + specularLight * inten);
}