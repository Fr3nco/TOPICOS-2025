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
#define TAM_CLASIFICADOR 20
#define ERR_ARCHIVO 1
#define SIN_MEM 2
#define ERROR_VEC 3
#define BIEN 0
#define ERROR_FECHA 1
#define ERR_FORMATO_LINEA 4
#define CANTIDAD_DICCIONARIO 10
#define NOM_ARCH_GENERAL "indices_icc_general_capitulos.csv"
#define NOM_ARCH_ITEMS "Indices_items_obra.csv"
#define NOM_ARCH_SALIDA "indices_icc_salida.dat"

// Estructuras de datos
typedef struct {
    char periodo[TAM_PERIODO];
    char nivGenApertura[TAM_NIV_GEN_APERTURA];
    double indice;
    char clasificador[14];
    double var_mensual;
    double var_interanual;
} RegistroICC;

typedef struct {
    char periodo[TAM_PERIODO];
    char clasificador[TAM_CLASIFICADOR];
    char nivel_general_aperturas[TAM_NIV_GEN_APERTURA];
    char tipo_variable[15]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroSalida;

// Funciones genéricas
typedef int (*FuncionParseo)(char* linea, void* estructura);
typedef void (*FuncionClasificador)(void* estructura);

// Funciones de manipulación de archivos
int cargarDesdeCSV(const char* nomArch, Vector* vec,
                   size_t tamRegistro,
                   FuncionParseo parsearLinea);
int grabarArchivoSalida(const Vector* vector, const char* nombreArch);
int mostrarArchivoBinario(const char* nombreArchBinario);

// Funciones de manipulación de cadenas y ctype.h
char *mi_strchr(char *s, int c);
char mi_toupper(char c);
char *mi_strcpy(char *cad1, const char *cad2);
int mi_strcmp (const char *cad1,const char *cad2);
void eliminarComillas(char* str);
void reemplazarComaPorPunto(char* str);

// Funciones de fecha
int convertirFecha(char* fOrig, char* fConv);
bool esFechaValida(int dia,int mes,int anio);

// Funciones de parseo y carga
int parsearLineaRegistroGeneralICC(char* linea, void* reg);
int parsearLineaItemsObra(char* linea, void* reg);

// Funciones de normalización y desencriptación
bool normalizarNivelGeneralAperturasItems(char * str);
void desencriptarIndiceGeneral(char* cadena);
void desencriptarCadena(char* cadena);
char desencriptarCaracter(char caracter);

// Funciones de clasificación
int agregarClasificador(Vector* vec, FuncionClasificador generarClasif);
void generarClasificadorIndiceGeneral(void* reg);
void generarClasificadorItems(void * reg);
int ordenClasif(const char* clasif);

// Funciones de cálculo de variaciones
bool mismoMesAnioAnterior(const char *fechaActual, const char *fechaAnterior, char * tipoVariacion);
bool calculoVariacionMensual(Vector *v);
bool calculoVariacionInteranual(Vector *v);
double redondear2Decimales(double valor);

// Funciones de comparación y visualización
int compararRegistrosICC(const void* a, const void* b);
void imprimirRegistroICC(const void* elem);

#endif // CABECERAS_H
