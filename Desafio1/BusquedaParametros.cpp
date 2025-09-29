#include <iostream>
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
 * @brief Funcion para probar descompresion con parametros integrados
 *
 * @param data Datos encriptados originales
 * @param size Tamaño de los datos
 * @param pista Secuencia a buscar
 * @param sizePista Tamaño de la pista
 * @param claveXOR Clave XOR a aplicar
 * @param rotacionBits Bits de rotacion a aplicar
 * @param rutaArchivoModificado Ruta donde guardar el resultado exitoso
 * @return true si encuentra la pista, false en caso contrario
 */
bool ProbarDescompresion(unsigned char* data, int size, unsigned char* pista, int sizePista,
                         unsigned char claveXOR, int rotacionBits, const char* rutaArchivoModificado) {
    if (data == nullptr || pista == nullptr || size <= 0 || sizePista <= 0) {
        return false;
    }

    // Probar RLE con desencriptado integrado
    int total_rle = 0;
    bool esValido_rle = false;
    unsigned char* descomprimido_rle = descompresionRLE(data, size, claveXOR, rotacionBits,
                                                        total_rle, esValido_rle);

    if (descomprimido_rle != nullptr && esValido_rle && total_rle > 0) {
        if (buscarSecuencia(descomprimido_rle, total_rle, pista, sizePista)) {
            cout << "Pista encontrada con RLE!" << endl;
            cout << "Clave XOR: " << (int)claveXOR << " (0x" << hex << (int)claveXOR << dec << ")" << endl;
            cout << "Bits de rotacion: " << rotacionBits << endl;
            cout << "Contenido descomprimido (primeros 100 caracteres):" << endl;

            int mostrar = (total_rle > 100) ? 100 : total_rle;
            mostrarContenido(descomprimido_rle, mostrar);

            // Guardar archivo descomprimido si se proporciona la ruta
            if (rutaArchivoModificado != nullptr) {
                if (crearArchivoConTexto(rutaArchivoModificado, descomprimido_rle, total_rle)) {
                    cout << "Archivo descomprimido guardado en: " << rutaArchivoModificado << endl;
                } else {
                    cout << "Error al guardar el archivo descomprimido" << endl;
                }
            }

            delete[] descomprimido_rle;
            return true;
        }
        delete[] descomprimido_rle;
    }

    // Probar LZ78 con desencriptado integrado
    int total_lz78 = 0;
    bool esValido_lz78 = false;
    unsigned char* descomprimido_lz78 = descompresionLZ78(data, size, claveXOR, rotacionBits,
                                                          total_lz78, esValido_lz78);

    if (descomprimido_lz78 != nullptr && esValido_lz78 && total_lz78 > 0) {
        if (buscarSecuencia(descomprimido_lz78, total_lz78, pista, sizePista)) {
            cout << "Pista encontrada con LZ78!" << endl;
            cout << "Clave XOR: " << (int)claveXOR << " (0x" << hex << (int)claveXOR << dec << ")" << endl;
            cout << "Bits de rotacion: " << rotacionBits << endl;
            cout << "Contenido descomprimido (primeros 100 caracteres):" << endl;

            int mostrar = (total_lz78 > 100) ? 100 : total_lz78;
            mostrarContenido(descomprimido_lz78, mostrar);

            // Guardar archivo descomprimido si se proporciona la ruta
            if (rutaArchivoModificado != nullptr) {
                if (crearArchivoConTexto(rutaArchivoModificado, descomprimido_lz78, total_lz78)) {
                    cout << "Archivo descomprimido guardado en: " << rutaArchivoModificado << endl;
                } else {
                    cout << "Error al guardar el archivo descomprimido" << endl;
                }
            }

            delete[] descomprimido_lz78;
            return true;
        }
        delete[] descomprimido_lz78;
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
 * Para cada combinacion, intenta descomprimir directamente los datos con los parametros
 * integrados y busca la pista en el resultado.
 *
 * Si encuentra coincidencia, guarda los parametros en `nbits` y `claveK`.
 *
 * @param data Puntero a los datos encriptados.
 * @param nbits Variable de salida donde se almacenara la cantidad de bits de rotacion encontrada.
 * @param claveK Variable de salida donde se almacenara la clave XOR encontrada.
 * @param sizeEncriptado Tamaño en bytes del arreglo de datos encriptados.
 * @param sizePista Tamaño en bytes de la pista.
 * @param pista Puntero a la secuencia de bytes a buscar.
 * @param rutaArchivoModificado Ruta donde guardar el archivo descomprimido (opcional, puede ser nullptr).
 * @return true si encuentra los parametros, false en caso contrario.
 */
bool BuscarParametros(unsigned char* data, int& nbits, int& claveK, int sizeEncriptado,
                      int sizePista, unsigned char* pista, const char* rutaArchivoModificado) {

    // Validacion de entrada
    if (data == nullptr || pista == nullptr) {
        cout << "Error: datos nulos" << endl;
        return false;
    }

    // Verificar que el tamaño sea multiplo de 3
    if (sizeEncriptado % 3 != 0) {
        cout << "Advertencia: El tamaño no es multiplo de 3 (formato de terna)" << endl;
    }

    // Mostrar la pista que estamos buscando
    cout << "Pista a buscar: ";
    mostrarContenido(pista, sizePista);
    cout << "Iniciando busqueda de parametros..." << endl;

    int intentos = 0;

    // Busqueda exhaustiva con limites
    for (int clave = 0; clave < 255 ; clave++) {
        for (int bits = 0; bits <= 7 ; bits++) {
            intentos++;

            // Mostrar progreso cada 200 intentos para no saturar la consola
            if (intentos % 100 == 0) {
                cout << "Progreso: " << intentos << "/"
                     << " (Clave: " << clave << ", Bits: " << bits << ")" << endl;
            }

            // Probar descompresion directamente con los parametros actuales
            // No necesitamos copiar ni modificar los datos originales
            if (ProbarDescompresion(data, sizeEncriptado, pista, sizePista,
                                    (unsigned char)clave, bits, rutaArchivoModificado)) {
                cout << "Parametros encontrados!" << endl;
                cout << "Total de intentos realizados: " << intentos << endl;

                // Guardar los parametros encontrados
                nbits = bits;
                claveK = clave;
                return true;
            }
        }
    }

    // Si llegamos aqui, no se encontraron parametros validos
    cout << "No se encontraron parametros validos despues de " << intentos << " intentos." << endl;
    cout << "Posibles causas:" << endl;
    cout << "- Los datos no estan en formato de ternas" << endl;
    cout << "- La pista no existe en el texto descomprimido" << endl;
    cout << "- Los algoritmos de descompresion necesitan ajustes" << endl;

    return false;
}
