#pragma warning(disable:4996)
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include <iostream>


float vertices_mar[] = {
    // Posición          // Color            // Normales    // Texturas (poniendo más de 1 hacemos que se repitan las texturas)
    0.5f, 0.0f, -0.5f,   0.0, 1.0, 0.0,   .0f, 1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,   0.0, 1.0, 0.0,   0.0f, 1.0f, 0.0f,  10.0f, 0.0f,
    0.5f, 0.0f,  0.5f,   0.0, 1.0, 0.0,   0.0f, 1.0f, 0.0f,  10.0f, 10.0f,

    0.5f, 0.0f,  0.5f,   0.0, 1.0, 0.0,   .0f, 1.0f, 0.0f,  10.0f, 10.0f,
    -0.5f, 0.0f, -0.5f,   0.0, 1.0, 0.0,   0.0f, 1.0f, 0.0f,  0.0f, 10.0f,
    -0.5f, 0.0f,  0.5f,   0.0, 1.0, 0.0,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
};
