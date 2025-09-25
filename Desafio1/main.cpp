#include <iostream>
#include <fstream>
#include <iomanip>
#include "Funciones.h"

using namespace std;


/**
 * @brief Punto de entrada principal del programa.
 *
 * Este programa realiza el siguiente flujo:
 * 1. Lee un archivo en memoria dinámica.
 * 2. Muestra el contenido original en consola.
 * 3. Aplica una operación XOR con una clave fija (0x5A).
 * 4. Aplica una rotación de bits a la derecha sobre cada byte.
 * 5. Muestra en consola el resultado después de cada transformación.
 *
 * @return int Código de salida (0 si termina correctamente).
 */
int main() {
    string rutaBase = "../../Datos/";
        ///< Ruta relativa al archivo a leer
    int sizeEncriptado = 0;
    int sizePista=0;
    //Valor n de rotacion
    int nBits=0;
    int claveK=0;
    // Leer el archivo en memoria dinámica
    unsigned char* encriptacion = leerArchivoACharArray(rutaBase+"encriptado4.txt", sizeEncriptado);
    unsigned char* pista=leerArchivoACharArray(rutaBase+"pista4.txt", sizePista);
    BuscarParametros(encriptacion,nBits,claveK,sizeEncriptado,sizePista,pista);

    /*
    if (encriptacion != nullptr) {
        cout << "El archivo tiene " << size << " caracteres." << endl << endl;

        // Mostrar contenido original
        mostrarContenido("Contenido original:", encriptacion, size);

        // Definir clave XOR
        unsigned char clave = 0x5A;

        // Aplicar XOR sobre el contenido en memoria
        DoXOR(encriptacion, clave, size);
        mostrarContenido("Contenido despues de XOR:", encriptacion, size);

        // Rotar bits a la derecha (ejemplo: 3 posiciones)
        RotarDerecha(size, encriptacion, 3);
        mostrarContenido("Contenido despues de rotacion:", encriptacion, size);
        int total;
        //unsigned char* descomprimido=descompresionLZ78(encriptacion, size,total);
        //mostrarContenido("Contenido despues descompresión", descomprimido,total);
        // Liberar memoria asignada
        delete[] encriptacion;
    } else {
        cerr << "Error al leer el archivo." << endl;
    }
    */
    return 0;
}
