#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char perido[12];
    char NivGenApertura[31];
    char indice[17];
} Archivo;

void reemplazarComaPorPunto(char *indice);
double obtenerIndiceNumerico(Archivo* linea);

int main()
{
    system("chcp 1252 >nul");//para los acentos

    //leo desde el archivo y copio en la estructura
    Archivo linea = {"1/2/2022","iypcngwjaq","103"};

    printf("Índice original: %s\n", linea.indice);

    double indiceNumerico = obtenerIndiceNumerico(&linea);

    printf("Índice modificado: %s\n", linea.indice);

    printf("Índice numérico : %.4f\n", indiceNumerico);

    return 0;
}
void reemplazarComaPorPunto(char *indice)
{
    //Buscos carácter nulo de terminación
    for (int i = 0; indice[i] != '\0'; i++)
    {
        if (indice[i] == ',')
        {
            indice[i] = '.'; // Si encuentra una coma, la reemplaza por un punto
        }
    }
}
double obtenerIndiceNumerico(Archivo* linea)
{
    char indiceTemp[17];
    strcpy(indiceTemp, linea->indice);
    //reempazamos
    reemplazarComaPorPunto(linea->indice);

    return atof(linea->indice); // atof convierte string a double
}
