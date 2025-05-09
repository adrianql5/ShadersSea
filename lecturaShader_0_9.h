/******************************************
  Lectura y Creacion de Shaders
*******************************************/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS //Evitamos alguno errores de funciones no seguras
#endif
#pragma warning(disable:4996) //Evitamos alguno errores de funciones no seguras

//Librerias necesarias 
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


GLuint vertexShader, fragmentShader; // Definicion  vertex y fragment shader
GLuint  progShader;					// Definicion de el program Shader

									
// Para leer los ficheros de texto plano 


char *textFileRead(const char *fn) {
	
	FILE *fichero;
	char *contenido = NULL;

	int count = 0;
	
	if (fn != NULL) {
		fichero = fopen(fn, "rt");

		if (fichero != NULL) {

			fseek(fichero, 0, SEEK_END);
			count = ftell(fichero);
			rewind(fichero);

			if (count > 0) {
				contenido = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(contenido, sizeof(char), count, fichero);
				contenido[count] = '\0';
			}
			fclose(fichero);
		}
		else {

			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			return NULL;
		}
	}
	
	return contenido;
}

// Nos indica e imprime por pantalla si hay algun error al crear el shader o el program

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}


GLuint setShaders(const char* nVertx, const char* nFrag) {
	GLuint vertexShader, fragmentShader, progShader;

	// Crear shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Leer el código fuente de los archivos
	char* ficherovs = textFileRead(nVertx);
	if (ficherovs == NULL) return 0; // Error
	char* ficherofs = textFileRead(nFrag);
	if (ficherofs == NULL) return 0; // Error

	const char* codigovs = ficherovs;
	const char* codigofs = ficherofs;

	// Cargar shaders
	glShaderSource(vertexShader, 1, &codigovs, NULL);
	glShaderSource(fragmentShader, 1, &codigofs, NULL);

	free(ficherovs);
	free(ficherofs);

	// Compilar shaders
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	printShaderInfoLog(vertexShader);
	printShaderInfoLog(fragmentShader);

	// Crear programa y enlazar shaders
	progShader = glCreateProgram();
	glAttachShader(progShader, vertexShader);
	glAttachShader(progShader, fragmentShader);
	glLinkProgram(progShader);
	printProgramInfoLog(progShader);

	// Eliminar shaders (ya no son necesarios después de enlazar)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return progShader; // Retornar el programa creado
}


