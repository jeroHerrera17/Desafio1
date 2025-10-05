/**
 * @brief Descomprime una secuencia codificada con el algoritmo LZ78 con desencriptado integrado.
 *
 * Esta función recibe un arreglo de bytes encriptados, aplica la desencriptación
 * mediante operaciones XOR y rotación de bits, y luego descomprime usando el algoritmo LZ78.
 * Implementa validación temprana que termina el procesamiento al detectar el primer
 * acceso inválido al diccionario o carácter no alfabético.
 *
 * El formato esperado después del desencriptado es una secuencia de ternas donde
 * los dos primeros bytes forman un número que referencia una entrada del diccionario
 * y el tercer byte es el carácter a concatenar.
 *
 * @param data Puntero al arreglo de datos encriptados y comprimidos.
 * @param size Cantidad de bytes en el arreglo data.
 * @param claveXOR Clave XOR para desencriptar los datos.
 * @param rotacionBits Número de bits para rotar a la derecha (0-7).
 * @param total Referencia donde se almacena el tamaño total del texto descomprimido.
 * @param esValido Referencia donde se indica si la descompresión fue válida.
 * @return Puntero a un arreglo dinámico de caracteres terminado en '\0'.
 *         El llamador es responsable de liberar esta memoria con delete[].
 */
unsigned char* descompresionLZ78(unsigned char* data, int size, unsigned char claveXOR,
                                 int rotacionBits, int& total, bool& esValido) {
    esValido = false;
    total = 0;

    if (size % 3 != 0) {
        return nullptr;
    }

    int numTernas = size / 3;

    // Crear diccionario dinámico
    unsigned char** diccionario = new unsigned char*[numTernas + 1];
    int* longitudes = new int[numTernas + 1];

    // Entrada vacía
    diccionario[0] = new unsigned char[1];
    diccionario[0][0] = '\0';
    longitudes[0] = 0;

    int posDic = 0;
    total = 0;

    // Ajustar rotación para que esté en rango válido
    rotacionBits %= 8;

    // Primera pasada: validación con terminación temprana
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;

        // Desencriptar al vuelo con XOR
        unsigned char byte1 = data[pos] ^ claveXOR;
        unsigned char byte2 = data[pos + 1] ^ claveXOR;
        unsigned char byte3 = data[pos + 2] ^ claveXOR;

        // Aplicar rotación a la derecha
        byte1 = (byte1 >> rotacionBits) | (byte1 << (8 - rotacionBits));
        byte2 = (byte2 >> rotacionBits) | (byte2 << (8 - rotacionBits));
        byte3 = (byte3 >> rotacionBits) | (byte3 << (8 - rotacionBits));

        int numero = (int(byte1) << 8) | int(byte2);
        unsigned char caracter = byte3;

        // Validación de carácter alfabético
        if (caracter < 'a' || caracter > 'z') {
            // Limpieza de memoria
            for (int j = 0; j <= posDic; j++) {
                delete[] diccionario[j];
            }
            delete[] diccionario;
            delete[] longitudes;
            return nullptr;
        }

        // Validación de referencia del diccionario
        if (numero > posDic) {
            // Limpieza de memoria
            for (int j = 0; j <= posDic; j++) {
                delete[] diccionario[j];
            }
            delete[] diccionario;
            delete[] longitudes;
            return nullptr;
        }

        // Si llegamos aquí, la terna es válida
        posDic++;
        int len = (numero == 0) ? 1 : longitudes[numero] + 1;
        longitudes[posDic] = len;
        total += len;

        // Prevención de overflow de memoria
        if (total > 1000000) {
            for (int j = 0; j <= posDic; j++) {
                delete[] diccionario[j];
            }
            delete[] diccionario;
            delete[] longitudes;
            return nullptr;
        }
    }

    // Validación de resultado final
    if (total == 0) {
        delete[] diccionario[0];
        delete[] diccionario;
        delete[] longitudes;
        return nullptr;
    }

    // Segunda pasada: construir resultado
    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos_resultado = 0;
    posDic = 0;

    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;

        // Desencriptar nuevamente
        unsigned char byte1 = data[pos] ^ claveXOR;
        unsigned char byte2 = data[pos + 1] ^ claveXOR;
        unsigned char byte3 = data[pos + 2] ^ claveXOR;

        byte1 = (byte1 >> rotacionBits) | (byte1 << (8 - rotacionBits));
        byte2 = (byte2 >> rotacionBits) | (byte2 << (8 - rotacionBits));
        byte3 = (byte3 >> rotacionBits) | (byte3 << (8 - rotacionBits));

        int numero = (int(byte1) << 8) | int(byte2);
        unsigned char caracter = byte3;

        // En la segunda pasada, todas las ternas han sido validadas
        posDic++;
        int len;

        if (numero == 0) {
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

    // Limpieza final
    for (int i = 0; i <= posDic; i++) {
        delete[] diccionario[i];
    }
    delete[] diccionario;
    delete[] longitudes;

    descomprimido[total] = '\0';
    esValido = true;
    return descomprimido;
}

