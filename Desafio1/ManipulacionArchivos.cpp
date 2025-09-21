#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Crea un archivo y escribe un texto dentro de él.
 *
 * Abre (o crea) un archivo en modo binario y escribe en él el contenido
 * de un arreglo de bytes. Si el archivo no puede abrirse o crearse,
 * muestra un mensaje de error y devuelve false.
 *
 * @param rutaArchivo Ruta del archivo a crear o sobrescribir.
 * @param texto Puntero al arreglo de bytes a escribir.
 * @param size Número de bytes a escribir en el archivo.
 * @return true si el archivo se creó y escribió correctamente, false en caso de error.
 */
bool crearArchivoConTexto(const string& rutaArchivo, unsigned char* texto, int size) {
    ofstream archivo(rutaArchivo, ios::binary);
    if (!archivo.is_open()) {
        cerr << "No se pudo crear o abrir el archivo." << endl;
        return false;
    }

    // Escribir el texto en el archivo
    archivo.write(reinterpret_cast<const char*>(texto), size);
    archivo.close();

    return true;
}

/**
 * @brief Lee un archivo y devuelve su contenido como un arreglo dinámico.
 *
 * Abre un archivo en modo binario, determina su tamaño y lo carga
 * completamente en memoria. El resultado se devuelve como un puntero
 * a un arreglo dinámico de bytes (unsigned char).
 *
 * El arreglo devuelto incluye un carácter nulo (`'\0'`) adicional al final
 * para permitir su impresión directa como string. El usuario es responsable
 * de liberar la memoria con `delete[]`.
 *
 * @param rutaArchivo Ruta del archivo a leer.
 * @param size Referencia donde se almacenará el tamaño del archivo leído.
 * @return unsigned char* Puntero al contenido del archivo, o nullptr si ocurre un error.
 */
unsigned char* leerArchivoACharArray(const string& rutaArchivo, int& size) {
    ifstream archivo(rutaArchivo, ios::binary);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        size = 0;
        return nullptr;
    }

    // Contar caracteres
    archivo.seekg(0, ios::end);
    size = static_cast<int>(archivo.tellg());
    archivo.seekg(0, ios::beg);

    // Reservar memoria para arreglo (+1 para terminador nulo)
    unsigned char* buffer = new unsigned char[size + 1];

    // Leer todo el archivo en el buffer
    archivo.read(reinterpret_cast<char*>(buffer), size);

    // IMPORTANTE: Agregar terminador nulo para poder imprimir como string
    buffer[size] = '\0';

    archivo.close();
    return buffer;
}
