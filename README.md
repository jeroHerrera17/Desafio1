# Desafío 1: Encriptación y descompresión de texto

![Estado](https://img.shields.io/badge/Estado-Finalizado-green)

Este proyecto tiene como objetivo reconstruir un mensaje que ha pasado por un proceso de ** compresión ** (usando RLE o LZ78) y posteriormente fue ** encriptado ** mediante operaciones a nivel de bits (rotación y XOR).  

La solución implementa un enfoque de **búsqueda exhaustiva** (fuerza bruta), probando distintas combinaciones de parámetros hasta encontrar la correcta. A pesar del carácter exhaustivo, se incorporaron optimizaciones para reducir iteraciones innecesarias y evitar un uso excesivo de memoria dinámica.

## Características principales

- Implementación modular en C++.
- Métodos de descompresión: RLE y LZ78.
- Métodos de desencriptado: XOR y rotación de bits.
- Validación de resultados comparando con una pista.
- Procesamiento automático de múltiples archivos de entrada.

## Clonacion

Para clonar el repositorio:

```bash
git clone https://github.com/jeroHerrera17/Desafio1.git
````
El programa leerá automáticamente los archivos de entrada encriptados y la pista, probará las posibles combinaciones de parámetros y guardará el resultado en un archivo de salida cuando se detecte una coincidencia.

## Organización del código

La estructura del proyecto está organizada en módulos para facilitar la comprensión, el mantenimiento y la escalabilidad del código. Cada archivo cumple un rol específico dentro de la solución:

- `main.cpp`  
  Contiene el **punto de entrada** del programa. Se encarga de:
  - Iterar sobre los diferentes archivos de entrada.
  - Construir las rutas dinámicamente.
  - Llamar a la función principal `BuscarParametros`, que inicia el proceso de desencriptado y descompresión.
  - Gestionar la memoria utilizada en cada iteración.

- `funciones.h`  
  Archivo de cabecera que **declara** todas las funciones y estructuras utilizadas en el programa. Define la interfaz que conecta `main.cpp` con la implementación de las rutinas en `funciones.cpp`.

- `funciones.cpp`  
  Contiene la **implementación de las funciones** declaradas en `funciones.h`. Aquí se encuentran los algoritmos de desencriptado (XOR y rotación), las rutinas de descompresión (RLE y LZ78), así como las funciones de validación y de lectura/escritura de archivos.

- `datos/`  
  Carpeta destinada a almacenar los archivos de **entrada y pistas** necesarias para la ejecución del programa. Incluye:
  - Archivos encriptados y comprimidos (`encriptadoX.txt`).
  - Archivos de pistas (`pistaX.txt`).
  - Archivos resultantes generados por el programa (`modificadoX.txt`).

Esta organización permite mantener una separación clara entre la **lógica del programa** (main), las **funciones auxiliares** (módulos en C++) y los **datos utilizados** para probar la solución.


## Autores

* Alejandro Bedoya Zuluaga
* Jerónimo Espinosa Herrera

