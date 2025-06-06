#include <stdio.h>
#include <stdlib.h>

#define TAM_REGISTRO 501
#define ERROR_ARCHIVO 1

typedef struct {
    int d, m, a;
} Fecha;

typedef struct {
    Fecha Fingr;
    char nombre[31];
    int indice;
} VarCosto;

int FuncionGenerica(size_t tamreg);

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
        //ACA GUARDA DE "registro" a la estructura
        sscanf(registro,"\"%d/%d/%d\";\"%30[^\"]\";%d",&var->Fingr.d, &var->Fingr.m, &var->Fingr.a,var->nombre, &var->indice);//"\"%d/%d/%d\";\"%30[^\"]\";%d"   "\"%d/%d/%d\";\"%30s\";%d"

        // AAAA/MM/DD // ACA ESCRIBE EL NUEVO ARCHIVO
        fprintf(fpOut, "%04d/%02d/%02d ;%s;%d\n",var->Fingr.a, var->Fingr.m, var->Fingr.d,var->nombre, var->indice);
    }

    fclose(fpIn);
    fclose(fpOut);
    free(reg);

    return 0;
}
