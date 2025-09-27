#include<iostream>
#include "funciones.h"
using namespace std;

/**
 * @brief Funcion para buscar una secuencia dentro de otra usando arreglos
 */
bool buscarSecuencia(unsigned char* texto, int sizeTexto, unsigned char* pista, int sizePista) {
    if (texto == nullptr || pista == nullptr || sizeTexto < sizePista) {
        return false;
    }

    for (int i = 0; i <= sizeTexto - sizePista; i++) {
        bool coincide = true;
        for (int j = 0; j < sizePista; j++) {
            if (texto[i + j] != pista[j]) {
                coincide = false;
                break;
            }
        }
        if (coincide) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Intenta descomprimir los datos usando diferentes algoritmos de compresion
 *        y busca la pista dentro del resultado descomprimido.
 *
 * Este procedimiento prueba con RLE y LZ78 corregidos para el formato de ternas.
 * Si la pista se encuentra dentro de la descompresion, retorna `true`.
 *
 * @param data Puntero al arreglo de datos comprimidos.
 * @param size Tamaño en bytes del arreglo de datos comprimidos.
 * @param pista Puntero al arreglo con la secuencia a buscar.
 * @param sizePista Tamaño en bytes de la pista.
 * @return `true` si la pista es encontrada en alguna de las descompresiones, `false` en caso contrario.
 */
bool ProbarDescompresion(unsigned char* data, int size, unsigned char* pista, int sizePista) {
    if (data == nullptr || pista == nullptr || size <= 0 || sizePista <= 0) {
        return false;
    }

    // Verificar que el tamaño sea múltiplo de 3 para el formato de ternas
    if (size % 3 != 0) {
        cout << "Advertencia: El tamaño no es múltiplo de 3 en ProbarDescompresion" << endl;
    }

    //cout << "Probando descompresion RLE..." << endl;
    int total_rle = 0;

    // Probar descompresion RLE - ahora adaptada para formato de ternas
    unsigned char* descomprimido_rle = descompresionRLE(data, size, total_rle);
    if (descomprimido_rle != nullptr && total_rle > 0) {
        //cout << "RLE genero " << total_rle << " bytes descomprimidos" << endl;

        // Buscar la pista en el resultado
        if (buscarSecuencia(descomprimido_rle, total_rle, pista, sizePista)) {
            cout << "¡Pista encontrada con RLE!" << endl;
            cout << "Contenido descomprimido (primeros 100 caracteres):" << endl;
            int mostrar = (total_rle > 100) ? 100 : total_rle;
            mostrarContenido(descomprimido_rle, mostrar);
            delete[] descomprimido_rle;
            return true;
        }
        delete[] descomprimido_rle;
    } else {
        //cout << "RLE no pudo descomprimir los datos" << endl;
    }

    //cout << "Probando descompresion LZ78..." << endl;
    int total_lz78 = 0;

    // Probar descompresion LZ78 - ahora adaptada para formato de ternas
    unsigned char* descomprimido_lz78 = descompresionLZ78(data, size, total_lz78);
    if (descomprimido_lz78 != nullptr && total_lz78 > 0) {
        //cout << "LZ78 genero " << total_lz78 << " bytes descomprimidos" << endl;

        // Buscar la pista en el resultado
        if (buscarSecuencia(descomprimido_lz78, total_lz78, pista, sizePista)) {
            cout << "¡Pista encontrada con LZ78!" << endl;
            cout << "Contenido descomprimido (primeros 100 caracteres):" << endl;
            int mostrar = (total_lz78 > 100) ? 100 : total_lz78;
            mostrarContenido(descomprimido_lz78, mostrar);
            delete[] descomprimido_lz78;
            return true;
        }
        delete[] descomprimido_lz78;
    } else {
        //cout << "LZ78 no pudo descomprimir los datos" << endl;
    }

    return false;
}

/**
 * @brief Busca los parametros correctos de desencriptacion (clave XOR y bits de rotacion).
 *
 * Esta funcion prueba todas las combinaciones posibles de:
 * - Claves XOR (0 a 254)
 * - Rotaciones de bits (0 a 7)
 *
 * Para cada combinacion, aplica:
 * 1. Operacion XOR sobre los datos.
 * 2. Rotacion de bits a la derecha.
 * 3. Intento de descompresion y búsqueda de la pista.
 *
 * Si encuentra coincidencia, guarda los parametros en `nbits` y `claveK`.
 *
 * @param data Puntero a los datos encriptados.
 * @param nbits Variable de salida donde se almacenara la cantidad de bits de rotacion encontrada.
 * @param claveK Variable de salida donde se almacenara la clave XOR encontrada.
 * @param sizeEncriptado Tamaño en bytes del arreglo de datos encriptados.
 * @param sizePista Tamaño en bytes de la pista.
 * @param pista Puntero a la secuencia de bytes a buscar.
 */
void BuscarParametros(unsigned char* data, int& nbits, int& claveK, int sizeEncriptado, int sizePista, unsigned char* pista) {

    // cout << "Iniciando búsqueda de parametros..." << endl;
    // cout << "Tamaño encriptado: " << sizeEncriptado << ", Tamaño pista: " << sizePista << endl;

    if (data == nullptr || pista == nullptr) {
        cout << "Error: datos nulos" << endl;
        return;
    }

    // Verificar que el tamaño sea múltiplo de 3
    if (sizeEncriptado % 3 != 0) {
        cout << "Advertencia: El tamaño no es múltiplo de 3 (formato de terna)" << endl;
    }

    // Mostrar la pista
    cout << "Pista a buscar: ";
    mostrarContenido(pista, sizePista);

    int intentos = 0;
    const int MAX_INTENTOS = 2040; // 255 * 8 = limite maximo para evitar ciclos infinitos

    // Búsqueda exhaustiva con limites
    for (int clave = 0; clave < 255 && intentos < MAX_INTENTOS; clave++) {
        for (int bits = 0; bits <= 7 && intentos < MAX_INTENTOS; bits++) {
            intentos++;

            // Mostrar progreso cada 200 intentos para no saturar la consola
            if (intentos % 200 == 0) {
                // cout << "Progreso: " << intentos << "/" << MAX_INTENTOS
                //      << " (Clave: " << clave << ", Bits: " << bits << ")" << endl;
            }

            unsigned char* dataCopia = new unsigned char[sizeEncriptado];

            // Restaurar datos originales en la copia
            for (int k = 0; k < sizeEncriptado; k++) {
                dataCopia[k] = data[k];
            }

            // Aplicar transformaciones en el orden correcto
            DoXOR(dataCopia, (unsigned char)clave, sizeEncriptado);
            RotarDerecha(sizeEncriptado, dataCopia, bits);

            // Probar descompresion con los datos transformados
            if (ProbarDescompresion(dataCopia, sizeEncriptado, pista, sizePista)) {
                cout << "¡Parametros encontrados!" << endl;
                cout << "Clave XOR: " << clave << " (0x" << hex << clave << dec << ")" << endl;
                cout << "Bits de rotacion: " << bits << endl;
                nbits = bits;
                claveK = clave;
                delete[] dataCopia;
                return;
            }

            // Liberar memoria en cada iteracion para evitar memory leaks
            delete[] dataCopia;
        }
    }

    // //cout << "No se encontraron parametros validos despues de " << intentos << " intentos." << endl;
    // cout << "Puede que necesites verificar:" << endl;
    // cout << "- Las funciones de descompresion RLE y LZ78" << endl;
    // cout << "- El formato correcto de la pista" << endl;
    // cout << "- Que los datos esten en formato de ternas" << endl;
}
