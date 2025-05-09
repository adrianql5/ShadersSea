#include "camara.h"
#include "definiciones.h"

#define DEG_TO_RAD(x) ((x) * 0.01745329252f) // Convierto grados a radianes
//#define posicionCamara(x, y, z) vec3(0.0, 50, -0.9) // Defino la posición de la cámar


using namespace glm;

float alpha = 90.0f; // Establezco el ángulo horizontal inicial en la vista exterior
float beta = 1.0f;  // Establezco el ángulo vertical inicial en la vista exterior

vec3 cameraPos;
vec3 vectorDirectorCamara; 

vec3 cameraRotationPoint = vec3(24.0f, 70.30f, 60.0f); // Defino el centro de rotación de la camara


// Configuro una cámara exterior que me permite ver toda la escena y rotarla alrededor del origen
void myCamaraExterior(int W_WIDTH, int W_HEIGHT) {
    // Defino la proyección en perspectiva
    mat4 projection = perspective(radians(45.0f), (float)W_WIDTH / (float)W_HEIGHT, 1.0f, 2000.0f);
    unsigned int projectionLoc = glGetUniformLocation(shadersProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

    // Defino la vista usando una cámara orbital
    mat4 view = lookAt(
        vec3(
            DISTANCIA * sin(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta)), // Calculo la posición en X
            DISTANCIA * sin(DEG_TO_RAD(beta)), // Calculo la posición en Y
            DISTANCIA * cos(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta))  // Calculo la posición en Z
        ),
        vec3(0.0f, 20.0f, 0.0f), // Apunto la cámara al centro de la escena
        vec3(0.0f, 1.0f, 0.0f)  // Establezco la dirección "up"
    );

    unsigned int viewLoc = glGetUniformLocation(shadersProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}

void myCamaraCruz(int W_WIDTH, int W_HEIGHT) {
    // Proyección ortográfica: coordenadas de -1 a 1 en ambas dimensiones
    mat4 projection = ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    unsigned int projectionLoc = glGetUniformLocation(shadersProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

    // Vista identidad: no aplicamos ninguna transformación
    mat4 view = mat4(1.0f);
    unsigned int viewLoc = glGetUniformLocation(shadersProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}



// Cámara desde el faro

void myCamaraFaro(int W_WIDTH, int W_HEIGHT) {
   // Defino la proyección en perspectiva  
   mat4 projection = perspective(radians(45.0f), (float)W_WIDTH / (float)W_HEIGHT, 1.0f, 3000.0f);  
   unsigned int projectionLoc = glGetUniformLocation(shadersProgram, "projection");  
   glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));  
 
 
   vec3 posCentroFaro = vec3(-200.0f, 0.0f, 0.0f); // Centro de la circunferencia
   // Posición de la cámara girando alrededor del faro  
   cameraPos = vec3(  
       POSICION_INICIAL_FARO.x +  sin(DEG_TO_RAD(alpha)),
       POSICION_INICIAL_FARO.y + ALTURA_CAMARA,
       POSICION_INICIAL_FARO.z +  cos(DEG_TO_RAD(alpha))
     );

   // Calcular la dirección de vista usando alpha y beta  
   vec3 direction;  
   direction.x = sin(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta));  
   direction.y = sin(DEG_TO_RAD(beta));  
   direction.z = cos(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta));  
   vec3 apuntarCamara = cameraPos + normalize(direction);  
   vectorDirectorCamara = normalize(direction);  

   // Defino la vista apuntando al nuevo target  
   mat4 view = lookAt(  
       cameraPos,  
       apuntarCamara,  
       vec3(0.0f, 1.0f, 0.0f)  
   );  

   unsigned int viewLoc = glGetUniformLocation(shadersProgram, "view");  
   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));  
}


