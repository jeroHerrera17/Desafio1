
#include <iostream>
#include "Funciones.h"
    using namespace std;

/**
 * @brief   Función para buscar una secuencia dentro de otra usando arreglos.
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
 * @brief Intenta descomprimir los datos usando diferentes algoritmos de compresión
 *        y busca la pista dentro del resultado descomprimido.
 *
 * Este procedimiento prueba con RLE y LZ78 corregidos para el formato de ternas.
 * Si la pista se encuentra dentro de la descompresión, retorna true y guarda el archivo.
 *
 * @param data Puntero al arreglo de datos comprimidos.
 * @param size Tamaño en bytes del arreglo de datos comprimidos.
 * @param pista Puntero al arreglo con la secuencia a buscar.
 * @param sizePista Tamaño en bytes de la pista.
 * @param rutaArchivoModificado Ruta donde guardar el archivo descomprimido.
 * @return true si la pista es encontrada en alguna de las descompresiones, false en caso contrario.
 */
bool ProbarDescompresion(unsigned char* data, int size, unsigned char* pista, int sizePista, const char* rutaArchivoModificado) {
    if (data == nullptr || pista == nullptr || size <= 0 || sizePista <= 0) {
        return false;
    }

    // Verificar que el tamaño sea múltiplo de 3 para el formato de ternas
    if (size % 3 != 0) {
        cout << "Advertencia: El tamaño no es múltiplo de 3 en ProbarDescompresion" << endl;
    }

    int total_rle = 0;

    // Probar descompresión RLE - adaptada para formato de ternas
    unsigned char* descomprimido_rle = descompresionRLE(data, size, total_rle);
    if (descomprimido_rle != nullptr && total_rle > 0) {
        if (buscarSecuencia(descomprimido_rle, total_rle, pista, sizePista)) {
            cout << "¡Pista encontrada con RLE!" << endl;
            cout << "Contenido descomprimido (primeros 100 caracteres):" << endl;
            int mostrar = (total_rle > 100) ? 100 : total_rle;
            mostrarContenido(descomprimido_rle, mostrar);

            if (crearArchivoConTexto(rutaArchivoModificado, descomprimido_rle, total_rle)) {
                cout << "Archivo guardado exitosamente en: " << rutaArchivoModificado << endl;
            } else {
                cout << "Error al guardar el archivo en: " << rutaArchivoModificado << endl;
            }

            delete[] descomprimido_rle;
            return true;
        }
        delete[] descomprimido_rle;
    }

    int total_lz78 = 0;

    // Probar descompresión LZ78 - adaptada para formato de ternas
    unsigned char* descomprimido_lz78 = descompresionLZ78(data, size, total_lz78);
    if (descomprimido_lz78 != nullptr && total_lz78 > 0) {
        if (buscarSecuencia(descomprimido_lz78, total_lz78, pista, sizePista)) {
            cout << "¡Pista encontrada con LZ78!" << endl;
            cout << "Contenido descomprimido (primeros 100 caracteres):" << endl;
            int mostrar = (total_lz78 > 100) ? 100 : total_lz78;
            mostrarContenido(descomprimido_lz78, mostrar);

            if (crearArchivoConTexto(rutaArchivoModificado, descomprimido_lz78, total_lz78)) {
                cout << "Archivo guardado exitosamente en: " << rutaArchivoModificado << endl;
            } else {
                cout << "Error al guardar el archivo en: " << rutaArchivoModificado << endl;
            }

            delete[] descomprimido_lz78;
            return true;
        }
        delete[] descomprimido_lz78;
    }

    return false;
}

/**
 * @brief Busca los parámetros correctos de desencriptación (clave XOR y bits de rotación).
 *
 * Esta función prueba todas las combinaciones posibles de:
 * - Claves XOR (0 a 254)
 * - Rotaciones de bits (0 a 7)
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
 * @param rutaArchivoModificado Ruta donde guardar el archivo descomprimido.
 */
void BuscarParametros(unsigned char* data, int& nbits, int& claveK, int sizeEncriptado, int sizePista, unsigned char* pista, const char* rutaArchivoModificado) {
    if (data == nullptr || pista == nullptr) {
        cout << "Error: datos nulos" << endl;
        return;
    }

    if (sizeEncriptado % 3 != 0) {
        cout << "Advertencia: El tamaño no es múltiplo de 3 (formato de terna)" << endl;
    }

    cout << "Pista a buscar: ";
    mostrarContenido(pista, sizePista);

    int intentos = 0;
    const int MAX_INTENTOS = 2040; // 255 * 8

    for (int clave = 0; clave < 255 && intentos < MAX_INTENTOS; clave++) {
        for (int bits = 0; bits <= 7 && intentos < MAX_INTENTOS; bits++) {
            intentos++;

            // Mostrar progreso cada 200 intentos (opcional)
            if (intentos % 200 == 0) {
                // cout << "Progreso: " << intentos << "/" << MAX_INTENTOS
                //      << " (Clave: " << clave << ", Bits: " << bits << ")" << endl;
            }

            unsigned char* dataCopia = new unsigned char[sizeEncriptado];
            for (int k = 0; k < sizeEncriptado; k++) {
                dataCopia[k] = data[k];
            }

            DoXOR(dataCopia, (unsigned char)clave, sizeEncriptado);
            RotarDerecha(sizeEncriptado, dataCopia, bits);

            if (ProbarDescompresion(dataCopia, sizeEncriptado, pista, sizePista, rutaArchivoModificado)) {
                cout << "¡Parámetros encontrados!" << endl;
                cout << "Clave XOR: " << clave << " (0x" << hex << clave << dec << ")" << endl;
                cout << "Bits de rotación: " << bits << endl;
                nbits = bits;
                claveK = clave;
                delete[] dataCopia;
                return;
            }

            delete[] dataCopia;
        }
    }

    cout << "No se encontraron parámetros válidos después de " << intentos << " intentos." << endl;
}

