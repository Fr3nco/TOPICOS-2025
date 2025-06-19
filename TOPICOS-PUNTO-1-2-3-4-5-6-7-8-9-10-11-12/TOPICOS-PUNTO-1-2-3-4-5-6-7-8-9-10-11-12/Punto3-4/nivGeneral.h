#ifndef NIVGENERAL_H_INCLUDED
#define NIVGENERAL_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    char* cursor;
    bool secFin;
    int pos;

}secuenciaPalabras;

typedef struct
{
    char vPal[51];
}Palabra;

void secuenciaPalabrasCrear(secuenciaPalabras* sec, char* cad);
bool secuenciaPalabrasDesencriptar(secuenciaPalabras* sec, Palabra* pal);
bool secuenciaPalabrasEscribir(secuenciaPalabras* sec, Palabra* pal);
void secuenciaPalabrasEscribirCaracter(secuenciaPalabras* sec, char c);
bool secuenciaPalabrasFin(secuenciaPalabras* sec);
void secuenciaPalabrasCerrar(secuenciaPalabras* sec);


void palabraATitulo(Palabra* pal);



#endif // NIVGENERAL_H_INCLUDED
