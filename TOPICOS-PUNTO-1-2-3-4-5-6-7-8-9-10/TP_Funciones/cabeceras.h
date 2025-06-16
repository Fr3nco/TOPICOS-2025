#ifndef CABECERAS_H
#define CABECERAS_H

#include "csvEstructura.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int mi_strcmp (const char *cad1,const char *cad2);
void convertirFecha(char* fOrig, char* fConv);
int parsearLineaRegistroICC(char* linea, void* reg);
void eliminarComillas(char* str);
void reemplazarComaPorPunto(char* str);
void imprimirRegistroICC(const void* elem);
char *mi_strcpy(char *cad1, const char *cad2);
void generarClasificadorIndiceGeneral(void* reg);
void desencriptarIndiceGeneral(char* cadena);
void formatearFecha(const Fecha *f, char *fechaStr);
Fecha parsearFecha(const char *fechaStr);
char mi_toupper(char c);
bool  normalizarNivelGeneralAperturasItems( char * str);
char *mi_strchr(char *s, int c);
void ClasificadorItems(void * reg);
int parsearLineaItems(char* linea, void* reg);
void desencriptarCadena(char* cadena, Diccionario* diccionario, int ceDicc);
char desencriptarCaracter(char caracter, Diccionario* diccionario, int ceDicc);
int ordenClasif(const char* clasif);

#endif // CABECERAS_H
