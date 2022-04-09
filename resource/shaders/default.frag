#version 330 core
out vec4 FragColor;

// Color of vertex shader
in vec3 color;

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

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;


    FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specularLight);
}
