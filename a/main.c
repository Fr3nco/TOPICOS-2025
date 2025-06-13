#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINEA 512
#define NUM_SIMBOLOS 10

typedef struct {
    char Encriptado;
    char Desencriptado;
} Diccionario;

char desencriptar(char caracter, Diccionario* diccionario, int ceDicc) {
    for (int i = 0; i < ceDicc; i++) {
        if (diccionario[i].Encriptado == caracter) {
            return diccionario[i].Desencriptado;
        }
    }
    return caracter;
}

void quitarComillas(char* campo) {
    size_t len = strlen(campo);
    if (len >= 2 && campo[0] == '"' && campo[len - 1] == '"') {
        memmove(campo, campo + 1, len - 2);
        campo[len - 2] = '\0';
    }
}

int main() {
    FILE* entrada = fopen("Indices_items_obra.csv", "r");
    FILE* salida = fopen("desencriptado.csv", "w");

    if (!entrada || !salida) {
        perror("Error abriendo archivos");
        return 1;
    }

    Diccionario diccionario[NUM_SIMBOLOS] = {
        {'@', 'a'}, {'8', 'b'}, {'3', 'e'}, {'1', 'i'}, {'0', 'o'},
        {'$', 's'}, {'7', 't'}, {'|', 'l'}, {'5', 'm'}, {'9', 'n'}
    };

    char linea[MAX_LINEA];
    fprintf(salida, "Periodo;Nivel General y Aperturas;Indice\n");

    while (fgets(linea, sizeof(linea), entrada)) {

        char* act = strrchr(linea, '\n');
        *act = '\0';

        // Puntero al último ;
        act = strrchr(linea, ';');



        char* indice = act + 1;

        // Cortar al segundo ;
        *act = '\0';
        act = strrchr(linea, ';');



        char* encriptado = act + 1;
        quitarComillas(encriptado);


        for (char* p = encriptado; *p != '\0'; p++) {
            *p = desencriptar(*p, diccionario, NUM_SIMBOLOS);
        }


        *act = '\0';
        char* fecha = linea;
        quitarComillas(fecha);


        fprintf(salida, "\"%s\";\"%s\";%s\n", fecha, encriptado, indice);
    }

    fclose(entrada);
    fclose(salida);
    printf("Archivo 'desencriptado.csv' generado correctamente.\n");
    return 0;
}
