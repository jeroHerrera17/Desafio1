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
 * @brief Descomprime una secuencia codificada con el algoritmo LZ78 con desencriptado integrado.
 *
 * Esta función recibe un arreglo de bytes encriptados, aplica la desencriptación
 * (XOR + rotación de bits) y luego descomprime usando el algoritmo LZ78.
 *
 * El formato esperado después del desencriptado es una secuencia de ternas (número, número, letra),
 * donde los dos primeros bytes forman un número que referencia a una entrada previa del diccionario
 * y el tercer byte es el nuevo carácter a concatenar. La numeración empieza en 1
 * (entrada 0 indica que el carácter no depende de ninguna cadena previa).
 *
 * El algoritmo realiza dos pasadas:
 *  - **Primera pasada:** Desencripta, valida datos y calcula el tamaño total del resultado.
 *  - **Segunda pasada:** Reconstruye cada cadena usando el diccionario dinámico.
 *
 * @param data Puntero al arreglo de datos encriptados y comprimidos.
 * @param size Cantidad de bytes en el arreglo `data`.
 * @param claveXOR Clave XOR para desencriptar.
 * @param rotacionBits Número de bits para rotar a la derecha (0-7).
 * @param total Referencia donde se almacena el tamaño total del texto descomprimido.
 * @param esValido Referencia donde se indica si la descompresión fue válida.
 * @return Un puntero a un arreglo dinámico de caracteres (unsigned char*)
 *         que contiene el texto descomprimido terminado en `\0`.
 *         El llamador es responsable de liberar esta memoria con `delete[]`.
 * @note
 * - Incluye validación temprana para detectar combinaciones incorrectas de parámetros.
 * - Si las referencias del diccionario están muy fuera de rango, retorna nullptr.
 */
unsigned char* descompresionLZ78(unsigned char* data, int size, unsigned char claveXOR,
                                 int rotacionBits, int& total, bool& esValido);

/**
 * @brief Descomprime un arreglo previamente comprimido usando el algoritmo RLE con desencriptado integrado.
 *
 * Esta función recibe un arreglo de bytes encriptados, aplica la desencriptación
 * (XOR + rotación de bits) y luego descomprime usando Run-Length Encoding.
 *
 * El algoritmo interpreta el formato de ternas después del desencriptado:
 * - Primer byte: no utilizado en RLE
 * - Segundo byte: número de repeticiones
 * - Tercer byte: carácter a repetir
 *
 * Ejemplo después del desencriptado: ternas que representan "4A3B2C1D2A" -> "AAAABBBCCDAA".
 *
 * @param entrada Puntero al arreglo de datos encriptados y comprimidos.
 * @param size Cantidad de caracteres del arreglo de entrada.
 * @param claveXOR Clave XOR para desencriptar.
 * @param rotacionBits Número de bits para rotar a la derecha (0-7).
 * @param total Referencia donde se almacena el tamaño total del arreglo descomprimido.
 * @param esValido Referencia donde se indica si la descompresión fue válida.
 * @return Puntero al texto descomprimido (dinámico, debe liberarse con `delete[]`).
 * @note
 * - Incluye validación: al menos 30% de los caracteres deben ser letras válidas.
 * - Si muy pocos caracteres son válidos, retorna nullptr.
 */
unsigned char* descompresionRLE(unsigned char* entrada, int size, unsigned char claveXOR,
                                int rotacionBits, int& total, bool& esValido);

/**
 * @brief Muestra el contenido de un arreglo de bytes como texto imprimible.
 *
 * Esta función recorre un arreglo de bytes y muestra en consola
 * los caracteres ASCII que son letras como char y el resto de caracteres como enteros.
 * Los caracteres no imprimibles son omitidos.
 *
 * @param contenido Puntero al arreglo de bytes a mostrar.
 * @param size Número de bytes en el arreglo.
 */
void mostrarContenido(unsigned char* contenido, int size);

/**
 * @brief Función para buscar una secuencia dentro de otra usando arreglos.
 *
 * Busca si la secuencia `pista` está contenida dentro de `texto` utilizando
 * búsqueda secuencial con comparación byte a byte.
 *
 * @param texto Puntero al arreglo donde buscar.
 * @param sizeTexto Tamaño del arreglo de texto.
 * @param pista Puntero a la secuencia a buscar.
 * @param sizePista Tamaño de la secuencia a buscar.
 * @return true si encuentra la secuencia, false en caso contrario.
 */
bool buscarSecuencia(unsigned char* texto, int sizeTexto, unsigned char* pista, int sizePista);

/**
 * @brief Intenta descomprimir los datos usando diferentes algoritmos de compresión
 *        con parámetros de desencriptación integrados y busca la pista en el resultado.
 *
 * Esta función aplica desencriptación (XOR + rotación) y descompresión de manera integrada,
 * probando tanto RLE como LZ78. Si encuentra la pista en algún resultado,
 * opcionalmente guarda el archivo descomprimido.
 *
 * @param data Puntero al arreglo de datos encriptados y comprimidos.
 * @param size Tamaño en bytes del arreglo de datos.
 * @param pista Puntero al arreglo con la secuencia a buscar.
 * @param sizePista Tamaño en bytes de la pista.
 * @param claveXOR Clave XOR para desencriptar.
 * @param rotacionBits Número de bits para rotar a la derecha (0-7).
 * @param rutaArchivoModificado Ruta donde guardar el resultado exitoso (opcional, puede ser nullptr).
 * @return true si la pista es encontrada en alguna de las descompresiones, false en caso contrario.
 */
bool ProbarDescompresion(unsigned char* data, int size, unsigned char* pista, int sizePista,
                         unsigned char claveXOR, int rotacionBits, const char* rutaArchivoModificado);

/**
 * @brief Busca los parámetros correctos de desencriptación (clave XOR y bits de rotación).
 *
 * Esta función prueba todas las combinaciones posibles de:
 * - Claves XOR (0 a 254)
 * - Rotaciones de bits (0 a 7)
 *
 * Para cada combinación, intenta descomprimir directamente los datos con los parámetros
 * integrados y busca la pista en el resultado. Utiliza las versiones optimizadas de
 * las funciones de descompresión que incluyen validación temprana.
 *
 * Si encuentra coincidencia, guarda los parámetros en `nbits` y `claveK` y
 * opcionalmente guarda el archivo descomprimido.
 *
 * @param data Puntero a los datos encriptados y comprimidos.
 * @param nbits Variable de salida donde se almacenará la cantidad de bits de rotación encontrada.
 * @param claveK Variable de salida donde se almacenará la clave XOR encontrada.
 * @param sizeEncriptado Tamaño en bytes del arreglo de datos encriptados.
 * @param sizePista Tamaño en bytes de la pista.
 * @param pista Puntero a la secuencia de bytes a buscar.
 * @param rutaArchivoModificado Ruta donde guardar el archivo descomprimido (opcional, puede ser nullptr).
 * @return true si encuentra los parámetros correctos, false en caso contrario.
 */
bool BuscarParametros(unsigned char* data, int& nbits, int& claveK, int sizeEncriptado,
                      int sizePista, unsigned char* pista, const char* rutaArchivoModificado);

#endif // FUNCIONES_H
