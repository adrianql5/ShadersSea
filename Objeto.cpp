#include "Objeto.h"
#include "esfera.h"
#include <cassert>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h" //esta en los includes

// Declaración de los objetos

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Función para cargar una textura desde un archivo de imagen
GLuint myCargaTexturas(const char* nombre) {
    GLuint textura;

    // Genera un identificador para la textura
    glGenTextures(1, &textura);

    // Activa (vincula) la textura como una textura 2D
    glBindTexture(GL_TEXTURE_2D, textura);

    // Configura el modo de envoltura (wrapping) en S y T (horizontal y vertical) a repetición
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Establece el tipo de filtrado para minimizar y magnificar la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Suavizado al reducir tamaño
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Suavizado al aumentar tamaño

    // Indica que la imagen se debe voltear verticalmente al cargar
    stbi_set_flip_vertically_on_load(1);

    // Variables para almacenar las dimensiones y canales de color de la imagen
    int width, height, nrChannels;

    // Carga la imagen usando stb_image
    unsigned char* data = stbi_load(nombre, &width, &height, &nrChannels, 0);

    // Si se cargó correctamente la imagen
    if (data) {
        GLenum format;

        // Determina el formato de la textura según el número de canales
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        else format = GL_RGB; // Por defecto

        // Crea la textura en memoria de video
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Genera los mipmaps automáticamente (niveles de detalle)
       // glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        // Si falla la carga, imprime un mensaje de error
        printf("Error cargando textura: %s\n", nombre);
        printf("stbi_failure_reason: %s\n", stbi_failure_reason());
    }

    // Libera la memoria de la imagen cargada
    stbi_image_free(data);

    // Devuelve el ID de la textura generada
    return textura;
}

// FUnción para calcular la dirección base de un archivo
// Es necesaria para cuando necesitamos el .mtl de un .obj
std::string
getBaseDir(const std::string &path)
{
        size_t slash = path.find_last_of("/\\");
        if (slash != std::string::npos) {
                return path.substr(0, slash + 1); // incluye la última barra
        }
        return "";
}

extern GLuint setShaders(const char *nomb_vert, const char *nomb_frag);

void
crearObjeto(const char *archivo, Objeto &objeto, const char *shader)
{
        int vertexCount = 0;
        tinyobj::attrib_t atributos;
        std::vector<tinyobj::shape_t> formas;
        std::vector<tinyobj::material_t> materiales;
        std::string warning, error;

        std::string inputfile = archivo;
        std::string base_dir = getBaseDir(inputfile);

        bool ret = tinyobj::LoadObj(&atributos, &formas, &materiales, &warning,
                                    &error, archivo, base_dir.c_str(), true);
        if (!ret) {
                std::cerr << "Error cargando OBJ: " << error << std::endl;
                return;
        }

        std::vector<float> vertexData;

        for (const auto &shape : formas) {
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
                        int fv = shape.mesh.num_face_vertices[f];
                        int mat_id = shape.mesh.material_ids[f];

                        // Valores de color por defecto (si no hay material)
                        float r = 1.0f, g = 1.0f, b = 1.0f;

                        if (mat_id >= 0 && mat_id < materiales.size()) {
                                r = materiales[mat_id].diffuse[0];
                                g = materiales[mat_id].diffuse[1];
                                b = materiales[mat_id].diffuse[2];
                        }

                        // Triangulación manual: triangula cualquier cara con más de 3 vértices
                        for (int v = 1; v < fv - 1; ++v) {
                                // ÍNDICES de los 3 vértices del triángulo
                                int idx0 = index_offset;
                                int idx1 = index_offset + v;
                                int idx2 = index_offset + v + 1;

                                const tinyobj::index_t indices[3] = { shape.mesh.indices[idx0],
                                                                      shape.mesh.indices[idx1],
                                                                      shape.mesh.indices[idx2] };

                                for (int i = 0; i < 3; ++i) {
                                        const tinyobj::index_t &idx = indices[i];

                                        // Vértices
                                        float vx = atributos.vertices[3 * idx.vertex_index + 0];
                                        float vy = atributos.vertices[3 * idx.vertex_index + 1];
                                        float vz = atributos.vertices[3 * idx.vertex_index + 2];

                                        // Normales (si existen)
                                        float nx = 0.0f, ny = 0.0f, nz = 0.0f;
                                        if (idx.normal_index >= 0) {
                                                nx = atributos.normals[3 * idx.normal_index + 0];
                                                ny = atributos.normals[3 * idx.normal_index + 1];
                                                nz = atributos.normals[3 * idx.normal_index + 2];
                                        }

                                        // Coordenadas de textura (si existen)
                                        float tx = 0.0f, ty = 0.0f;
                                        if (idx.texcoord_index >= 0) {
                                                // printf("Coordenadas de textura: %d\n", idx.texcoord_index);
                                                tx = atributos.texcoords[2 * idx.texcoord_index + 0];
                                                ty = atributos.texcoords[2 * idx.texcoord_index + 1];
                                        }

                                        // Añadir vértices, colores, coordenadas de textura y normales
                                        vertexData.push_back(vx);
                                        vertexData.push_back(vy);
                                        vertexData.push_back(vz);
                                        vertexData.push_back(r);
                                        vertexData.push_back(g);
                                        vertexData.push_back(b);
                                        vertexData.push_back(nx); // Normal X
                                        vertexData.push_back(ny); // Normal Y
                                        vertexData.push_back(nz); // Normal Z
                                        vertexData.push_back(tx); // Coordenada de textura X
                                        vertexData.push_back(ty); // Coordenada de textura Y
                                }
                        }

                        index_offset += fv;
                }
        }

        vertexCount =
        vertexData.size() /11; // 3 posiciones + 3 colores  + 3 normales + 2 coordenadas de textura
        //printf("Vértices: %d\n", vertexCount);

        unsigned int VBO;
        glGenVertexArrays(1, &objeto.VAO);
        glBindVertexArray(objeto.VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
                     vertexData.data(), GL_STATIC_DRAW);

        // Atributos de vértice
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) 0);
        glEnableVertexAttribArray(0);

        // Atributos de color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Atributos de normales
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Atributos de coordenadas de textura
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteBuffers(1, &VBO);

        if (shader != nullptr) {
                objeto.textura = myCargaTexturas(shader);
        }
}

