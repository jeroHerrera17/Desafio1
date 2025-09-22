#include <iostream>
#include <fstream>
#include <iomanip>
#include "Funciones.h"
using namespace std;

/**
 * @brief Muestra el contenido de un arreglo de bytes como texto imprimible.
 *
 * Esta función recorre un arreglo de bytes y muestra en consola
 * los caracteres ASCII que son letras como char y el resto de caracteres como enteros
 * Los caracteres no imprimibles son omitidos.
 *
 * @param titulo Texto que se mostrará como encabezado antes del contenido.
 * @param contenido Puntero al arreglo de bytes a mostrar.
 * @param size Número de bytes en el arreglo.
 */
void mostrarContenido(const string& titulo, unsigned char* contenido, int size) {
    cout << titulo << endl;

    for (int i = 0; i < size; ++i) {
        if ((contenido[i] >= 'a'&& contenido[i] <= 'z')||(contenido[i] >= 'A'&& contenido[i] <= 'Z') ) {
            cout << contenido[i];  // solo se imprimen caracteres que son letras
        } else {
          cout<<(int)contenido[i];  //caracteres diferentes de letras se imprimen como enteros
        }
    }
    cout << endl << endl;
}

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
    string rutaArchivo = "../../Datos/Encriptado2.txt"; ///< Ruta relativa al archivo a leer
    int size = 0;

    // Leer el archivo en memoria dinámica
    unsigned char* encriptacion = leerArchivoACharArray(rutaArchivo, size);

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

        // Liberar memoria asignada
        delete[] encriptacion;
    } else {
        cerr << "Error al leer el archivo." << endl;
    }

    return 0;
}
