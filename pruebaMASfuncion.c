#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_REGISTRO 501
#define ERROR_ARCHIVO 1

typedef struct {
    int d, m, a;
} Fecha;

typedef struct {
    Fecha Fingr;
    char nombre[31];
    double indice;
} VarCosto;

int FuncionGenerica(size_t tamreg);
void reemplazarComaPorPunto(char* str);

int main()
{

FuncionGenerica(sizeof(VarCosto));

    return 0;
}

int FuncionGenerica(size_t tamreg)
    {
    void* reg = malloc(tamreg);
    if (!reg) return 2;

    FILE* fpIn = fopen("indices_icc_general_capitulos.csv", "r");
    if (!fpIn)
    {
        free(reg);
        printf("No se pudo abrir el archivo de entrada.\n");
        return ERROR_ARCHIVO;
    }

    FILE* fpOut = fopen("archivoNuevo.csv", "w");
    if (!fpOut)
    {
        fclose(fpIn);
        free(reg);
        printf("No se pudo crear el archivo de salida.\n");
        return ERROR_ARCHIVO;
    }

    VarCosto* var = reg;
    char registro[TAM_REGISTRO];

    // Copia la cabecera en el archivoNuevo
    if (fgets(registro, TAM_REGISTRO, fpIn))
    {
        fprintf(fpOut, "%s", registro);
    }

    while (fgets(registro, TAM_REGISTRO, fpIn))
    {



         char* act = strchr(registro, '\n');
       *act = '\0';


        act = strrchr(registro, ';');
        *act = '\0';
        printf("Antes de reemplazar: %s\n", act + 1);
        reemplazarComaPorPunto(act + 1);
        printf("\ndespues de reemplazar: %s\n", act + 1);
        sscanf(act + 1, "%lf", &var->indice);


        act = strrchr(registro, ';');
        *act = '\0';
        strcpy(var->nombre, act + 1);


        sscanf(registro, "\"%d/%d/%d\"", &var->Fingr.d, &var->Fingr.m, &var->Fingr.a);


        fprintf(fpOut, "\"%04d/%02d/%02d\";%s;%lf\n",var->Fingr.a, var->Fingr.m, var->Fingr.d,var->nombre, var->indice);



    }

    fclose(fpIn);
    fclose(fpOut);
    free(reg);

    return 0;
}

void reemplazarComaPorPunto(char* str) {
    while (*str)
    {
        if (*str == ',')
        {
            *str = '.';
        }
        str++;
    }

}
