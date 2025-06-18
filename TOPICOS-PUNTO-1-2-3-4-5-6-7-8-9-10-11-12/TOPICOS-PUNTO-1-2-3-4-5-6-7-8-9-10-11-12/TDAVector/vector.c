#include "vector.h"
#include <stdlib.h>
#include <string.h>


bool vectorCrear(Vector* vec, size_t tamElem)
{
    vec->cap=10;
    vec->vec = malloc(vec->cap * tamElem);
    if(vec->vec==NULL)
        return false;

    vec->ce=0;
    vec->tamElem = tamElem;

    return true;
}

void vectorVaciar(Vector* vector)
{
    //lo dejo con capacidad pero sin elementos
    vector->cap=10;
    vector->vec = realloc(vector->vec,vector->cap*vector->tamElem); //cambio el tamaño
    vector->ce=0;
}

void vectorDestruir(Vector* vector)
{
    free(vector->vec);
    vector->vec=NULL;
    vector->ce=0;
    vector->cap=0;
    vector->tamElem=0;
}

int vectorInsertarAlFinal(Vector* vector, const void* elem)
{
    if(vector->ce == vector->cap)
    {
        int nueCap = vector->cap*1.5;    //nueva capacidad
        int* nueVec = realloc(vector->vec, nueCap*vector->tamElem);
        //nuevo vector para no perder la direccion en caso de q no lo haga

        if(!nueVec)
            return NO_HAY_MEM;

        vector->vec = nueVec;
        vector->cap = nueCap;
    }

    void* posIns = vector->vec + vector->ce * vector->tamElem;

    memcpy(posIns,elem,vector->tamElem);
    vector->ce++;

    return TODO_OK;
}

void vectorMostrarGen(const Vector* vector, Imprimir imprimir)
{
    void* ult = vector->vec + (vector->ce - 1)* vector->tamElem;

    printf("Cantidad de Elementos: %d de [%d]\n",(int)vector->ce,vector->cap);

    for(void* i = vector->vec; i <= ult; i += vector->tamElem)
    {
        imprimir(i);
    }
    printf("\n");
}

int vectorCopiar(Vector* vRegistro, const Vector* vItems) {

    if (!vRegistro || !vItems || vItems->tamElem != vRegistro->tamElem)
        return ERROR_VEC;

    for (size_t i = 0; i < vItems->ce; i++) {
        void* elem = (char*)vItems->vec + i * vItems->tamElem;
        if (vectorInsertarAlFinal(vRegistro, elem)) {
            puts("Error al copiar elemento");
            return NO_HAY_MEM;
        }
    }

    return TODO_OK;
}

void vectorOrdenar(Vector* vec, Cmp cmp)
{
    qsort(vec->vec, vec->ce, vec->tamElem, cmp);
}






