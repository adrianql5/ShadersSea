// Copyright (c) 2025 Adrián Quiroga Linares Lectura y referencia permitidas; reutilización y plagio prohibidos

﻿#ifndef OBJETO_H
#define OBJETO_H

#include <iostream>
#include <stdio.h>
#include <glad.h>

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <stdlib.h>

// Variables globales para los VAOs
extern unsigned int VAOBarco;
extern unsigned int VAOMar;

typedef struct {
   glm::vec3 pos;          // Posición en 3D
   float velocidad;        // Velocidad del objeto
   glm::vec3 escalado;     // Escalado en 3D
   glm::vec3 color;        // Color RGB
   GLuint textura;
   GLuint VAO;       // Lista de render
   int numVertices;
   float radioColision;
   glm::vec3 direccion;
   glm::vec3 hitbox; // Tamaño del hitbox
   float rotacion;
} Objeto;

void crearObjeto(const char* archivo, Objeto& objeto, const char* shader = nullptr);
void crearObjetoVertices(unsigned int& VAO, const float* vertices);
void dibujaMar();  // Función temporal para el mar ahora mismo
bool colision(Objeto objeto1, Objeto objeto2);
void dibujaEsfera(unsigned int& VAO);
void dibujaCruz(unsigned int& VAO);
GLuint myCargaTexturas(const char *nome);
#endif
