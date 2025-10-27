// Copyright (c) 2025 Adrián Quiroga Linares Lectura y referencia permitidas; reutilización y plagio prohibidos

#ifndef PROYECTILES_H
#define PROYECTILES_H

#include <iostream>
#include <stdio.h>

#include <glad.h>

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <stdlib.h>

#define tamanoProyectil 0.5f



glm::mat4 dibujarObjeto(const Objeto& objeto, unsigned int transformLoc, unsigned int colorLoc, glm::mat4 model);
void actualizarBarcos(std::vector<Objeto>& barcosActivos, unsigned int transforLoc, unsigned int colorLoc);
void actualizarProyectiles(std::vector<Objeto>& proyectilesActivos, std::vector<Objeto>& barcosActivos, unsigned int transforLoc, unsigned int colorLoc);
void lanzarProyectil(unsigned int VAO, glm::vec3 origen, glm::vec3 vectorDirectorCamara, std::vector<Objeto> &barcosActivos, std::vector<Objeto> &proyectilesActivos, float velocidad, unsigned int textura);
void generarBarcoActivo(unsigned int VAO, glm::vec3 origen, glm::vec3 vectorDirectorBarco, std::vector<Objeto>& barcosActivos, float velocidad);
void generarBarcosAleatorios(int cantidad, Objeto barco, std::vector<Objeto>& barcosActivos);
#endif
