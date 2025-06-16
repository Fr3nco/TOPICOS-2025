#include "cabeceras.h"

int cargarDesdeCSV(const char* nomArch,Vector *vec,size_t tamRegistro,FuncionParseo parsearLinea)
{
    FILE * arch=fopen(nomArch,"rt");
    if(!arch)
    {
        printf("Error en la apertura de %s\n",nomArch);
        return ERR_ARCHIVO;
    }
    if(!vectorCrear(vec,tamRegistro))
    {
        fclose(arch);
        return NO_HAY_MEM;
    }

    char linea[TAM_LINEA];
    void *estructura= malloc(tamRegistro);
    if (!estructura)
    {
        fclose(arch);
        return NO_HAY_MEM;
    }

    fgets(linea,TAM_LINEA,arch);

    while(fgets(linea,TAM_LINEA,arch)!=NULL)
    {
        if(parsearLinea(linea,estructura)==BIEN)
        {
            if(vectorInsertarAlFinal(vec,estructura))
            {
                puts("Error al ingresos elemento al vector\n");
                free(estructura);
                fclose(arch);
                vectorDestruir(vec);
                return NO_HAY_MEM;

            }
        }
        else
        {
            puts("\nFormato incorrecto de linea");
        }
    }

    free(estructura);
    fclose(arch);
    return TODO_OK;
}

int parsearLineaRegistroICC(char* linea, void* reg)
{
    RegistroICC* registro = (RegistroICC*) reg;
    char lineaAux[TAM_LINEA];
    mi_strcpy(lineaAux, linea);

    char* ptr;

    //  Periodo (DD/MM/AAAA)
    ptr = strtok(lineaAux, ";");
    if (ptr == NULL)
        return ERR_FORMATO_LINEA;
    eliminarComillas(ptr);
    convertirFecha(ptr, registro->periodo);

    //  Nivel General y Aperturas
    ptr = strtok(NULL, ";");
    if (ptr == NULL)
        return ERR_FORMATO_LINEA;
    eliminarComillas(ptr);
    desencriptarIndiceGeneral(ptr); // Llamada a la función desencriptar.
    strncpy(registro->nivGenApertura, ptr, TAM_NIV_GEN_APERTURA - 1);
    registro->nivGenApertura[TAM_NIV_GEN_APERTURA - 1] = '\0';

    // Tercer: Indice
    ptr = strtok(NULL, ";");
    if (ptr == NULL)
        return ERR_FORMATO_LINEA;
    reemplazarComaPorPunto(ptr);
    registro->indice = atof(ptr);

    return BIEN;
}

void convertirFecha(char* fOrig, char* fConv)
{
    int d, m, a;
    sscanf(fOrig, "%d/%d/%d", &d, &m, &a);
    sprintf(fConv, "%04d-%02d-%02d", a, m, d);
}

Fecha parsearFecha(const char *fechaStr)
{
    Fecha f;
    sscanf(fechaStr, "%d/%d/%d", &f.dia, &f.mes, &f.anio);
    return f;
}

// Función para convertir de la estructura Fecha a string "YYYY-MM-DD"
void formatearFecha(const Fecha *f, char *fechaStr)
{
    sprintf(fechaStr, "%04d-%02d-%02d", f->anio, f->mes, f->dia);
}


