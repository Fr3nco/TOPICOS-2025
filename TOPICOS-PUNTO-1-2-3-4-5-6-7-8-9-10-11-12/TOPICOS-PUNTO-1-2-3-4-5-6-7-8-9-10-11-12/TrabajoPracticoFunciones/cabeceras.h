#ifndef CABECERAS_H
#define CABECERAS_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../TDAVector/vector.h"
#include "../Punto3-4/nivGeneral.h"
#define TAM_LINEA 501
#define TAM_PERIODO 11      // Formato de fecha: "AAAA-MM-DD"
#define TAM_NIV_GEN_APERTURA 41
#define ERR_ARCHIVO 1
#define SIN_MEM 2
#define ERROR_VEC 3
#define BIEN 0
#define ERR_FORMATO_LINEA 4
#define CANTIDAD_DICCIONARIO 10
#define NOM_ARCH_GENERAL "indices_icc_general_capitulos.csv"
#define NOM_ARCH_ITEMS "indices_items_obra.csv"


typedef struct {
    char periodo[TAM_PERIODO];
    char nivGenApertura[TAM_NIV_GEN_APERTURA];
    double indice;
    char clasificador[14];
    double var_mensual;
    double var_interanual;

} RegistroICC;


typedef struct
{
    char Encriptado;
    char Desencriptado;

}Diccionario;


// Funciones genéricas
typedef int (*FuncionParseo)(char* linea, void* estructura);
typedef void (*FuncionClasificador)(void* estructura);


int cargarDesdeCSV(const char* nomArch, Vector* vec,
                   size_t tamRegistro,
                   FuncionParseo parsearLinea);



//Funciones de manipulacion de cadenas
char *mi_strchr(char *s, int c);
char mi_toupper(char c);
char *mi_strcpy(char *cad1, const char *cad2);
int mi_strcmp (const char *cad1,const char *cad2);

//Funciones de Fecha
void convertirFecha(char* fOrig, char* fConv);


//Funciones de parseo y carga
int parsearLineaRegistroGeneralICC(char* linea, void* reg);
int parsearLineaItemsObra(char* linea, void* reg);
void eliminarComillas(char* str);
void reemplazarComaPorPunto(char* str);
bool normalizarNivelGeneralAperturasItems( char * str);

//funciones de Clasificador
int agregarClasificador(Vector* vec, FuncionClasificador generarClasif);
void generarClasificadorIndiceGeneral(void* reg);
void generarClasificadorItems(void * reg);
int ordenClasif(const char* clasif);

//Funciones de Encriptacion/Desencriptacion
void desencriptarIndiceGeneral(char* cadena);
void desencriptarCadena(char* cadena);
char desencriptarCaracter(char caracter);

//Funciones de variacion(mensual-interanual)
bool mismoMesAnioAnterior(const char *fechaActual, const char *fechaAnterior, char * tipoVariacion);
bool calculoVariacionMensual(Vector *v);
bool calculoVariacionInteranual(Vector *v);

//Funcion de Visualizacion
void imprimirRegistroICC(const void* elem);

//comparacionRegistros
int compararRegistrosICC(const void* a, const void* b);





#endif // CABECERAS_H
