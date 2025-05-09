#version 330 core 

// Atributos de entrada del v�rtice
layout(location = 0) in vec3 aPos;        // Posici�n del v�rtice (x, y, z)
layout(location = 1) in vec3 aColor;     // Color v�rtice (para iluminaci�n)
layout(location = 2) in vec3 aNormal;   // Normal del v�rtice (x, y, z)
layout(location = 3) in vec2 aTextCoord;  // Coordenadas de textura (u, v)

// Uniforms: matrices para transformar los v�rtices
uniform mat4 model;       // Transforma del espacio local al mundo
uniform mat4 view;        // Transforma del mundo a la vista (c�mara)
uniform mat4 projection;  // Transforma de vista a espacio de proyecci�n (pantalla)

// Variables de salida para el fragment shader
out vec3 FragPos;     // Posici�n del fragmento en el espacio mundial
out vec3 Color;
out vec3 Normal;      // Normal transformada al espacio mundial
out vec2 TextCoord;   // Coordenadas de textura para el fragment shader

void main() {
    // Transformamos la posici�n del v�rtice al espacio mundial
    vec4 worldPosition = model * vec4(aPos, 1.0);

    // Guardamos la posici�n en espacio mundial para usarla en iluminaci�n
    FragPos = vec3(worldPosition);

    // Calculamos la normal transformada correctamente al espacio mundial
    // Usamos la matriz inversa transpuesta para evitar distorsiones si hay escalado
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Pasamos las coordenadas de textura directamente
    TextCoord = aTextCoord;
    Color = aColor;

    // Calculamos la posici�n final del v�rtice en espacio de clip
    // (proyecci�n * vista * modelo * posici�n)
    gl_Position = projection * view * worldPosition;
}
