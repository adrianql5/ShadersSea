// Copyright (c) 2025 AdriÃ¡n Quiroga Linares Lectura y referencia permitidas; reutilizaciÃ³n y plagio prohibidos



    #include <windows.h>
    #include <mmsystem.h>  // Necesario para PlaySound
    #pragma comment(lib, "winmm.lib")  // Librería de Windows Multimedia 
    #include <iostream>
    #include <stdio.h>

    #include <glad.h>
    #include <thread>
    #include <chrono>




    #include <glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <unordered_map>

    #include "lecturaShader_0_9.h"
    #include "Objeto.h"
    #include "camara.h"


    #include "dibujo.h"
    #include "definiciones.h"

    #include "skybox.h"
    #include "cruceta.h"


    #define RGB_TO_FLOAT(x) ((x) / 256.0f) // convierto RGB a float
    #define DEG_TO_RAD(x) ((x) * 0.01745329252f) // convierto grados a radianes

    std::unordered_map<int, bool> teclas;

    using namespace glm;

    int W_WIDTH = 1200;
    int W_HEIGHT = 800;
    float anguloFoco;

    Objeto terreno = { {0, 0, 0}, 0, {2000, 0, 2000}, {0, 0.1875, 0.3046}, 0, 0,6,0};
    Objeto barco = { {200, 0.6, 0.0}, 0.25, {0.2, 0.2, 0.9}, {1, 0, 0}, 0,0,29310,10,{0,0,0},{0,0,0},0.0f};
    Objeto barco2 = { {200, 0.6, 0.0}, 0.25, {.000001, .00001, .00001}, {1, 0, 0}, 0,0,29310,10,{0,0,0},{0,0,0},45.0f };
    Objeto faro = { {0, 0, 0}, 0, {4.5, 4.5, 4.5}, {0, 0, 0}, 0,0,3723,0 };
    Objeto proyectil1 = { {2, 10, 0}, 0.9, {1.2, 1.2, 1.2},{1, 0, 0}, 0,0,1080,0 ,{20,20,20} };
    Objeto cruz = { {0, 0, 0}, 0, {1, 1, 1}, {1, 0, 0}, 0,0,0,0 };

    std::vector<Objeto> barcosActivos; // Array para almacenar los barcos activos
	std::vector<Objeto> proyectilesActivos; // Array para almacenar los proyectiles activos
    unsigned int VAOBarco;
    unsigned int VAOMar;

    extern mat4 Mview, Mprojection;
    // Variables para el control del tiempo
    double lastTime = glfwGetTime();
    double currentTime;
    double lapsoTime;
    int nbFrames = 0;
    double tiempoBarcos = 0;

	int tipoCamara = 0; // 1 = exterior, 0 = faro

    // Variables de cámara


    mat4 view;
    mat4 projection;

    // Carga de shaders
    extern GLuint setShaders(const char* nVertx, const char* nFrag);
    GLuint shadersProgram;
    GLuint shadersBarco;

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);
    void tiempo();

    int i = 0;
	int nivelActual = 1; // Variable para el nivel actual
    int noche = -1;
    float anguloDifusa = LUZ_DIFUSA_ANGULO;


    void iluminacion() {

        //El color de la luz ambiente
        unsigned int lightLoc = glGetUniformLocation(shadersProgram, "lightColor");
        if (noche == -1) {
            glUniform3f(lightLoc, 1.0f, 1.0f, 1.0f);
        }
        else {
            glUniform3f(lightLoc, .5f, 0.5f, 0.5f);
            unsigned int NocheLoc = glGetUniformLocation(shadersProgram, "noche");
            glUniform1i(NocheLoc, noche);
            //luz
            unsigned int lightPosLoc = glGetUniformLocation(shadersProgram, "lightPos");
            glUniform3f(lightPosLoc, cameraPos.x, cameraPos.y-1, cameraPos.z);
            // La luz está situada debajo del jugador

            //luz
            unsigned int lightDirLoc = glGetUniformLocation(shadersProgram, "luzDir");
            glUniform3f(lightDirLoc, sin(anguloDifusa *GR), 0, cos(anguloDifusa * GR));

            //Luz Especular
            //la posicion del usuario/camara (0,0,10) en nuestro caso
            unsigned int viewPosLoc = glGetUniformLocation(shadersProgram, "viewPos");
            glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
            //glUniform3f(viewPosLoc, 0.0f, 10.0f, 0.0f);
            anguloDifusa += 0.8;
        }



    }


       // ----------------------


    void display() {
            // Actualiza el tiempo de animaciones o lógica
        tiempo();

        glUseProgram(shadersProgram);
        unsigned int transformLoc = glGetUniformLocation(shadersProgram, "model");
        unsigned int colorLoc = glGetUniformLocation(shadersProgram, "objectoColor");


		switch (tipoCamara) {
		case 0: myCamaraFaro(W_WIDTH, W_HEIGHT); break;
		// case 1: myCamaraExterior(W_WIDTH, W_HEIGHT); break;
		default: myCamaraExterior(W_WIDTH, W_HEIGHT); break;
		}

        if (noche == 1) {
            draw_skybox(Mview,Mprojection, skyboxTextureNight);
        }
        else {
            draw_skybox(Mview, Mprojection, skyboxTextureDay);
        }



        if(!tipoCamara) draw_cruceta();
        // Obtiene la ubicación de las variables uniformes en el shader

        glUseProgram(shadersProgram);
        // Dibujar terreno
        glm::mat4 model = glm::mat4(1.0f);
        dibujarObjeto(terreno, transformLoc, colorLoc, model);

        //Dibujar faro
        dibujarObjeto(faro, transformLoc, colorLoc, model);

        

        for (i; i < 1; i++)
        {
            generarBarcosAleatorios(nivelActual,barco, barcosActivos);

        }
        // Aumentar en uno el número de barcos acada vez que se elimina a todos los barcos
		if (barcosActivos.size() == 0) {
			if (nivelActual == 1) {
                PlaySound(TEXT("resources/sounds/soundtrack.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			i--;
            nivelActual++;
		}


        actualizarBarcos(barcosActivos, transformLoc, colorLoc);
        actualizarProyectiles(proyectilesActivos, barcosActivos, transformLoc, colorLoc);

    }


    void reescalado(GLFWwindow* ventana, GLint ancho, GLint alto) {
        glViewport(0, 0, ancho, alto);
        W_WIDTH = ancho;
        W_HEIGHT = alto;

    }

    // Función para inicializar OpenGL
    void openGlInit() {
        //Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
        //Incializaciones varias
        glClearDepth(1.0f); //Valor z-buffer
        glClearColor(0.0627, 0.1725, 0.3294, 1);
        glEnable(GL_DEPTH_TEST); // z-buffer
        glEnable(GL_CULL_FACE); //ocultacion caras back
        glEnable(GL_TEXTURE_2D);


        glCullFace(GL_BACK);
    }

    int main(int argc, char** argv) {
       if (!glfwInit()) {
           std::cerr << "Error al inicializar GLFW" << std::endl;
           return -1;
       }

       glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
       glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
       glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

       //Musica de fondo

       GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Shader Seas", NULL, NULL);
	    // Habilitar VSync

       if (!window) {
           std::cerr << "Error al crear la ventana" << std::endl;
           glfwTerminate();
           return -1;
       }

       glfwSetWindowSizeCallback(window, reescalado);
       glfwMakeContextCurrent(window);
       glfwSwapInterval(1);


       if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
           std::cerr << "Error al inicializar GLAD" << std::endl;
           return -1;
       }

	   openGlInit(); // Inicializar OpenGL

       glfwSetKeyCallback(window, keyCallback);
       glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

       shadersProgram = setShaders("shader.vert", "shader.frag");


       glUniform2f(glGetUniformLocation(shadersProgram, "screenSize"), W_WIDTH, W_HEIGHT);
       ////// CREAR LOS VAOS de los objetos

       crearObjeto("resources/pirate_ship_LowPoly.obj", barco, "resources/textures/madera.jpg");
       //crearObjeto("resources/Boat.obj", barco, "resources/textures/madera.jpg");
       crearObjeto("resources/faroFinal.obj", faro);

       //crearObjetoVertices(terreno.VAO, vertices_mar); // Crear el VAO del mar
       dibujaMar(); // Crear el VAO del mar temporal
       terreno.VAO = VAOMar;
	   dibujaEsfera(proyectil1.VAO); // Crear el VAO de la esfera
       dibujaCruz(cruz.VAO);

        //proyectil1.textura=myCargaTexturas("./resources/textures/canon.jpg");
        //printf("%d\n", proyectil1.textura); 
        //terreno.textura=myCargaTexturas("./resources/textures/mar.jpg");

        // Cargar la textura de la noche
        load_skybox_textures({
            "./resources/skybox/Espacio1/espaciooscuro.png",
            "./resources/skybox/Espacio1/espaciooscuro.png",
            "./resources/skybox/Espacio1/espaciooscuro.png",
            "./resources/skybox/Espacio1/espaciooscuro.png",
            "./resources/skybox/Espacio1/espaciooscuro.png",
            "./resources/skybox/Espacio1/espaciooscuro.png"
            }, skyboxTextureNight);
        
        
       
        // Cargar la textura del día
        load_skybox_textures({
            "./resources/skybox/Dia1/Daylight_Box_Right.bmp",
            "./resources/skybox/Dia1/Daylight_Box_Left.bmp",
            "./resources/skybox/Dia1/Daylight_Box_Top.bmp",
            "./resources/skybox/Dia1/Daylight_Box_Bottom.bmp",
            "./resources/skybox/Dia1/Daylight_Box_Front.bmp",
            "./resources/skybox/Dia1/Daylight_Box_Back.bmp"
        },skyboxTextureDay);
        
        init_skybox(); 

        init_cruceta();

       if (terreno.VAO == 0) {
		   std::cerr << "Error al crear el VAO del terreno" << std::endl;
		   return -1;
       }


        
        glUniform1i(glGetUniformLocation(shadersProgram, "textura1"),0);

       while (!glfwWindowShouldClose(window)) {
           processInput(window);
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		   glUseProgram(shadersProgram);
           display();
           iluminacion();


           glfwSwapBuffers(window);
           glfwPollEvents();
       }

       glfwTerminate();
       PlaySound(NULL, 0, 0);
       return 0;
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    }

    void processInput(GLFWwindow* window) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
           beta += VELOCIDAD_CAMARA;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
           beta -= VELOCIDAD_CAMARA;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
           alpha -= VELOCIDAD_CAMARA;

        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
           alpha += VELOCIDAD_CAMARA;

        }
        static bool keyPressed = false;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !keyPressed) {
			keyPressed = true;
			lanzarProyectil(proyectil1.VAO,cameraPos, vectorDirectorCamara, barcosActivos, proyectilesActivos, VELOCIDAD_PROYECTIL, proyectil1.textura);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			keyPressed = false;
		}

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			alpha = ALPHA_INICIAL;
			beta = BETA_INICIAL;
            tipoCamara = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            alpha = ALPHA_INICIAL;
            beta = BETA_INICIAL;
            tipoCamara = 1;
        }

        static bool keyPressed2 = false;
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !keyPressed) {
            noche *= -1;
            
            // Retraso de 100 milisegundos , si no la transición no es fina 
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            keyPressed2 = true;
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) {
            keyPressed2 = false;
        }


    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
       glViewport(0, 0, width, height);
       W_WIDTH = width;
       W_HEIGHT = height;
    }

    void tiempo() {
       static double unSegundo = 0;
       currentTime = glfwGetTime();
       nbFrames++;
       lapsoTime = currentTime - lastTime;
       unSegundo += lapsoTime;
       if (unSegundo >= 1.0) {
           //printf("%f frame/s\n", double(nbFrames));
           nbFrames = 0;
           unSegundo = 0;
       }
       lastTime = currentTime;
    }
