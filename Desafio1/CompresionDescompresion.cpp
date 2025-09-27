#include <iostream>

using namespace std;


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

unsigned char* descompresionLZ78(unsigned char* data, int size, int& total) {
    cout << "Dentro de funcion descompresion LZ78:\n";


    //Calculo de tamaño descomprimido

    // Diccionario para almacenar las cadenas
    unsigned char** diccionario = new unsigned char*[size];
    int* longitudes = new int[size];
    int posDic = 0;
    total = 0;
    int numero = 0;

    // Primera pasada: calcular tamaño total
    for (int i = 0; i < size; i++) {
        unsigned char c = data[i];
        numero%=1000;
        if (c >= 'a' && c <= 'z') {

            // Encontramos una letra minúscula, fin del número
            //cout << "Numero reconstruido: " << numero << ", letra: " << c << "\n";

            int len = (numero == 0) ? 1 : longitudes[numero] + 1;
            ++posDic;
            longitudes[posDic] = len;
            total += len;

            //cout << "Par (" << numero << ", " << c << ") -> longitud = "
              //   << len << ", total acumulado = " << total <<"\n\n\n"<< endl;
            numero = 0;  // Reiniciar para el siguiente número

        } else {
            int base;
            int valorByte = (int)c;


            // Es parte del número, acumular según el valor ASCII del byte


            // Determinar la base según el rango del byte actual

            if (valorByte < 10) {
                base = 10;
            } else if (valorByte < 100) {
                base = 100;
            } else if (valorByte < 1000) {
                base = 1000;
            } else {
                base = 1000;  // Máximo para 3 dígitos
            }


            numero = numero * base + valorByte;
            //cout << "Acumulando byte ASCII " << valorByte << " (char: '"
               //  << (char)c << "'), numero actual: " << numero
                 //<< ", base usada: " << base << endl;
        }
    }

    //cout << "Tamanioo total descomprimido: " << total << endl;

    // Reservar memoria para el resultado
    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos = 0;

    // Segunda pasada: reconstruir las cadenas
    posDic = 0;
    numero = 0;

    for (int i = 0; i < size; i++) {
        unsigned char c = data[i];
        numero%=1000;
        if (c >= 'a' && c <= 'z') {

            // Fin del número, crear la cadena
            unsigned char* nuevaCadena;
            int len;

            if (numero == 0) {
                // Caso base: solo el carácter
                len = 1;
                nuevaCadena = new unsigned char[1];
                nuevaCadena[0] = c;
            } else {
                // Concatenar cadena del diccionario + nuevo carácter
                if (numero > posDic) {
                    cerr << "Error: referencia a entrada " << numero
                         << " que no existe en diccionario (tamaño: " << posDic << ")" << endl;
                    // Manejo de error: tratar como caso base
                    len = 1;
                    nuevaCadena = new unsigned char[1];
                    nuevaCadena[0] = c;
                } else {
                    len = longitudes[numero] + 1;
                    nuevaCadena = new unsigned char[len];

                    // Copiar la cadena del diccionario
                    for (int j = 0; j < longitudes[numero]; j++) {
                        nuevaCadena[j] = diccionario[numero][j];
                    }
                    nuevaCadena[len - 1] = c;  // Agregar el nuevo carácter
                }
            }

            // Agregar al diccionario
            ++posDic;
            diccionario[posDic] = nuevaCadena;

            // Copiar al buffer de salida
            for (int j = 0; j < len; j++) {
                if (pos + j < total) {  // Verificación de límites
                    descomprimido[pos + j] = nuevaCadena[j];
                }
            }
            pos += len;
            numero = 0;  // Reiniciar para el siguiente número

        } else {
            // Acumular bytes para formar el número
            int valorByte = (int)c;

            // Determinar la base según el rango del byte actual
            int base;
            if (valorByte < 10) {
                base = 10;
            } else if (valorByte < 100) {
                base = 100;
            } else if (valorByte < 1000) {
                base = 1000;
            } else {
                base = 1000;
            }

            numero = numero * base + valorByte;
        }
    }

    // Verificación de seguridad
    if (pos != total) {
        cerr << "Advertencia: pos=" << pos << " pero total=" << total << endl;
    }

    // Liberar memoria del diccionario
    for (int i = 1; i <= posDic; i++) {
        delete[] diccionario[i];
    }
    delete[] diccionario;
    delete[] longitudes;

    // Agregar terminador de cadena
    descomprimido[total] = '\0';

    return descomprimido;
}




/**
 * @brief Descomprime un arreglo previamente comprimido usando el algoritmo RLE (Run-Length Encoding).
 *
 * Este algoritmo interpreta una secuencia en la forma "nX", donde "n" es un número
 * que indica cuántas veces se repite el carácter "X".
 * Por ejemplo: `"4A3B2C1D2A"` -> `"AAAABBBCCDAA"`.
 *
 * @param entrada Puntero al arreglo de caracteres comprimidos.
 * @param size    Cantidad de caracteres del arreglo de entrada.
 * @param total   Referencia donde se almacena la longitud del texto descomprimido.
 * @return unsigned char* Puntero al texto descomprimido (dinámico, debe liberarse con `delete[]`).
 *
 * @note
 * - La función calcula primero la longitud final para reservar memoria suficiente.
 * - Se asume que los números son enteros positivos y de cualquier cantidad de dígitos.
 * - La cadena resultante se termina con '\0'.
 */
unsigned char* descompresionRLE(unsigned char* entrada,int size, int& total){
    int longitudFinal = 0;
    int numero = 0;
    for (int i = 0; i < size; i += 3) {
        numero = int(entrada[i+1]);
        longitudFinal += numero;
    }
    //Crear arreglo dinámico con la longitud suficiente (+1 para '\0')
    unsigned char* descomprimido = new unsigned char[longitudFinal + 1];
    //realizar la descompresión
    int pos = 0;
    int repeticiones = 0;
    for (int i = 0; i < size; i += 3) {
        repeticiones = int(entrada[i+1]);
        for(int j = 0; j < repeticiones; j++){
            descomprimido[pos] = entrada[i + 2];
            pos++;
        }
        repeticiones = 0;
    }
    //Terminar la cadena con '\0'
    descomprimido[pos] = '\0';
    total = longitudFinal;


    return descomprimido;
}





