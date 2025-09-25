#include<iostream>
#include "funciones.h"
using namespace std;

/**
 * @brief Intenta descomprimir los datos usando diferentes algoritmos de compresión
 *        y busca la pista dentro del resultado descomprimido.
 *
 * Este procedimiento prueba con RLE (y tiene código comentado para LZ78).
 * Si la pista se encuentra dentro de la descompresión, retorna `true`.
 *
 * @param data Puntero al arreglo de datos comprimidos.
 * @param size Tamaño en bytes del arreglo de datos comprimidos.
 * @param pista Puntero al arreglo con la secuencia a buscar.
 * @param sizePista Tamaño en bytes de la pista.
 * @return `true` si la pista es encontrada en alguna de las descompresiones, `false` en caso contrario.
 */
bool ProbarDescompresion(unsigned char* data, int size, unsigned char* pista, int sizePista) {
    int total = 0;
    int pos = 0; // Reiniciar posición

    // Probar descompresión RLE
    unsigned char* descomprimido = descompresionRLE(data, size, total);
    if (descomprimido != nullptr) {
        for (int i = 0; i < total && pos < sizePista; i++) { // usar 'total' no 'size'
            if (descomprimido[i] == pista[pos]) {
                pos++;
                if (pos == sizePista) {
                    delete[] descomprimido; // Liberar memoria
                    return true;
                }
            } else {
                pos = 0; // Reiniciar si no coincide
            }
        }
        delete[] descomprimido; // Liberar memoria
    }

    // Reiniciar posición para segundo intento
    pos = 0;
    total = 0;

    /**
     * Código alternativo para probar con descompresión LZ78.
     * Actualmente está comentado para evitar conflictos.
     *
     *
     *
     * @code
     * descomprimido = descompresionLZ78(data, size, total);
     * if (descomprimido != nullptr) {
     *     for (int i = 0; i < total && pos < sizePista; i++) {
     *         if (descomprimido[i] == pista[pos]) {
     *             pos++;
     *             if (pos == sizePista) {
     *                 delete[] descomprimido;
     *                 return true;
     *             }
     *         } else {
     *             pos = 0;
     *         }
     *     }
     *     delete[] descomprimido;
     * }
     * @endcode
     */

    return false;
}

/**
 * @brief Busca los parámetros correctos de desencriptación (clave XOR y bits de rotación).
 *
 * Esta función prueba todas las combinaciones posibles de:
 * - Claves XOR (0 a 254)
 * - Rotaciones de bits (0 a 6)
 *
 * Para cada combinación, aplica:
 * 1. Operación XOR sobre los datos.
 * 2. Rotación de bits a la derecha.
 * 3. Intento de descompresión y búsqueda de la pista.
 *
 * Si encuentra coincidencia, guarda los parámetros en `nbits` y `claveK`.
 *
 * @param data Puntero a los datos encriptados.
 * @param nbits Variable de salida donde se almacenará la cantidad de bits de rotación encontrada.
 * @param claveK Variable de salida donde se almacenará la clave XOR encontrada.
 * @param sizeEncriptado Tamaño en bytes del arreglo de datos encriptados.
 * @param sizePista Tamaño en bytes de la pista.
 * @param pista Puntero a la secuencia de bytes a buscar.
 */
void BuscarParametros(unsigned char* data, int& nbits, int& claveK, int sizeEncriptado, int sizePista, unsigned char* pista) {
    cout << "Tamaño pista: " << sizePista << ", Tamaño encriptado: " << sizeEncriptado << "\n" << endl;
    mostrarContenido("Pista", pista, sizePista);

    // Búsqueda exhaustiva (fuerza bruta)
    for (int clave = 0; clave < 255; clave++) {
        for (int bits = 0; bits < 7; bits++) { // 0-6 bits de rotación
            unsigned char* dataCopia = new unsigned char[sizeEncriptado];
            // Restaurar datos originales
            for (int k = 0; k < sizeEncriptado; k++) {
                dataCopia[k] = data[k];
            }

            // Aplicar transformaciones
            DoXOR(dataCopia, clave, sizeEncriptado);
            RotarDerecha(sizeEncriptado, dataCopia, bits);

            // Probar descompresión
            if (ProbarDescompresion(dataCopia, sizeEncriptado, pista, sizePista)) {
                cout << "¡Parámetros encontrados!" << endl;
                cout << "Clave XOR: " << clave << endl;
                cout << "Bits de rotación: " << bits << endl;
                nbits = bits;
                claveK = clave;
                delete[] dataCopia;
                return;
            }

            // Opcional: aquí se podría mostrar progreso
            delete[] dataCopia; // Liberar memoria en cada iteración
        }
    }

    cout << "No se encontraron parametros validos." << endl;
    return;
}
