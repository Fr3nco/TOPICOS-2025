#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

#define ERROR_VEC 3
#define NO_HAY_MEM 1
#define TODO_OK 0

#define INSERCION 1

typedef struct
{
	void* vec;
	int ce;
	int cap;
	size_t tamElem;
}
Vector;

typedef void(*Imprimir)(const void*);
typedef int(*Cmp)(const void* a, const void* b);

bool vectorCrear(Vector* vec, size_t tamElem);

void vectorDestruir(Vector* vector);

void vectorVaciar(Vector* vector);

int vectorInsertarAlFinal(Vector* vector, const void* elem);

void vectorMostrarGen(const Vector* vector, Imprimir imprimir);

int vectorCopiar(Vector* vRegistro, const Vector* vItems);

void vectorOrdenar(Vector*, Cmp cmp);

#endif // VECTOR_H_INCLUDED
