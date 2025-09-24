#include <iostream>

using namespace std;
unsigned char* descompresionLZ78(unsigned char* data, int size,int& total) {
    cout << "Dentro de funcion descompresion:\n";

    unsigned char** diccionario = new unsigned char*[size];
    int* longitudes = new int[size];
    int tamDic = 0;
    total = 0;
    int numero = 0;

    // Primer pasada: calcular tamaño total
    for (int i = 0; i < size; i++) {
        unsigned char c = data[i];

        if (c >= 'a' && c <= 'z') {
            int len = (numero == 0) ? 1 : longitudes[numero] + 1;

            longitudes[++tamDic] = len;
            total += len;

            cout << "Par (" << numero << ", " << c << ") -> longitud = "
                 << len << "  total = " << total << endl;

            numero = 0;
        } else {
            numero = numero * 256 + (int)c;
        }
    }

    cout << "Tamaño total descomprimido: " << total << endl;

    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos = 0;

    // Segunda pasada: reconstruir cadenas
    tamDic = 0;
    numero = 0;
    for (int i = 0; i < size; i++) {
        unsigned char c = data[i];

        if (c >= 'a' && c <= 'z') {
            unsigned char* nuevaCadena;
            int len;

            if (numero == 0) {
                len = 1;
                nuevaCadena = new unsigned char[1];
                nuevaCadena[0] = c;
            } else {
                len = longitudes[numero] + 1;
                nuevaCadena = new unsigned char[len];
                for (int j = 0; j < longitudes[numero]; j++) {
                    nuevaCadena[j] = diccionario[numero][j];
                }
                nuevaCadena[len - 1] = c;
            }

            diccionario[++tamDic] = nuevaCadena;

            // Copia manual al buffer de salida
            for (int j = 0; j < len; j++) {
                descomprimido[pos + j] = nuevaCadena[j];
            }
            pos += len;

            numero = 0;
        } else {
            numero = numero * 256 + (int)c;
        }
    }

    // Seguridad: verificar que no se salió de rango
    if (pos != total) {
        cerr << "Advertencia: pos=" << pos << " pero total=" << total << endl;
    }

    // Liberar memoria del diccionario
    for (int i = 1; i <= tamDic; i++) {
        delete[] diccionario[i];
    }
    delete[] diccionario;
    delete[] longitudes;

    // Agregar terminador de cadena
    descomprimido[total] = '\0';

    return descomprimido;
}

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
unsigned char* descompresionRLE(unsigned char* entrada,int size, int& total){

    // Calcular la longitud aproximada de la cadena descomprimida
    int longitudFinal = 0;
    int numero = 0;
    for (int i = 0; i < size; i ++) {
        if (int(entrada[i]) >= 0 && int(entrada[i]) <= 9){
            numero = numero * 10 + (int(entrada[i]));
        }
        else {
            longitudFinal += numero;
            numero = 0;
        }
    }
    //Crear arreglo dinámico con la longitud suficiente (+1 para '\0')
    unsigned char* descomprimido = new unsigned char[longitudFinal + 1];
    //realizar la descompresión
    int pos = 0;
    int repeticiones = 0;
    for (int i = 0; i < size; i ++) {
        if (int(entrada[i]) >= 0 && int(entrada[i]) <= 9){
            repeticiones = repeticiones * 10 + (int(entrada[i]));
        }
        else{
            for (int j = 0; j < repeticiones; j++){
                descomprimido[pos] = entrada[i];
                pos++;
            }
            repeticiones = 0;
        }
    }
    //Terminar la cadena con '\0'
    descomprimido[pos] = '\0';
    total = longitudFinal;

    return descomprimido;
}





