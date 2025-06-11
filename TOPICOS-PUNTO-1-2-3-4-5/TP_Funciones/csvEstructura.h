#ifndef CSVESTRUCTURA_H_INCLUDED
#define CSVESTRUCTURA_H_INCLUDED
#include "../fecha/fecha.h"
#include "../vector/vector.h"
#include "../nivGenApertura/nivGeneral.h"
#define TAM_LINEA 501
#define TAM_PERIODO 11      // Formato de fecha: "AAAA-MM-DD"
#define TAM_NIV_GEN_APERTURA 31   // Nivel general y aperturas: 30 caracteres + '\0'
#define ERR_ARCHIVO 1
#define SIN_MEM 2
#define ERROR_VEC 3
#define BIEN 0
#define ERR_FORMATO_LINEA 4
#define NOM_ARCH_GENERAL "indices_icc_general_capitulos.csv"
#define NOM_ARCH_ITEMS "indices_items_obra.csv"
#define NOM_ARCH_MOD "indices_icc_general_capitulos_modificado.csv"

typedef struct {
    char periodo[TAM_PERIODO];
    char nivGenApertura[TAM_NIV_GEN_APERTURA];
    double indice;
    char clasificador[14];
} RegistroICC;

// Funciones gen�ricas
typedef int (*FuncionParseo)(char* linea, void* estructura);
typedef void (*FuncionClasificador)(void* estructura);


int cargarDesdeCSV(const char* nomArch, Vector* vec,
                   size_t tamRegistro,
                   FuncionParseo parsearLinea);

int agregarClasificador(Vector* vec, FuncionClasificador generarClasif);


#endif // CSVESTRUCTURA_H_INCLUDED



