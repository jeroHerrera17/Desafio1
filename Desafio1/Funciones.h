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
bool crearArchivoConTexto(const char* rutaArchivo, unsigned char* texto, int size);

/**
 * @brief Lee un archivo y devuelve su contenido como un arreglo dinámico.
 *
 * @param rutaArchivo Ruta del archivo.
 * @param size Referencia donde se almacenará el tamaño leído.
 * @return char* Puntero al contenido del archivo (se debe liberar con delete[]).
 */
unsigned char* leerArchivoACharArray(const char* rutaArchivo, int& size);

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
 * @brief   Descomprime una secuencia codificada con el algoritmo LZ78.
 *
 * Esta función recibe un arreglo de bytes que representa la compresión
 * de un texto bajo el esquema LZ78, y reconstruye la cadena original.
 *
 * El formato esperado en `data` es una secuencia de pares (número, letra),
 * donde el número referencia a una entrada previa del diccionario y la
 * letra es el nuevo carácter a concatenar. La numeración empieza en 1
 * (entrada 0 indica que el carácter no depende de ninguna cadena previa).
 *
 * Ejemplo de entrada:
 * ```
 * 0a 0b 1c ...
 * ```
 *
 * El algoritmo realiza dos pasadas:
 *  - **Primera pasada:** Calcula el tamaño total del resultado para
 *    reservar memoria.
 *  - **Segunda pasada:** Reconstruye cada cadena usando el diccionario
 *    dinámico y las copia al buffer de salida.
 *
 * @param data   Puntero al arreglo de datos comprimidos (entrada).
 * @param size   Cantidad de bytes en el arreglo `data`.
 * @param total  Referencia donde se almacena el tamaño total del texto
 *               descomprimido (longitud del resultado sin incluir `\0`).
 *
 * @return Un puntero a un arreglo dinámico de caracteres (unsigned char*)
 *         que contiene el texto descomprimido terminado en `\0`.
 *         El llamador es responsable de liberar esta memoria con `delete[]`.
 * @note
 * - El diccionario se maneja con índices crecientes.
 * - La función imprime mensajes de depuración por consola (`cout` y `cerr`).
 * - Si la reconstrucción genera menos caracteres de los esperados,
 *   se muestra una advertencia.
 */
unsigned char* descompresionLZ78(unsigned char* data,int size,int& total);

/**
 * @brief descomprime un arreglo previamente comprimido usando elalgoritmo RLE (Run-Length Encoding).
 *
 * este algoritmo multiplica un caracter A de una secuencia de estos por la cantidad n que sea
 * mencionada en el mismo.
 * Ejemplo: "4A3B2C1D2A" -> "AAAABBBCCDAA".
 *
 * @param entrada puntero a un arreglo de caracteres previamente comprimido.
 * @param size cantidad de caracteres del arreglo de entrada.
 * @return total Tamaño total del arreglo descomprimido.
*/
unsigned char* descompresionRLE(unsigned char* entrada,int size, int& total);

/**
 * @brief Muestra el contenido de un arreglo de bytes como texto imprimible.
 *
 * Esta función recorre un arreglo de bytes y muestra en consola
 * los caracteres ASCII que son letras como char y el resto de caracteres como enteros
 * Los caracteres no imprimibles son omitidos.
 *
 * @param contenido Puntero al arreglo de bytes a mostrar.
 * @param size Número de bytes en el arreglo.
 */
void mostrarContenido( unsigned char* contenido, int size);
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
void BuscarParametros(unsigned char* data,int&nbits,int&claveK,int sizeEncriptado,int sizePista,unsigned char*pista);
#endif // FUNCIONES_H
