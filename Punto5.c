#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char periodo[11];
    char nivel_general_aperturas[31];
    double indice_icc;
    char Clasificador[14];
}tRegistro;

int mi_strcmp (const char *cad1,const char *cad2);
char * mi_strcpy(char *cad1, const char *cad2);
void generarClasificador(tRegistro *reg);

int main()
{
    return 0;
}
/*Crear una función para generar un campo que se llame Clasificador y que el mismo contenga
el valor Nivel general cuando nivel_general_apertura sea igual a Nivel general y Capítulos
cuando nivel_general_aperturas sea distinto a Nivel general.*/
int mi_strcmp (const char *cad1,const char *cad2)
{
    while((*cad1 !='\0'&& *cad2!='\0')&&*cad1==*cad2)
    {
        cad1++;
        cad2++;
    }

    if(*cad1!=*cad2)
    {
        if(*cad1>*cad2)
            return 1;
        else
            return -1;
    }

    return 0;
}

char *mi_strcpy(char *cad1, const char *cad2)
{
    if (cad1 == NULL || cad2 == NULL) {
        return NULL;
    }

    char *inicio = cad1;

    while(*cad2!='\0')
    {
        *cad1=*cad2;
        cad2++;
        cad1++;
    }

    *cad1='\0';

    return inicio;

}


void generarClasificador(tRegistro *reg)
{
    if(mi_strcmp(reg->nivel_general_aperturas,"Nivel general")==0)
    {
        mi_strcpy (reg->Clasificador,"Nivel general");
    }

        mi_strcpy (reg->Clasificador,"Capítulos");

}
