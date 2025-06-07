#include "nivGeneral.h"

void secuenciaPalabrasCrear(secuenciaPalabras* sec, char* cad)
{
    sec->cursor = (char*)cad;
    sec->secFin = false;
    sec->pos = 1;
}

bool secuenciaPalabrasDesencriptar(secuenciaPalabras* sec, Palabra* pal)
{
    while(*(sec->cursor) != '\0' && !esLetra(*(sec->cursor)))
    {
        sec->pos++;
        sec->cursor++;
    }

    if(*(sec->cursor) == '\0')
    {
        sec->secFin = true;
        return false;
    }

    char* iPal = pal->vPal;

    while(*(sec->cursor) != '\0' && esLetra(*(sec->cursor)))
    {
        int swap = (int)*(sec->cursor);

        if(sec->pos%2 == 0)
        {
            swap += 2;
        }else
        {
            swap += 4;
        }

        if(swap > 'z')
            swap = 'a' + (swap - 'z' - 1);

        *iPal = (char)swap;
        sec->pos++;
        iPal++;
        sec->cursor++;
    }

    *iPal = '\0';

    return true;
}

bool secuenciaPalabrasEscribir(secuenciaPalabras* sec, Palabra* pal)
{
    char* iPal = pal->vPal;

    while(*iPal != '\0')
    {
        *(sec->cursor) = *iPal;
        sec->cursor++;
        iPal++;
    }

    return true;
}

void secuenciaPalabrasEscribirCaracter(secuenciaPalabras* sec, char c)
{
    *(sec->cursor) = c;
    sec->cursor++;
}

bool secuenciaPalabrasFin(secuenciaPalabras* sec)
{
    return sec->secFin;
}

void secuenciaPalabrasCerrar(secuenciaPalabras* sec)
{
    *(sec->cursor) = '\0';
}

void palabraATitulo(Palabra* pal)
{
    int swap = (int)*(pal->vPal);
    swap -= 32;
    *pal->vPal = (char)swap;
}

bool esLetra(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