void eliminarComillas(char* str)
{
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

void reemplazarComaPorPunto(char* str) {
    char* p = str;
    while (*p) {
        if (*p == ',') {
            *p = '.';
        }
        p++;
    }
}

void imprimirRegistroICC(const void* elem)
{
    const RegistroICC* r = (const RegistroICC*) elem;
    printf("%11s | %31s | %8.2lf | %14s\n", r->periodo, r->nivGenApertura, r->indice,r->clasificador);
}

int mi_strcmp (const char *cad1,const char *cad2)
{
    while((*cad1 !='\0'&& *cad2!='\0')&&*cad1==*cad2)
    {
        cad1++;
        cad2++;
    }

    if(*cad1!=*cad2)
    {
        if(*cad1>*cad2)
            return 1;
        else
            return -1;
    }

    return 0;
}

char *mi_strcpy(char *cad1, const char *cad2)
{
    if (cad1 == NULL || cad2 == NULL)
    {
        return NULL;
    }

    char *inicio = cad1;

    while(*cad2!='\0')
    {
        *cad1=*cad2;
        cad2++;
        cad1++;
    }

    *cad1='\0';

    return inicio;

}

int agregarClasificador(Vector* vec, FuncionClasificador generarClasif)
{
    if(vec == NULL || generarClasif == NULL)
        return ERROR_VEC;

    for(size_t i = 0; i < vec->ce; i++)
    {
        void* elem = (char*)vec->vec + i * vec->tamElem;
        generarClasif(elem);
    }

    return BIEN;
}

void generarClasificadorIndiceGeneral(void* reg)
{
    RegistroICC* registro = (RegistroICC*) reg;
    if(mi_strcmp(registro->nivGenApertura, "Nivel general") == 0)
    {
        mi_strcpy(registro->clasificador, "Nivel general");
    }
    else
    {
        mi_strcpy(registro->clasificador, "Capitulos");
    }
}

void desencriptarIndiceGeneral(char* cadena)
{
    secuenciaPalabras secL, secE;
    Palabra pal;

    secuenciaPalabrasCrear(&secL, cadena);
    secuenciaPalabrasCrear(&secE, cadena);
    secuenciaPalabrasDesencriptar(&secL, &pal);
    palabraATitulo(&pal);

    while(!secuenciaPalabrasFin(&secL))
    {
        secuenciaPalabrasEscribir(&secE, &pal);

        if(secuenciaPalabrasDesencriptar(&secL, &pal))
        {
            secuenciaPalabrasEscribirCaracter(&secE, ' ');
        }
    }

    secuenciaPalabrasCerrar(&secE);
}
/* SEGUNDO ARCHIVO*/

//PUNTO 6
int parsearLineaItems(char* linea, void* reg)
{
    int ceDicc;
    Diccionario diccionario[] =
    {
    {'@', 'a'}, {'8', 'b'}, {'3', 'e'}, {'1', 'i'}, {'0', 'o'},
    {'$', 's'}, {'7', 't'}, {'|', 'l'}, {'5', 'm'}, {'9', 'n'}
    };

    ceDicc = sizeof(diccionario) / sizeof(diccionario[0]);

    RegistroICC* registro = (RegistroICC*) reg;
    char lineaAux[TAM_LINEA];
    mi_strcpy(lineaAux, linea);

    char* ptr;

    //  Periodo (DD/MM/AAAA)
    ptr = strtok(lineaAux, ";");
    if (ptr == NULL)
        return ERR_FORMATO_LINEA;
    eliminarComillas(ptr);
    convertirFecha(ptr, registro->periodo);

    //  Nivel General y Aperturas
    ptr = strtok(NULL, ";");
    if (ptr == NULL)
        return ERR_FORMATO_LINEA;
    eliminarComillas(ptr);
    desencriptarCadena(ptr, diccionario, ceDicc);
    normalizarNivelGeneralAperturasItems(ptr);

    strncpy(registro->nivGenApertura, ptr, TAM_NIV_GEN_APERTURA - 1);
    registro->nivGenApertura[TAM_NIV_GEN_APERTURA - 1] = '\0';

    // Tercer: Indice
    ptr = strtok(NULL, ";");
    if (ptr == NULL)
        return ERR_FORMATO_LINEA;
    reemplazarComaPorPunto(ptr);
    registro->indice = atof(ptr);

    return BIEN;
}
/*Punto 7*/

void desencriptarCadena(char* cadena, Diccionario* diccionario, int ceDicc)
{
    while(*cadena)
    {
        *cadena = desencriptarCaracter(*cadena, diccionario, ceDicc); //en el contenido de cadena se guarda lo que devuelve la funcion
        cadena++;

    }

}


char desencriptarCaracter(char caracter, Diccionario* diccionario, int ceDicc)
{
    int i;

for(i = 0 ; i < ceDicc ; i++)
{
    if(diccionario[i].Encriptado == caracter)
    {
        return diccionario[i].Desencriptado;
    }

}

return caracter; // en caso de que no este encriptado el caracter devuelve el mismo
}



//PUNTO 8
char mi_toupper(char c)
{
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}
char *mi_strchr(char *str, int c)
{
    while (str != NULL && *str != '\0') {
        if (*str == (char)c) {
            return str;
        }
        str++;
    }
    return NULL;
}

bool  normalizarNivelGeneralAperturasItems( char *str)
{
    char *primerGuion= mi_strchr(str,'_');
    if(primerGuion==NULL)
        return false;
    memmove(str,primerGuion+1, strlen(primerGuion));

    *str=mi_toupper(*str);

  char *p = str;
    while (*p != '\0')
    {
        if (*p == '_')
        {
            *p = ' ';
        }
        p++;
    }

    return true;
}

/* Punto 9 */


void ClasificadorItems(void *reg)
{
    RegistroICC *registro= (RegistroICC*)reg;
    mi_strcpy(registro->clasificador,"ítems");
}


/* Punto 10 */

int compararRegistrosICC(const void* a, const void* b) {
    const RegistroICC* r1 = (const RegistroICC*)a;
    const RegistroICC* r2 = (const RegistroICC*)b;

    int cmpFecha = mi_strcmp(r1->periodo, r2->periodo);
    if (cmpFecha != 0)
        return cmpFecha;

    return ordenClasif(r1->clasificador) - ordenClasif(r2->clasificador);
}

int ordenClasif(const char* clasif)
{
        if (mi_strcmp(clasif, "Nivel general") == 0) return 0;
        if (mi_strcmp(clasif, "Capitulos") == 0) return 1;
        return 2; // ítems u otros
}
