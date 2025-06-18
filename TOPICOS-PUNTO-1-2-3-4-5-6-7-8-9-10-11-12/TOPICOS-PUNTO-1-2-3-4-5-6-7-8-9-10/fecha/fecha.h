#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int dia;
    int mes;
    int anio;

}Fecha;

void ingresarFecha (Fecha *fecha);
void mostrarFecha(const Fecha *fecha);
bool esFechaValida(const Fecha* fecha);
int cantDiasMes(int mes, int anio);
bool esBisiesto(int anio);
Fecha diaSiguiente(const Fecha* fecha);
Fecha obtenerDiaSiguiente(Fecha fecha);
Fecha sumarNDias_2(Fecha fechaInicial, int n);
Fecha sumarNDias(const Fecha* f,const int dias);
Fecha restarNDias(const Fecha* f, const int dias);
int diferenciaEnDias(Fecha fecha1, Fecha fecha2);
//optimizado
int diferenciaEnDiasOptimizado(Fecha fecha1, Fecha fecha2);
int diasEnAnio(int anio);
int diasDesdeInicioAnio(Fecha fecha);

#endif // FECHA_H_INCLUDED
