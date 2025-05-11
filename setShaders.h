/******************************************
  Lectura y Creacion de Shaders
*******************************************/
#ifndef SET_SHADERS_
#define SET_SHADERS_

// Librerias necesarias
#include <glad.h>

#include <glfw3.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Para leer los ficheros de texto plano


#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

static char* textFileRead(const char* fn) {
    if (fn == nullptr) {
        std::cerr << "ERROR: Filename is null" << std::endl;
        return nullptr;
    }

    std::ifstream file(fn, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "ERROR: Could not open file '" << fn << "'" << std::endl;
        return nullptr;
    }

    std::ostringstream contents;
    contents << file.rdbuf();  // Lee todo el contenido del archivo

    std::string str = contents.str();  // Guarda como string temporal

    // Reservar memoria en heap y copiar el contenido
    char* result = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), result);
    result[str.size()] = '\0';  // Null terminación

    return result;
}


// Nos indica e imprime por pantalla si hay algun error al crear el shader o el program

static void
printShaderInfoLog(GLuint obj)
{
        int infologLength = 0;
        int charsWritten = 0;
        char *infoLog;

        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

        if (infologLength > 0) {
                infoLog = (char *) malloc(infologLength);
                glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
                // printf("%s\n", infoLog);
                free(infoLog);
        }
}

static void
printProgramInfoLog(GLuint obj)
{
        int infologLength = 0;
        int charsWritten = 0;
        char *infoLog;

        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

        if (infologLength > 0) {
                infoLog = (char *) malloc(infologLength);
                glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
                // printf("%s\n", infoLog);
                free(infoLog);
        }
}


static GLuint
setShaders(const char *nVertx, const char *nFrag, GLuint& skyboxTexture)
{
        // Definicion vertex y fragment shader
        GLuint vertexShader, fragmentShader;
        // ficheros de vertex y fragment shaders
        char *ficherovs = NULL;
        char *ficherofs = NULL;
        // Definicion del program Shader
        GLuint progShader;
        const char *codigovs = NULL;
        const char *codigofs = NULL;

        // Creo el vertexShader y el FragmentShader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Leo el codigo del ficheo y hay error devuelve un NULL
        ficherovs = textFileRead(nVertx);
        if (ficherovs == NULL)
                return (GLuint) NULL;
        ficherofs = textFileRead(nFrag);
        if (ficherofs == NULL)
                return (GLuint) NULL;

        // Lo igual al puntero para cargarlos
        codigovs = ficherovs;
        codigofs = ficherofs;
        // Los cargo
        glShaderSource(vertexShader, 1, &codigovs, NULL);
        glShaderSource(fragmentShader, 1, &codigofs, NULL);

        // Libero los ficheros
        free(ficherovs);
        free(ficherofs);

        // Copio vertex y Fragment
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        // Miro si hay algun error
        printShaderInfoLog(vertexShader);
        printShaderInfoLog(fragmentShader);

        // Creo el programa asociado
        progShader = glCreateProgram();

        // Le attacheo shaders al programa
        glAttachShader(progShader, vertexShader);
        glAttachShader(progShader, fragmentShader);

        // Lo linko
        glLinkProgram(progShader);
        // A ver si hay errores
        printProgramInfoLog(progShader);

        return (progShader);
}

static GLuint
setShaders_str(const char *nVertx, const char *nFrag)
{
        GLuint vertexShader, fragmentShader;
        GLuint progShader;

        // Creo el vertexShader y el FragmentShader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Los cargo
        glShaderSource(vertexShader, 1, &nVertx, NULL);
        glShaderSource(fragmentShader, 1, &nFrag, NULL);

        // Copio vertex y Fragment
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        // Miro si hay algun error
        printShaderInfoLog(vertexShader);
        printShaderInfoLog(fragmentShader);

        // Creo el programa asociado
        progShader = glCreateProgram();

        // Le attacheo shaders al programa
        glAttachShader(progShader, vertexShader);
        glAttachShader(progShader, fragmentShader);

        // Lo linko
        glLinkProgram(progShader);
        // A ver si hay errores
        printProgramInfoLog(progShader);

        return (progShader);
}

#endif

