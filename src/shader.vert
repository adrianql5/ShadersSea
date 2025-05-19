#version 330 core 

// Atributos de entrada del vértice
layout(location = 0) in vec3 aPos;        // Posición del vértice (x, y, z)
layout(location = 1) in vec3 aColor;     // Color vértice (para iluminación)
layout(location = 2) in vec3 aNormal;   // Normal del vértice (x, y, z)
layout(location = 3) in vec2 aTextCoord;  // Coordenadas de textura (u, v)

// Uniforms: matrices para transformar los vértices
uniform mat4 model;       // Transforma del espacio local al mundo
uniform mat4 view;        // Transforma del mundo a la vista (cámara)
uniform mat4 projection;  // Transforma de vista a espacio de proyección (pantalla)

// Variables de salida para el fragment shader
out vec3 FragPos;     // Posición del fragmento en el espacio mundial
out vec3 Color;
out vec3 Normal;      // Normal transformada al espacio mundial
out vec2 TextCoord;   // Coordenadas de textura para el fragment shader

void main() {
    // Transformamos la posición del vértice al espacio mundial
    vec4 worldPosition = model * vec4(aPos, 1.0);

    // Guardamos la posición en espacio mundial para usarla en iluminación
    FragPos = vec3(worldPosition);

    // Calculamos la normal transformada correctamente al espacio mundial
    // Usamos la matriz inversa transpuesta para evitar distorsiones si hay escalado
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Pasamos las coordenadas de textura directamente
    TextCoord = aTextCoord;
    Color = aColor;

    // Calculamos la posición final del vértice en espacio de clip
    // (proyección * vista * modelo * posición)
    gl_Position = projection * view * worldPosition;
}
