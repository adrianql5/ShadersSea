#include "setShaders.h"
#include "stb_image.h"
#include <glad.h>

#include <glfw3.h>
#include <vector>
#include <string>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// --- Shaders
static const char vertex_shader_cruceta[] = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

static const char fragment_shader_cruceta[] = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform int texture_count;
uniform sampler2D textures[16];
uniform vec3 color;

void main()
{
    if (texture_count == 0) {
        FragColor = vec4(color, 1.0);
    } else {
        FragColor = texture(textures[0], TexCoord);
        if (FragColor.a < 0.1) {
            discard;
        }
    }
}
)";

// --- Globals
GLuint crucetaVAO = 0;
GLuint crucetaTexture = 0;
GLuint crucetaShader = 0;

// --- Initialization
void init_cruceta()
{   
    crucetaShader = setShaders_str(vertex_shader_cruceta, fragment_shader_cruceta);

    // Load texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("./resources/textures/cruceta.png", &width, &height, &nrChannels, 0);
    if (!data) {
        printf("Failed to load cruceta.png\n");
        return;
    }

    glGenTextures(1, &crucetaTexture);
    glBindTexture(GL_TEXTURE_2D, crucetaTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Geometry: position (vec3) + texCoord (vec2)
    float vertices[] = {
        // positions        // texCoords
        -0.1f, -0.1f, 0.0f,  0.0f, 0.0f,
         0.1f, -0.1f, 0.0f,  1.0f, 0.0f,
         0.1f,  0.1f, 0.0f,  1.0f, 1.0f,

        -0.1f, -0.1f, 0.0f,  0.0f, 0.0f,
         0.1f,  0.1f, 0.0f,  1.0f, 1.0f,
        -0.1f,  0.1f, 0.0f,  0.0f, 1.0f
    };

    GLuint VBO;
    glGenVertexArrays(1, &crucetaVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(crucetaVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // aTexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    //printf("d\n",crucetaTexture);
}

// --- Draw
void draw_cruceta()
{
    glUseProgram(crucetaShader);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crucetaTexture);
    glUniform1i(glGetUniformLocation(crucetaShader, "textures[0]"), 0);

    // Set uniforms
    glUniform1i(glGetUniformLocation(crucetaShader, "texture_count"), 1);
    glUniform3f(glGetUniformLocation(crucetaShader, "color"), 1.0f, 1.0f, 1.0f); // fallback color if needed

    glBindVertexArray(crucetaVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

