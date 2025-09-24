#include <iostream>

using namespace std;
unsigned char* descompresionLZ78(unsigned char* data, int size) {
    cout << "Dentro de funcion descompresion:\n";

    int* longitudes = new int[size];  // arreglo dinámico para longitudes
    int tamDic = 0;                   // tamaño lógico del diccionario
    int total = 0;                    // tamaño descomprimido
    int numero = 0;                   // acumulador para el índice

    for (int i = 0; i < size; i++) {
        unsigned char c = data[i];

        if (c >= 'a' && c <= 'z') {
            // calculamos longitud de la nueva cadena
            int len;
            if (numero == 0) {
                len = 1; // cadena nueva de una sola letra
            } else {
                len = longitudes[numero] + 1;
            }

            // guardamos en diccionario
            longitudes[++tamDic] = len;

            // sumamos al total descomprimido
            total += len;

            cout << "Par (" << numero << ", " << c << ") -> longitud = "
                 << len << "  total = " << total << endl;

            numero = 0; // reiniciar acumulador
        } else {
            // reconstruir numero en base 256
            numero = numero * 256 + (int)c;
        }
    }

    cout << "Tamaño total descomprimido: " << total << endl;

    delete[] longitudes;  // liberar memoria
    return nullptr;
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
unsigned char* descompresionRLE(unsigned char* entrada){
    int capacidad = 100; //la capcidad inicial de la salida, esta podra ir variando dependiendo si se necesite
    unsigned char* arregloDescomprimido = new unsigned char[capacidad];
    int pos = 0; //posicion inicial del arreglo
    int n = 0; //acumulador

    for(int i = 0; entrada[i] != '\0'; i++){
        char c = entrada[i];
        if(c >= '0' && c <= '9'){
            n = n * 10 +(c - '0');
        }
        else{
            //en caso de acabarse la capacidad de almacenamiento esta se actualizara
            while(pos + n >= capacidad){
                capacidad *= 2;
                unsigned char* temp = new unsigned char[capacidad];
                for (int k = 0; k < pos; k++) temp[k] = arregloDescomprimido[k];
                delete[] arregloDescomprimido;
                arregloDescomprimido = temp;
            }
            // agregar el carácter repetido n-veces
            for (int j = 0; j < n; j++) {
                arregloDescomprimido[pos++] = c;
            }

        n = 0; // reiniciar para el próximo bloque
        }
    }

    arregloDescomprimido[pos] = '\0';
    return arregloDescomprimido;
}






