// Copyright (c) 2025 Adrián Quiroga Linares Lectura y referencia permitidas; reutilización y plagio prohibidos

#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad.h>

#include <glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

// Variables globales del skybox
extern GLuint skyboxVAO;
extern GLuint skyboxTextureNight;
extern GLuint skyboxTextureDay;
extern GLuint skyboxShader;

/**
 * @brief Inicializa los recursos necesarios para renderizar el skybox.
 */
void init_skybox();

/**
 * @brief Carga las texturas de las caras del cubemap para el skybox.
 * @param faces Vector con rutas de las 6 imágenes del cubemap (orden: +X, -X, +Y, -Y, +Z, -Z)
 */
void load_skybox_textures(const std::vector<std::string>& faces, GLuint& skyboxTexture);

/**
 * @brief Dibuja el skybox usando las matrices de vista y proyección proporcionadas.
 * @param view Matriz de vista (sin traslación interna)
 * @param projection Matriz de proyección
 */
void draw_skybox(const glm::mat4& view, const glm::mat4& projection, GLuint& skyboxTexture);

#endif // SKYBOX_H


