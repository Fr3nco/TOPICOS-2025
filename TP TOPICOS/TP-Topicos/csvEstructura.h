#ifndef CSVESTRUCTURA_H_INCLUDED
#define CSVESTRUCTURA_H_INCLUDED


#include "../Vector/vector.h"

#define TAM_LINEA 501
#define TAM_PERIODO 11      // Formato de fecha: "AAAA-MM-DD"
#define TAM_NIV_GEN_APERTURA 31   // Nivel general y aperturas: 30 caracteres + '\0'
#define ERR_ARCHIVO 1
#define NO_HAY_MEM 2
#define ERROR_VEC 3
#define BIEN 0
#define ERR_FORMATO_LINEA 4

typedef struct {
    char periodo[TAM_PERIODO];              // "AAAA-MM-DD"
    char nivGenApertura[TAM_NIV_GEN_APERTURA];
    double indice;
    char clasificador[14];
} RegistroICC;

// Funciones genéricas


// Tipos para funciones callback
typedef int (*FuncionParseo)(char* linea, void* estructura);
typedef void (*FuncionClasificador)(void* estructura);

// Funciones para manejo de CSV
int cargarDesdeCSV(const char* nomArch, Vector* vec,
                   size_t tamRegistro,
                   FuncionParseo parsearLinea);

int agregarClasificador(Vector* vec, FuncionClasificador generarClasif);


#endif // CSVESTRUCTURA_H_INCLUDED
