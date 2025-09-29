
/**
 * @file main.cpp
 * @brief Programa principal para el procesamiento de archivos encriptados con pistas.
 *
 * Este programa lee múltiples archivos encriptados y sus correspondientes archivos
 * de pista, calcula los parámetros necesarios para su modificación/descifrado,
 * y genera archivos de salida con los resultados.
 *
 * El flujo principal es:
 * 1. Construcción dinámica de las rutas de entrada/salida.
 * 2. Lectura de los archivos encriptados y de pistas en memoria.
 * 3. Llamado a la función de procesamiento (@ref BuscarParametros).
 * 4. Escritura del archivo modificado.
 * 5. Liberación de memoria.
 */

#include <iostream>

#include "funciones.h"

    using namespace std;

/**
 * @brief Punto de entrada principal del programa.
 *
 * Procesa secuencialmente un conjunto de archivos encriptados y sus pistas asociadas.
 * Para cada archivo:
 * - Construye las rutas de entrada y salida.
 * - Lee en memoria los datos encriptados y la pista.
 * - Llama a @ref BuscarParametros para generar un archivo modificado.
 * - Libera la memoria reservada.
 *
 * @return int Código de retorno estándar (0 si la ejecución fue correcta).
 *
 * @var sizeEncriptado Tamaño en bytes del archivo encriptado leído en memoria.
 * @var sizePista      Tamaño en bytes del archivo de pista leído en memoria.
 * @var nBits          Número de bits deducido del análisis de los archivos.
 * @var claveK         Clave encontrada a partir de la pista y del encriptado.
 * @var nArchivos      Número total de archivos a procesar.
 * @var archivoEncriptado Ruta base del archivo encriptado (se modifica en cada iteración).
 * @var archivoPista      Ruta base del archivo de pista (se modifica en cada iteración).
 * @var archivoModificado Ruta base del archivo de salida modificado (se modifica en cada iteración).
 */
int main() {
    int sizeEncriptado = 0;
    int sizePista = 0;
    int nBits = 0;
    int claveK = 0;
    int nArchivos;
    cout<<"Ingrese la cantidad de archivos a procesar: "<<endl;
    cin>>nArchivos;
    char archivoEncriptado[] = "../../Datos/Encriptado0.txt";
    char archivoPista[]      = "../../Datos/pista0.txt";
    char archivoModificado[] = "../../Datos/modificado0.txt";

    for (int i = 1; i <= nArchivos; i++) {
        archivoEncriptado[sizeof("../../Datos/Encriptado") - 1] = '0' + i;
        archivoPista[sizeof("../../Datos/pista") - 1] = '0' + i;
        archivoModificado[sizeof("../../Datos/modificado") - 1] = '0' + i;

        cout << "=== Procesando archivo " << i << " ===" << endl;
        cout << "Archivo encriptado: " << archivoEncriptado << endl;
        cout << "Archivo pista     : " << archivoPista << endl;
        cout << "Archivo salida    : " << archivoModificado << endl;

        unsigned char* enc = leerArchivoACharArray(archivoEncriptado, sizeEncriptado);
        unsigned char* pista = leerArchivoACharArray(archivoPista, sizePista);

        if (enc != nullptr && pista != nullptr) {
            BuscarParametros(enc, nBits, claveK, sizeEncriptado, sizePista, pista, archivoModificado);

            delete[] enc;
            delete[] pista;
        } else {
            cout << "Error al leer los archivos para el indice " << i << endl;
        }

        cout << "=== Fin procesamiento archivo " << i << " ===" << endl << endl;
    }

    return 0;
}