void
dibujaMar()
{
        unsigned int VBO;
        // Definición de los vértices de un cuadrado en el plano XZ
        float vertices[] = {
       	-0.5f, 0.0f,  0.5f, 0.0, 0.188, 0.306,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // vértice 1
	 0.5f, 0.0f,  0.5f, 0.0, 0.188, 0.306,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, // vértice 2
	 0.5f, 0.0f, -0.5f, 0.0, 0.188, 0.306  ,0.0f, 0.0f, 0.0f,  1.0f, 0.0f, // vértice 3

	-0.5f, 0.0f,  0.5f,  0.0, 0.188, 0.306, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // vértice 4
	 0.5f, 0.0f, -0.5f, 0.0, 0.188, 0.306,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, // vértice 5
	-0.5f, 0.0f, -0.5f,0.0, 0.188, 0.306 , 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // vértice 6
};
 
        glGenVertexArrays(1, &VAOMar); // Crear un Vertex Array Object
        glGenBuffers(1, &VBO); // Crear un Vertex Buffer Object

        // bind the Vertex Array Object first.
        glBindVertexArray(VAOMar); // Quiere decir que vamos a trabajar con este VAO a continuación

        glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO); // Quiere decir que vamos a trabajar con este VBO a continuación
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW); // Introducir los vértices en el VBO

        // Configurar el atributo de posición (ubicado en la posición 0 del VAO)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) 0);
        glEnableVertexAttribArray(0); // Habilitar el atributo de posición

        // Configurar el atributo de color (ubicado en la posición 1 del VAO)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Configurar el atributo de normales (ubicado en la posición 2 del VAO)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // position tEXTURAS (ubicado en la posición 3 del VAO)
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Unbind VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Eliminar los buffers
        glDeleteBuffers(1, &VBO);
}

void
crearObjetoVertices(unsigned int &VAO, const float vertices[])
{
        unsigned int VBO;

        glGenVertexArrays(1, &VAO); // Crear un Vertex Array Object
        glGenBuffers(1, &VBO); // Crear un Vertex Buffer Object

        // bind the Vertex Array Object first.
        glBindVertexArray(
        VAO); // Quiere decir que vamos a trabajar con este VAO a continuación

        glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO); // Quiere decir que vamos a trabajar con este VBO a continuación
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW); // Introducir los vértices en el VBO

        // Configurar el atributo de posición (ubicado en la posición 0 del VAO)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) 0);
        glEnableVertexAttribArray(0); // Habilitar el atributo de posición

        // Configurar el atributo de color (ubicado en la posición 1 del VAO)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Configurar el atributo de normales (ubicado en la posición 2 del VAO)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Configurar el atributo de texturas (ubicado en la posición 3 del VAO)
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),
                              (void *) (9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Unbind VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Eliminar los buffers
        glDeleteBuffers(1, &VBO);
}

void
dibujaEsfera(unsigned int &VAO)
{
 unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

    // Posiciones (atributo 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);

    // Normales (atributo 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

	// Texturas (atributo 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
       
}

void
dibujaCruz(unsigned int &VAO)
{
        unsigned int VBO;

        // Coordenadas para dibujar la cruz (dos líneas: una horizontal y una
        // vertical)
        float vertices_cruz[] = {
                // Línea horizontal (de izquierda a derecha)
                -0.05f, 0.0f, 1.0f, 0.0f,
                0.0f, // Vertice 1: posición (x, y), color (r, g, b)
                0.05f, 0.0f, 1.0f, 0.0f, 0.0f, // Vertice 2

                // Línea vertical (de abajo hacia arriba)
                0.0f, -0.05f, 1.0f, 0.0f, 0.0f, // Vertice 3
                0.0f, 0.05f, 1.0f, 0.0f, 0.0f // Vertice 4
        };

        // Crear buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Vincular buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Introducir los vértices en el VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cruz), vertices_cruz,
                     GL_STATIC_DRAW);

        // Definir el formato de los atributos del vértice:
        // Posición (x, y)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // Color (r, g, b)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Desvincular VBO y VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Eliminar VBO
        glDeleteBuffers(1, &VBO);
}
