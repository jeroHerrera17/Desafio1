#include <iostream>

using namespace std;


/**
 * @brief Aplica una operación XOR sobre un arreglo de texto.
 *
 * Esta función toma cada byte del arreglo y le aplica XOR con la clave dada.
 *
 * @param texto Puntero al arreglo de bytes del texto.
 * @param clave Byte usado como clave para el XOR.
 * @param tamaño Número de bytes en el arreglo.
 */


void DoXOR(unsigned char* texto, unsigned char clave, int tamaño) {

    for (int i = 0; i < tamaño; ++i) {
        texto[i] ^= clave;
    }
}


/**
 * @brief Rota los bits de cada byte de un texto hacia la derecha.
 *
 * Esta función rota los bits de cada byte del texto hacia la derecha
 * n posiciones. La rotación es circular.
 *
 * @param tamaño Número de bytes en el texto.
 * @param texto Puntero al arreglo de bytes del texto.
 * @param n Número de posiciones a rotar (0-7).
 */
void RotarDerecha(int tamaño, unsigned char* texto, int n) {
    n %= 8; // Asegurar que n esté entre 0 y 7
    for (int i = 0; i < tamaño; ++i) {
        texto[i] = (texto[i] >> n) | (texto[i] << (8 - n));
    }
}

/**
 * @brief Rota los bits de cada byte de un texto hacia la izquierda.
 *
 * Esta función rota los bits de cada byte del texto hacia la izquierda
 * n posiciones. La rotación es circular.
 *
 * @param tamaño Número de bytes en el texto.
 * @param texto Puntero al arreglo de bytes del texto.
 * @param n Número de posiciones a rotar (0-7).
 */
void RotarIzquierda(int tamaño, unsigned char* texto, int n) {
    n %= 8; // Asegurar que n esté entre 0 y 7
    for (int i = 0; i < tamaño; ++i) {
        texto[i] = (texto[i] << n) | (texto[i] >> (8 - n));
    }
}