/**
 * @brief Descomprime un arreglo usando el algoritmo RLE con desencriptado integrado.
 *
 * Esta función recibe un arreglo de bytes encriptados, aplica la desencriptación
 * mediante operaciones XOR y rotación de bits, y luego descomprime usando el algoritmo
 * Run-Length Encoding. Implementa validación temprana que termina el procesamiento
 * al detectar la primera terna con formato inválido.
 *
 * El algoritmo interpreta el formato de ternas después del desencriptado donde
 * el segundo byte indica el número de repeticiones y el tercer byte es el carácter
 * a repetir.
 *
 * @param data Puntero al arreglo de datos encriptados y comprimidos.
 * @param size Cantidad de caracteres del arreglo de entrada.
 * @param claveXOR Clave XOR para desencriptar los datos.
 * @param rotacionBits Número de bits para rotar a la derecha (0-7).
 * @param total Referencia donde se almacena el tamaño total del arreglo descomprimido.
 * @param esValido Referencia donde se indica si la descompresión fue válida.
 * @return Puntero al texto descomprimido terminado en '\0'.
 *         El llamador es responsable de liberar esta memoria con delete[].
 */
unsigned char* descompresionRLE(unsigned char* data, int size, unsigned char claveXOR,
                                int rotacionBits, int& total, bool& esValido) {
    esValido = false;
    total = 0;

    if (size % 3 != 0) {
        return nullptr;
    }

    int numTernas = size / 3;

    // Ajustar rotación para que esté en rango válido
    rotacionBits %= 8;

    // Primera pasada: validación con terminación temprana
    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;

        // Desencriptar con XOR
        unsigned char byte1 = data[pos] ^ claveXOR;
        unsigned char byte2 = data[pos + 1] ^ claveXOR;  // repeticiones
        unsigned char byte3 = data[pos + 2] ^ claveXOR;  // caracter

        // Aplicar rotación a la derecha
        byte1 = (byte1 >> rotacionBits) | (byte1 << (8 - rotacionBits));
        byte2 = (byte2 >> rotacionBits) | (byte2 << (8 - rotacionBits));
        byte3 = (byte3 >> rotacionBits) | (byte3 << (8 - rotacionBits));

        int repeticiones = int(byte2);
        unsigned char caracter = byte3;

        // Validación de formato de terna
        if (caracter < 'a' || caracter > 'z' || repeticiones <= 0 || repeticiones >= 256) {
            return nullptr;
        }

        total += repeticiones;

        // Prevención de overflow de memoria
        if (total > 1000000) {
            return nullptr;
        }
    }

    // Validación de resultado final
    if (total == 0) {
        return nullptr;
    }

    // Segunda pasada: construir resultado
    unsigned char* descomprimido = new unsigned char[total + 1];
    int pos_resultado = 0;

    for (int i = 0; i < numTernas; i++) {
        int pos = i * 3;

        // Desencriptar nuevamente
        unsigned char byte1 = data[pos] ^ claveXOR;
        unsigned char byte2 = data[pos + 1] ^ claveXOR;
        unsigned char byte3 = data[pos + 2] ^ claveXOR;

        byte1 = (byte1 >> rotacionBits) | (byte1 << (8 - rotacionBits));
        byte2 = (byte2 >> rotacionBits) | (byte2 << (8 - rotacionBits));
        byte3 = (byte3 >> rotacionBits) | (byte3 << (8 - rotacionBits));

        int repeticiones = int(byte2);
        unsigned char caracter = byte3;

        // En la segunda pasada, todas las ternas han sido validadas
        for (int j = 0; j < repeticiones && pos_resultado < total; j++) {
            descomprimido[pos_resultado] = caracter;
            pos_resultado++;
        }
    }

    descomprimido[total] = '\0';
    esValido = true;
    return descomprimido;
}
