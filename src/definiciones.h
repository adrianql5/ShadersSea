// Copyright (c) 2025 Adri√°n Quiroga Linares Lectura y referencia permitidas; reutilizaci√≥n y plagio prohibidos

#pragma once

// Macros
#define RGB_TO_FLOAT(x) ((x) / 256.0f) // convierto RGB a float
#define DEG_TO_RAD(x) ((x) * 0.01745329252f) // convierto grados a radianes


// Propiedades gameplay
#define VELOCIDAD_BARCO 0.1f
#define VELOCIDAD_PROYECTIL 2.0f

// Propiedades modelos
#define TAMANO_PROYECTIL 0.5f

#define TAMANO_BARCO 15.0f

#define TAMANO_FARO 1.5f

//Propiedades colisiones
#define RADIO_COLISION 130.0f
#define ALTURA_HITBOX 15.0f

// Propiedades de la c·mara
#define DISTANCIA 100.0f
#define ALTURA_CAMARA 33.0f
#define ALPHA_INICIAL 0.0f
#define BETA_INICIAL 1.0f
#define VELOCIDAD_CAMARA 0.25f
#define ALPHA_MINIMO 0.0f
#define ALPHA_MAXIMO 360.0f
#define BETA_MINIMO -89.0f
#define BETA_MAXIMO 89.0f

// Posiciones iniciales
#define POSICION_INICIAL_FARO glm::vec3(0.0f, 0.0f, 0.0f)
#define LIMITE_GENERACION_BARCOS_MAX glm::vec3(500.0f,0.0,123.0f);
#define LIMITE_GENERACION_BARCOS_MIN glm::vec3(500.0f,0.0,-123.0f);


// Iluminacion
#define LUZ_AMBIENTE 0.3f
#define LUZ_DIRECCIONAL 0.5f
#define LUZ_SPECULAR 0.5f
#define LUZ_INTENSIDAD 1.0f
#define LUZ_POSICION glm::vec3(0.0f, 10.0f, 0.0f) // PosiciÛn de la luz
#define LUZ_DIFUSA_ANGULO 30.0f // ¡ngulo de apertura de la luz difusa

//COnversiones
#define GR 0.0175 // Grados a radianes



