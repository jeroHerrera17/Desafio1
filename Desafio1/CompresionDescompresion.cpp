
#include <iostream>
    using namespace std;

/**
 * @brief Descompresión con algoritmo LZ78 usando formato de ternas.
 *
 * Cada terna ocupa 3 bytes:
 *   [byte1][byte2][caracter]
 * donde (byte1 << 8) | byte2 es el índice en el diccionario.
 *
 * @param data  Datos comprimidos.
 * @param size  Tamaño en bytes de los datos.
 * @param total Variable de salida: tamaño del texto descomprimido.
 * @return Puntero al arreglo descomprimido (terminado en '\0') o nullptr en caso de error.
 */
unsigned char* descompresionLZ78(unsigned char* data, int size, int& total) {
    if (size % 3 != 0) {
        cerr << "Error: El tamaño de datos no es múltiplo de 3 (formato de terna)" << endl;
        total = 0;
        return nullptr;
    }

    int numTernas = size / 3;

    // Diccionario dinámico
    unsigned char** diccionario = new unsigned char*[numTernas + 1];
    int* longitudes = new int[numTernas + 1];

    diccionario[0] = new unsigned char[1];
    diccionario[0][0] = '\0';
    longitudes[0] = 0;

    int posDic = 0;
    total = 0;

    // Calcular tamaño total descomprimido
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int numero = (int(data[pos]) << 8) | int(data[pos + 1]);
        unsigned char caracter = data[pos + 2];

        if (caracter >= 'a' && caracter <= 'z') {
            int len = (numero == 0 || numero > posDic) ? 1 : longitudes[numero] + 1;
            posDic++;
            longitudes[posDic] = len;
            total += len;
        }
    }

    if (total == 0) {
        delete[] diccionario[0];
        delete[] diccionario;
        delete[] longitudes;
        return nullptr;
    }

    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos_resultado = 0;
    posDic = 0;

    // Reconstrucción
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int numero = (int(data[pos]) << 8) | int(data[pos + 1]);
        unsigned char caracter = data[pos + 2];

        if (caracter >= 'a' && caracter <= 'z') {
            posDic++;
            int len;

            if (numero == 0 || numero > posDic - 1) {
                len = 1;
                diccionario[posDic] = new unsigned char[len + 1];
                diccionario[posDic][0] = caracter;
                diccionario[posDic][len] = '\0';
            } else {
                len = longitudes[numero] + 1;
                diccionario[posDic] = new unsigned char[len + 1];

                for (int j = 0; j < longitudes[numero]; j++) {
                    diccionario[posDic][j] = diccionario[numero][j];
                }
                diccionario[posDic][len - 1] = caracter;
                diccionario[posDic][len] = '\0';
            }

            longitudes[posDic] = len;

            for (int j = 0; j < len && pos_resultado < total; j++) {
                descomprimido[pos_resultado++] = diccionario[posDic][j];
            }
        }
    }

    // Liberar memoria auxiliar
    for (int i = 0; i <= posDic; i++) {
        delete[] diccionario[i];
    }
    delete[] diccionario;
    delete[] longitudes;

    descomprimido[total] = '\0';
    return descomprimido;
}

/**
 * @brief Descompresión con algoritmo RLE usando formato de ternas.
 *
 * Cada terna ocupa 3 bytes:
 *   [unused][repeticiones][caracter]
 *
 * @param entrada Datos comprimidos.
 * @param size    Tamaño en bytes de los datos.
 * @param total   Variable de salida: tamaño del texto descomprimido.
 * @return Puntero al arreglo descomprimido (terminado en '\0') o nullptr en caso de error.
 */
unsigned char* descompresionRLE(unsigned char* entrada, int size, int& total) {
    if (size % 3 != 0) {
        cerr << "Error: El tamaño de datos no es múltiplo de 3 (formato de terna)" << endl;
        total = 0;
        return nullptr;
    }

    int numTernas = size / 3;
    total = 0;

    // Calcular tamaño total
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int repeticiones = int(entrada[pos + 1]);
        unsigned char caracter = entrada[pos + 2];

        if (caracter >= 'a' && caracter <= 'z' && repeticiones > 0 && repeticiones < 256) {
            total += repeticiones;
        }
    }

    if (total == 0) {
        return nullptr;
    }

    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos_resultado = 0;

    // Reconstrucción
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int repeticiones = int(entrada[pos + 1]);
        unsigned char caracter = entrada[pos + 2];

        if (caracter >= 'a' && caracter <= 'z' && repeticiones > 0 && repeticiones < 256) {
            for (int j = 0; j < repeticiones && pos_resultado < total; j++) {
                descomprimido[pos_resultado++] = caracter;
            }
        }
    }

    descomprimido[total] = '\0';
    return descomprimido;
}

