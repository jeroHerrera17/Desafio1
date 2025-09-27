#include <iostream>
#include <fstream>
#include "Funciones.h"

using namespace std;


/**
 * @brief Punto de entrada principal del programa.
 *
 * Este programa realiza el siguiente flujo:
 * 1. Lee un archivo en memoria dinámica.
 * 2. Muestra el contenido original en consola.
 * 3. Aplica una operación XOR con una clave fija (0x5A).
 * 4. Aplica una rotación de bits a la derecha sobre cada byte.
 * 5. Muestra en consola el resultado después de cada transformación.
 *
 * @return int Código de salida (0 si termina correctamente).
 */
int main() {

    int sizeEncriptado = 0;
    int sizePista = 0;
    int nBits = 0;
    int claveK = 0;
    int nArchivos=4;
    int total=0;
    // Buffers para las rutas completas
    char archivoEncriptado[] = "../../Datos/encriptado0.txt";
    char archivoPista[]      = "../../Datos/pista0.txt";
    char archivoModificado[]      = "../../Datos/modificado0.txt";

    for (int i = 1; i <= nArchivos; i++) {  // supongamos que tenemos 5 archivos
        // Cambiar solo el dígito en la ruta
        archivoEncriptado[sizeof("../../Datos/encriptado") - 1] = '0' + i;
        archivoPista[sizeof("../../Datos/pista") - 1] = '0' + i;
        archivoModificado[sizeof("../../Datos/modificado") - 1] = '0' + i;
        //solo para trabajar con depuracion luego modificar en la practica

        cout << "Archivo encriptado: " << archivoEncriptado << endl;
        cout << "Archivo pista     : " << archivoPista << endl;
        unsigned char* enc=leerArchivoACharArray(archivoEncriptado,sizeEncriptado);
        unsigned char* pista=leerArchivoACharArray(archivoPista,sizePista);
        BuscarParametros( enc,  nBits,  claveK,  sizeEncriptado,  sizePista,  pista);

        // claveK=0x5A;
        // nBits=3;
        // DoXOR(enc, claveK, sizeEncriptado);
        // cout<<"\n\nDespues de XOR \n\n\n"<<endl;
        // mostrarContenido(enc,sizeEncriptado);
        // RotarDerecha(sizeEncriptado, enc, nBits);
        // mostrarContenido(enc,sizeEncriptado);
        // unsigned char* descomprimido = descompresionLZ78(enc, sizeEncriptado, total);
        // mostrarContenido(descomprimido,total);


    }

    /*
    if (encriptacion != nullptr) {
        cout << "El archivo tiene " << size << " caracteres." << endl << endl;

        // Mostrar contenido original
        mostrarContenido("Contenido original:", encriptacion, size);

        // Definir clave XOR
        unsigned char clave = 0x5A;

        // Aplicar XOR sobre el contenido en memoria
        DoXOR(encriptacion, clave, size);
        mostrarContenido("Contenido despues de XOR:", encriptacion, size);

        // Rotar bits a la derecha (ejemplo: 3 posiciones)
        RotarDerecha(size, encriptacion, 3);
        mostrarContenido("Contenido despues de rotacion:", encriptacion, size);
        int total;
        //unsigned char* descomprimido=descompresionLZ78(encriptacion, size,total);
        //mostrarContenido("Contenido despues descompresión", descomprimido,total);
        // Liberar memoria asignada
        delete[] encriptacion;
    } else {
        cerr << "Error al leer el archivo." << endl;
    }
    */
    return 0;

}
