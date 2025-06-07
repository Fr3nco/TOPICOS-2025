#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

#define NO_HAY_MEM 1
#define TODO_OK 0

typedef struct
{
	void* vec;
	int ce;
	int cap;
	size_t tamElem;
}
Vector;

typedef void(*Imprimir)(const void*);

bool vectorCrear(Vector* vec, size_t tamElem);

void vectorDestruir(Vector* vector);

void vectorVaciar(Vector* vector);

int vectorInsertarAlFinal(Vector* vector, const void* elem);

void vectorMostrarGen(const Vector* vector, Imprimir imprimir);

#endif // VECTOR_H_INCLUDED
