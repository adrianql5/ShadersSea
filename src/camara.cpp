#include "camara.h"
#include "definiciones.h"


#define DEG_TO_RAD(x) ((x) * 0.01745329252f) // Convierto grados a radianes
//#define posicionCamara(x, y, z) vec3(0.0, 50, -0.9) // Defino la posici�n de la c�mar


using namespace glm;

float alpha = 90.0f; // Establezco el �ngulo horizontal inicial en la vista exterior
float beta = 1.0f;  // Establezco el �ngulo vertical inicial en la vista exterior

vec3 cameraPos;
vec3 vectorDirectorCamara; 

vec3 cameraRotationPoint = vec3(24.0f, 70.30f, 60.0f); // Defino el centro de rotaci�n de la camara

mat4 Mprojection;
mat4 Mview;
// Configuro una c�mara exterior que me permite ver toda la escena y rotarla alrededor del origen
void myCamaraExterior(int W_WIDTH, int W_HEIGHT) {
    // Defino la proyecci�n en perspectiva
    Mprojection = perspective(radians(45.0f), (float)W_WIDTH / (float)W_HEIGHT, 1.0f, 2000.0f);
    unsigned int projectionLoc = glGetUniformLocation(shadersProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(Mprojection));

    // Defino la vista usando una c�mara orbital
    Mview = lookAt(
        vec3(
            DISTANCIA * sin(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta)), // Calculo la posici�n en X
            DISTANCIA * sin(DEG_TO_RAD(beta)), // Calculo la posici�n en Y
            DISTANCIA * cos(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta))  // Calculo la posici�n en Z
        ),
        vec3(0.0f, 20.0f, 0.0f), // Apunto la c�mara al centro de la escena
        vec3(0.0f, 1.0f, 0.0f)  // Establezco la direcci�n "up"
    );

    unsigned int viewLoc = glGetUniformLocation(shadersProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(Mview));
}

void myCamaraCruz(int W_WIDTH, int W_HEIGHT) {
    // Proyecci�n ortogr�fica: coordenadas de -1 a 1 en ambas dimensiones
    Mprojection = ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    unsigned int projectionLoc = glGetUniformLocation(shadersProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(Mprojection));

    // Vista identidad: no aplicamos ninguna transformaci�n
    Mview = mat4(1.0f);
    unsigned int viewLoc = glGetUniformLocation(shadersProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(Mview));
}



// C�mara desde el faro

void myCamaraFaro(int W_WIDTH, int W_HEIGHT) {
   // Defino la proyecci�n en perspectiva  
   Mprojection = perspective(radians(45.0f), (float)W_WIDTH / (float)W_HEIGHT, 1.0f, 3000.0f);  
   unsigned int projectionLoc = glGetUniformLocation(shadersProgram, "projection");  
   glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(Mprojection));  
 
 
   vec3 posCentroFaro = vec3(-200.0f, 0.0f, 0.0f); // Centro de la circunferencia
   // Posici�n de la c�mara girando alrededor del faro  
   cameraPos = vec3(  
       POSICION_INICIAL_FARO.x +  sin(DEG_TO_RAD(alpha)),
       POSICION_INICIAL_FARO.y + ALTURA_CAMARA,
       POSICION_INICIAL_FARO.z +  cos(DEG_TO_RAD(alpha))
     );

   // Calcular la direcci�n de vista usando alpha y beta  
   vec3 direction;  
   direction.x = sin(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta));  
   direction.y = sin(DEG_TO_RAD(beta));  
   direction.z = cos(DEG_TO_RAD(alpha)) * cos(DEG_TO_RAD(beta));  
   vec3 apuntarCamara = cameraPos + normalize(direction);  
   vectorDirectorCamara = normalize(direction);  

   // Defino la vista apuntando al nuevo target  
   Mview = lookAt(  
       cameraPos,  
       apuntarCamara,  
       vec3(0.0f, 1.0f, 0.0f)  
   );  

   unsigned int viewLoc = glGetUniformLocation(shadersProgram, "view");  
   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(Mview));  
}


