# Proyecto ShadersSea

**Autores**: Xabier Nóvoa Gómez, Adrián Quiroga Linares

---

## 1. 📄 Descripción General del Proyecto

El proyecto consiste en la simulación interactiva de un faro situado sobre una isla que defiende su entorno de oleadas de barcos enemigos.  
El faro está equipado con un mecanismo que lanza proyectiles (esferas) hacia los barcos con el objetivo de destruirlos.  

A medida que se eliminan todos los barcos de una oleada, el sistema incrementa automáticamente el nivel de dificultad, aumentando la cantidad de barcos por oleada de forma incremental:

- Nivel 1: 1 barco  
- Nivel 2: 2 barcos  
- Nivel 3: 3 barcos  
- ... y así sucesivamente.

---

## 2. ⭐ Características Principales

- **Skybox**: Implementación de una skybox utilizando un shader embebido para representar el entorno.
- **Iluminación Realista del Faro**: Movimiento orbital de la luz del faro, simulando el comportamiento de una luz giratoria real.
- **Sistema de Oleadas**: Incremento automático de dificultad con cada oleada superada.
- **Mecánica de Disparo**: Lanzamiento de proyectiles desde la cámara con detección de colisiones hacia los barcos enemigos.
- **Cruceta de Apuntado**: Cruceta activada en modo de disparo, renderizada con un shader embebido para apuntar con precisión.
- **Carga de Modelos 3D**: Uso de la biblioteca `TinyObjLoader` para cargar modelos `.obj` como barcos y otros objetos.
- **Música Inmersiva**: Reproducción de música de fondo tras el impacto del primer proyectil, gestionada mediante la API de audio de Windows.

---

## 3. 🎮 Controles del Usuario

### Movimiento de la Cámara
- `W` - Avanzar  
- `A` - Girar a la izquierda  
- `S` - Retroceder  
- `D` - Girar a la derecha  

### Disparo
- `Espacio` - Lanzar proyectil desde la cámara

### Cambio de Iluminación
- `N` - Cambiar entre distintos modos de iluminación

### Modos de Cámara
- `1` - Modo disparo con cruceta activada  
- `2` - Modo libre para observar la escena

---

## 5. Imagenes
![Vista del faro](capturas/captura1)

## 4. 📚 Documentación Externa Consultada

- [Documentación oficial de OpenGL](https://www.opengl.org/documentation/)
- [Repositorio de TinyObjLoader](https://github.com/tinyobjloader/tinyobjloader)
- [LearnOpenGL.com](https://learnopengl.com/) – Skybox, Lighting, Camera, Collision Detection
- Documentación de la API de audio de Windows
- Foros de Stack Overflow para resolución de problemas específicos
