#ifndef CABECERAS_H
#define CABECERAS_H

#include "csvEstructura.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Funciones de manipulacion de cadenas
char *mi_strchr(char *s, int c);
char mi_toupper(char c);
char *mi_strcpy(char *cad1, const char *cad2);
int mi_strcmp (const char *cad1,const char *cad2);

//Funciones de Fecha
void convertirFecha(char* fOrig, char* fConv);
void formatearFecha(const Fecha *f, char *fechaStr);
Fecha parsearFecha(const char *fechaStr);

//Funciones de parseo y carga
int parsearLineaRegistroICC(char* linea, void* reg);
int parsearLineaItems(char* linea, void* reg);
void eliminarComillas(char* str);
void reemplazarComaPorPunto(char* str);
bool normalizarNivelGeneralAperturasItems( char * str);

//funciones de Clasificador
void generarClasificadorIndiceGeneral(void* reg);
void ClasificadorItems(void * reg);
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





#endif // CABECERAS_H
