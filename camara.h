#ifndef INPUT_H
#define INPUT_H


#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>




extern float alpha;
extern float beta;
extern float DISTANCIA;
extern glm::vec3 cameraPos;
extern glm::vec3 vectorDirectorCamara;


//Uso este .h para no tener 1000 lineas de codigo en un mismo archivo y mejorar la legibilidad

extern GLuint shadersProgram;

void myCamaraExterior(int W_WIDTH, int W_HEIGHT);
void myCamaraCruz(int W_WIDTH, int W_HEIGHT);
void myCamaraFaro(int W_WIDTH, int W_HEIGHT);




#endif
