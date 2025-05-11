#include <iostream>
#include <stdio.h>

#include <glad.h>

#include <glfw3.h>

#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <stdlib.h>

#include "Objeto.h"
#include "definiciones.h"


//Model es la matriz base por si se quiere aplicar sobre otra matriz
glm::mat4 dibujarObjeto(const Objeto& objeto, unsigned int transformLoc, unsigned int colorLoc, glm::mat4 model) {

    // Aplicar transformaciones del objeto
    model = glm::translate(model, glm::vec3(objeto.pos[0], objeto.pos[1], objeto.pos[2]));
    model = glm::scale(model, glm::vec3(objeto.escalado[0], objeto.escalado[1], objeto.escalado[2]));
	model = glm::rotate(model, glm::radians(objeto.rotacion), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y
	//model = glm::rotate(model, glm::radians(2*), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación en X

    // Enviar la matriz transformada al shader
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Cambiar el color del objeto antes de dibujarlo
    glUniform3f(colorLoc, objeto.color.r, objeto.color.g, objeto.color.b);
    
    //glActiveTexture(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0); 
    // Dibujar el objeto
    //printf("%d\n",objeto.textura);
    glBindTexture(GL_TEXTURE_2D, objeto.textura);
    glBindVertexArray(objeto.VAO);
    glDrawArrays(GL_TRIANGLES, 0, objeto.numVertices);
    glBindTexture(GL_TEXTURE_2D, 0);

    return model; // Retorna la matriz transformada para posibles usos posteriores
}


// FUnción que se ejecuta de manera periodica (función time) para imprimir los barcos activos
void actualizarBarcos(std::vector<Objeto> &barcosActivos, unsigned int transforLoc, unsigned int colorLoc) {
	for (int i = 0; i < barcosActivos.size(); i++) {
		// Actualizar la posición del barco en función de su velocidad y el tiempo transcurrido
		barcosActivos[i].pos += barcosActivos[i].velocidad * barcosActivos[i].direccion;

		dibujarObjeto(barcosActivos[i], transforLoc, colorLoc, glm::mat4(1.0f)); // Dibuja el barco no colisionado
		if(barcosActivos[i].pos.x < -500) {
			barcosActivos.erase(barcosActivos.begin() + i); // Elimina el barco de la lista de barcos activos
			//printf("Barco fuera de la pantalla\n");
		}
	}
}
bool colisionaConAABB(const glm::vec3& punto, const Objeto& barco) {
    glm::vec3 min = barco.pos - barco.hitbox * 0.5f;
    glm::vec3 max = barco.pos + barco.hitbox * 0.5f;
    //printf("Barco - min(%.2f, %.2f, %.2f) max(%.2f, %.2f, %.2f)\n", min.x, min.y, min.z, max.x, max.y, max.z);
   // printf("Proyectil -posicion(%.2f, %.2f, %.2f) \n", punto.x, punto.y, punto.z);

    return (
        punto.x >= min.x && punto.x <= max.x &&
        punto.y >= min.y && punto.y <= max.y &&
        punto.z >= min.z && punto.z <= max.z
        );
}
// Calcula la distancia más corta entre un punto y un AABB
float distanciaAlAABB(const glm::vec3& punto, const Objeto& barco) {
    glm::vec3 min = barco.pos - barco.hitbox * 0.5f;
    glm::vec3 max = barco.pos + barco.hitbox* 0.5f;
    glm::vec3 clamped = glm::clamp(punto, min, max); // Punto más cercano dentro del AABB
    return glm::length(clamped - punto);
}

void actualizarProyectiles(std::vector<Objeto>& proyectilesActivos, std::vector<Objeto>& barcosActivos, unsigned int transforLoc, unsigned int colorLoc) {
    for (int i = 0; i < proyectilesActivos.size(); ) {
        auto& proyectil = proyectilesActivos[i];
        proyectil.pos += proyectil.direccion * proyectil.velocidad;

        // Validar que el proyectil siga dentro del espacio útil
        if (proyectil.pos.y >= 0 && proyectil.pos.x <= 1000 && proyectil.pos.z <= 1000) {
            dibujarObjeto(proyectil, transforLoc, colorLoc, glm::mat4(1.0f));

            glm::vec3 puntoImpacto = proyectil.pos;
            bool impacto = false;
            float menorDistancia = std::numeric_limits<float>::max();
            int barcoMasCercano = -1;

            // Revisar colisión con todos los barcos
            for (int j = 0; j < barcosActivos.size(); j++) {
                if (colisionaConAABB(puntoImpacto, barcosActivos[j])) {
                    //printf("Impacto con el barco %d!\n", j);
                    barcosActivos.erase(barcosActivos.begin() + j);
					proyectilesActivos.erase(proyectilesActivos.begin() + i); // Eliminar el proyectil
                    impacto = true;
                    break;
                }
                else {
                    // Calcular qué tan cerca estuvo
                    float distancia = distanciaAlAABB(puntoImpacto, barcosActivos[j]);
                    if (distancia < menorDistancia) {
                        menorDistancia = distancia;
                        barcoMasCercano = j;
                    }
                }
            }

            // Si no hubo impacto, mostrar qué tan cerca estuvo
            if (!impacto && barcoMasCercano != -1) {
                //printf("No hubo impacto. El proyectil pasó a %.2f unidades del barco %d\n", menorDistancia, barcoMasCercano);
            }
        }
        else {
            // Proyectil fuera del área válida
           // printf("Proyectil fuera de límites en x:%f, y:%f, z:%f\n", proyectil.pos.x, proyectil.pos.y, proyectil.pos.z);
            proyectilesActivos.erase(proyectilesActivos.begin() + i);
            continue;
        }

        i++;
    }
}






// FUnción para lanzar un proyectil: lo posiciona en la camara y calcula su punto de impacto con el mar
// Después se le pasa otra función para saber si colisiona con un barco. No lo representa en la pantalla.
// de eso se encarga actualizarProyectil.
void lanzarProyectil(unsigned int VAO, glm::vec3 origen, glm::vec3 vectorDirectorCamara, std::vector<Objeto> &barcosActivos, std::vector<Objeto> &proyectilesActivos, float velocidad, unsigned int textura) {
	Objeto proyectilNuevo;
	proyectilNuevo.pos = origen;
	proyectilNuevo.velocidad = velocidad;
	proyectilNuevo.escalado = { 1.2, 1.2, 1.2 };
	proyectilNuevo.color = { 1, 0, 0 };
	proyectilNuevo.numVertices = 1080;
	proyectilNuevo.VAO = VAO; // Asignar el VAO del proyectil
	proyectilNuevo.direccion = vectorDirectorCamara;
    proyectilNuevo.textura= textura;
	proyectilesActivos.push_back(proyectilNuevo); // Añadir el proyectil a la lista de proyectiles activos


}


void generarBarcoActivo(unsigned int VAO, glm::vec3 origen, glm::vec3 vectorDirectorBarco, std::vector<Objeto> &barcosActivos, float velocidad) {
    Objeto barcoNuevo;
    barcoNuevo.pos = origen;
    barcoNuevo.velocidad = velocidad;
    barcoNuevo.direccion = glm::vec3(-1, 0, 0);
    barcoNuevo.escalado = glm::vec3(TAMANO_BARCO, TAMANO_BARCO, TAMANO_BARCO);
    barcoNuevo.radioColision = RADIO_COLISION;
    barcoNuevo.VAO = VAO;
	barcoNuevo.numVertices = 1000000;
	barcoNuevo.hitbox = { 110, 65, 70 }; // Tamaño del hitbox (ajustar según sea necesario)
	barcoNuevo.rotacion = 90.0f; // Rotación inicial del barco  
    printf("Sale un barco");
    barcosActivos.push_back(barcoNuevo);


}

void generarBarcosAleatorios(int cantidad, Objeto barco, std::vector<Objeto> &barcosActivos) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Inicializar semilla aleatoria

    std::vector<float> zGenerados;
    const float minDistancia = 30.0f; // Distancia mínima entre barcos
    const float zMin = -350.0f;
    const float zMax = 350.0f;

    while (zGenerados.size() < static_cast<size_t>(cantidad)) {
        // Generar z fuera del rango prohibido (-15, 15)
        float zAleatorio;
        do {
            zAleatorio = zMin + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (zMax - zMin)));
        } while (zAleatorio > -30.0f && zAleatorio < 30.0f);

        // Verificar que no esté "pegado" a otro barco
        bool muyCerca = false;
        for (float z : zGenerados) {
            if (std::abs(z - zAleatorio) < minDistancia) {
                muyCerca = true;
                break;
            }
        }

        if (!muyCerca) {
            zGenerados.push_back(zAleatorio);
            glm::vec3 posicion(600.0f, 0.0f, zAleatorio);
            glm::vec3 direccion(-1.0f, 0.0f, 0.0f);
            generarBarcoActivo(barco.VAO, posicion, direccion, barcosActivos, barco.velocidad);
        }
    }
}
