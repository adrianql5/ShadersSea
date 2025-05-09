#version 330 core

in vec3 Color;
in vec3 FragPos;
in vec3 Normal;   
in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 objetoColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 luzDir;
uniform vec3 viewPos;
uniform int noche;


uniform sampler2D textura1;

void main()
{
    vec4 texturaColor = texture(textura1, TexCoord);
    // Normalizado
    vec3 norm = normalize(Normal);

    // Dirección hacia la luz
    vec3 lightDir = normalize(lightPos - FragPos);

    // Componente ambiental
    vec3 ambient = lightColor;

    vec3 ld = normalize(-lightPos);
    ld = luzDir;
    vec3 fd = normalize(FragPos - lightPos);

    if (acos(dot(fd, ld)) < radians(12.0)) {
        // Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor *5;

        // Specular
        float specularStrength = 1.0;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
        vec3 specular = specularStrength * spec * lightColor;

        // Combinación final
        vec3 result = (ambient + diffuse + specular) * (Color.rgb + texturaColor.rgb);
        FragColor = vec4(result, 1.0);
    } else {
        // MOdo luz total
        vec3 result = ambient * (Color.rgb + texturaColor.rgb);
        FragColor = vec4(result, 1.0);
    }
}
