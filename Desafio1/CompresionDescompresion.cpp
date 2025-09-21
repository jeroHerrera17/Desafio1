#include <iostream>

using namespace std;

/**
 * @brief Calcula la cantidad de dígitos en un número entero.
 *
 * @param n Número entero del cual se quiere conocer la cantidad de dígitos.
 * @return int Cantidad de dígitos de n.
 */
int contarDigitos(int n) {
    if (n == 0) {
        return 1;
    }
    int digitos = 0;
    while (n != 0) {
        n /= 10;
        digitos++;
    }
    return digitos;
}

/**
 * @brief Convierte un número entero en caracteres ASCII y los guarda en un buffer.
 *
 * @param numero Número a convertir.
 * @param destino Puntero al buffer donde se guardarán los dígitos (sin terminador nulo).
 * @return int Cantidad de dígitos escritos.
 */
int numeroAAscii(int numero, unsigned char* destino) {
    int longitud = contarDigitos(numero);
    for (int i = longitud - 1; i >= 0; --i) {
        destino[i] = static_cast<unsigned char>('0' + (numero % 10));
        numero /= 10;
    }
    return longitud;
}

/**
 * @brief Comprime un arreglo de caracteres usando el algoritmo RLE (Run-Length Encoding).
 *
 * Este algoritmo reemplaza secuencias consecutivas de caracteres repetidos por
 * el número de repeticiones seguido del carácter.
 * Ejemplo: "AAAABBBCCDAA" -> "4A3B2C1D2A".
 *
 * @param arreglo Puntero a un arreglo de caracteres sin comprimir.
 * @param longitud Cantidad de caracteres en el arreglo de entrada.
 * @return unsigned char* Puntero al texto comprimido (dinámico, debe liberarse con delete[]).
 */
unsigned char* compresionRlE(unsigned char* arreglo, int longitud) {
    // Calcular tamaño comprimido
    int tamanocomprimido = 0;
    for (int i = 0; i < longitud; ) {
        unsigned char caracter_actual = arreglo[i];
        int contador = 0;

        while (i < longitud && arreglo[i] == caracter_actual) {
            contador++;
            i++;
        }

        tamanocomprimido += contarDigitos(contador) + 1; // +1 por el carácter
    }

    // Crear buffer comprimido (+1 para terminador nulo)
    unsigned char* arregloComprimido = new unsigned char[tamanocomprimido + 1];
    int posicion = 0;

    for (int i = 0; i < longitud; ) {
        unsigned char caracter_actual = arreglo[i];
        int contador = 0;

        while (i < longitud && arreglo[i] == caracter_actual) {
            contador++;
            i++;
        }

        // Escribir el contador como ASCII
        posicion += numeroAAscii(contador, arregloComprimido + posicion);

        // Escribir el carácter mismo
        arregloComprimido[posicion++] = caracter_actual;
    }

    arregloComprimido[posicion] = '\0'; // terminador nulo
    return arregloComprimido;
}
