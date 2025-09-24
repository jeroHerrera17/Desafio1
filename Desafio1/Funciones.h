#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
using std::string;

/**
 * @brief Crea un archivo y escribe un texto dentro de él.
 *
 * @param rutaArchivo Ruta del archivo.
 * @param texto Puntero al texto a escribir.
 * @param size Número de bytes a escribir.
 * @return true si se creó/escribió correctamente, false en caso contrario.
 */
bool crearArchivoConTexto(const string& rutaArchivo, unsigned char* texto, int size);

/**
 * @brief Lee un archivo y devuelve su contenido como un arreglo dinámico.
 *
 * @param rutaArchivo Ruta del archivo.
 * @param size Referencia donde se almacenará el tamaño leído.
 * @return char* Puntero al contenido del archivo (se debe liberar con delete[]).
 */
unsigned char* leerArchivoACharArray(const string& rutaArchivo, int& size);

/**
 * @brief Aplica una operación XOR sobre un arreglo de texto.
 *
 * @param texto Puntero al arreglo de bytes del texto.
 * @param clave Byte usado como clave para el XOR.
 * @param tamaño Número de bytes en el arreglo.
 */
void DoXOR(unsigned char* texto, unsigned char  clave, int tamaño);

/**
 * @brief Rota los bits de cada byte de un texto hacia la derecha.
 *
 * @param tamaño Número de bytes en el texto.
 * @param texto Puntero al arreglo de bytes del texto.
 * @param n Número de posiciones a rotar (0-7).
 */
void RotarDerecha(int tamaño, unsigned char* texto, int n);

/**
 * @brief Rota los bits de cada byte de un texto hacia la izquierda.
 *
 * @param tamaño Número de bytes en el texto.
 * @param texto Puntero al arreglo de bytes del texto.
 * @param n Número de posiciones a rotar (0-7).
 */
void RotarIzquierda(int tamaño, unsigned char* texto, int n);
/**
 * @brief Comprime un arreglo de caracteres usando el algoritmo RLE (Run-Length Encoding).
 *
 * Este algoritmo reemplaza secuencias consecutivas de caracteres repetidos por
 * el número de repeticiones seguido del carácter. Por ejemplo:
 * - Entrada: "AAAABBBCCDAA"
 * - Salida:  "4A3B2C1D2A"
 *
 * @param arreglo Puntero a un arreglo de caracteres sin comprimir.
 * @param longitud Cantidad de caracteres en el arreglo de entrada.
 * @return int Siempre devuelve 0 en esta versión (se podría cambiar para retornar la longitud comprimida).
 */
int compresionRlE(unsigned char* arreglo, int longitud);
/**
 * @brief descompresionLZ78
 * @param data
 * @param size
 * @param total
 * @return
 */
unsigned char* descompresionLZ78(unsigned char* data,int size,int& total);

/**
 * @brief descomprime un arreglo previamente comprimido usando elalgoritmo RLE (Run-Length Encoding).
 *
 * este algoritmo multiplica un caracter A de una secuencia de estos por la cantidad n que sea
 * mencionada en el mismo.
 * Ejemplo: "4A3B2C1D2A" -> "AAAABBBCCDAA".
 *
 * @param arreglo puntero a un arreglo de caracteres previamente comprimido.
 * @param longitud cantidad de caracteres del arreglo de entrada.
 * @return unsigned char* Puntero al texto descomprimido (dinámico, debe liberarse con delete[]).
*/
unsigned char* descompresionRLE(unsigned char* entrada);

#endif // FUNCIONES_H
