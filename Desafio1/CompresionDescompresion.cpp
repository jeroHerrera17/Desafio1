#include <iostream>
using namespace std;

unsigned char* descompresionLZ78(unsigned char* data, int size, int& total) {
    //cout << "Iniciando descompresión LZ78 con formato de ternas..." << endl;

    if (size % 3 != 0) {
        cerr << "Error: El tamaño de datos no es múltiplo de 3 (formato de terna)" << endl;
        total = 0;
        return nullptr;
    }

    int numTernas = size / 3;

    unsigned char** diccionario = new unsigned char*[numTernas + 1];
    int* longitudes = new int[numTernas + 1];

    diccionario[0] = new unsigned char[1];
    diccionario[0][0] = '\0';
    longitudes[0] = 0;

    int posDic = 0;
    total = 0;

    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int numero = (int(data[pos]) << 8) | int(data[pos + 1]);
        unsigned char caracter = data[pos + 2];

        if (caracter >= 'a' && caracter <= 'z') {
            int len;
            if (numero == 0 || numero > posDic) {
                len = 1;
            } else {
                len = longitudes[numero] + 1;
            }

            posDic++;
            longitudes[posDic] = len;
            total += len;
        }
    }

    //cout << "Tamaño total calculado: " << total << endl;

    if (total == 0) {
        //cerr << "No se encontraron caracteres validos para descomprimir" << endl;
        delete[] diccionario[0];
        delete[] diccionario;
        delete[] longitudes;
        return nullptr;
    }

    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos_resultado = 0;

    posDic = 0;

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
                descomprimido[pos_resultado] = diccionario[posDic][j];
                pos_resultado++;
            }
        }
    }

    for (int i = 0; i <= posDic; i++) {
        delete[] diccionario[i];
    }
    delete[] diccionario;
    delete[] longitudes;

    descomprimido[total] = '\0';

    //cout << "Descompresion LZ78 completada. Bytes generados: " << pos_resultado << endl;
    return descomprimido;
}

unsigned char* descompresionRLE(unsigned char* entrada, int size, int& total) {
    //cout << "Iniciando descompresión RLE con formato de ternas..." << endl;

    if (size % 3 != 0) {
        cerr << "Error: El tamaño de datos no es múltiplo de 3 (formato de terna)" << endl;
        total = 0;
        return nullptr;
    }

    int numTernas = size / 3;
    //cout << "Número de ternas a procesar: " << numTernas << endl;

    total = 0;
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int repeticiones = int(entrada[pos + 1]);
        unsigned char caracter = entrada[pos + 2];

        if (caracter >= 'a' && caracter <= 'z' && repeticiones > 0 && repeticiones < 256) {
            total += repeticiones;
        }
    }

    //cout << "Tamaño total calculado: " << total << endl;

    if (total == 0) {
        // cerr << "No se encontraron caracteres validos para descomprimir" << endl;
        return nullptr;
    }

    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos_resultado = 0;

    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;
        int repeticiones = int(entrada[pos + 1]);
        unsigned char caracter = entrada[pos + 2];

        if (caracter >= 'a' && caracter <= 'z' && repeticiones > 0 && repeticiones < 256) {
            for (int j = 0; j < repeticiones && pos_resultado < total; j++) {
                descomprimido[pos_resultado] = caracter;
                pos_resultado++;
            }
        }
    }

    descomprimido[total] = '\0';

    //cout << "Descompresion RLE completada. Bytes generados: " << pos_resultado << endl;
    return descomprimido;
}
