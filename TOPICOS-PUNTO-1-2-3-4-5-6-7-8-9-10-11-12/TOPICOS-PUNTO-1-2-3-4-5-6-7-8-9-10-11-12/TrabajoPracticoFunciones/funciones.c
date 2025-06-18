#include "cabeceras.h"
//Variable global
const Diccionario diccionario[CANTIDAD_DICCIONARIO] =
    {
    {'@', 'a'}, {'8', 'b'}, {'3', 'e'}, {'1', 'i'}, {'0', 'o'},
    {'$', 's'}, {'7', 't'}, {'|', 'l'}, {'5', 'm'}, {'9', 'n'}
    };


//PUNTO 1:
void convertirFecha(char* fOrig, char* fConv)
{
    int d, m, a;
    sscanf(fOrig, "%d/%d/%d", &d, &m, &a);
    sprintf(fConv, "%04d-%02d-%02d", a, m, d);
}
//PUNTO 2
void reemplazarComaPorPunto(char* str) {
    char* p = str;
    while (*p) {
        if (*p == ',')
        {
            *p = '.';
        }
        p++;
    }
}

int parsearLineaRegistroGeneralICC(char* linea, void* reg) {
    RegistroICC* registro = (RegistroICC*) reg;
    char lineaAux[TAM_LINEA];
    mi_strcpy(lineaAux, linea);

    char* ptr;
    char* finCampo;

    // Periodo (DD/MM/AAAA)
    finCampo = mi_strchr(lineaAux, ';');
    if (finCampo == NULL)
        return ERR_FORMATO_LINEA;
    *finCampo = '\0';
    eliminarComillas(lineaAux);
    convertirFecha(lineaAux, registro->periodo);

    // Nivel General y Aperturas
    ptr = finCampo + 1;
    finCampo = mi_strchr(ptr, ';');
    if (finCampo == NULL)
        return ERR_FORMATO_LINEA;
    *finCampo = '\0';
    eliminarComillas(ptr);
    desencriptarIndiceGeneral(ptr);
    strncpy(registro->nivGenApertura, ptr, TAM_NIV_GEN_APERTURA - 1);
    registro->nivGenApertura[TAM_NIV_GEN_APERTURA - 1] = '\0';

    // Indice
    ptr = finCampo + 1;
    reemplazarComaPorPunto(ptr);
    registro->indice = atof(ptr);

    return BIEN;
}
//PUNTO 3 y 4
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
//Punto 5

//funcion genérica reutilizada luego en archivo items_obras
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

/* SEGUNDO ARCHIVO*/

//PUNTO 6
int parsearLineaItemsObra(char* linea, void* reg) {
    RegistroICC* registro = (RegistroICC*) reg;
    char lineaAux[TAM_LINEA];
    mi_strcpy(lineaAux, linea);

    char* ptr;
    char* finCampo;

    // Periodo (DD/MM/AAAA)
    finCampo = mi_strchr(lineaAux, ';');
    if (finCampo == NULL)
        return ERR_FORMATO_LINEA;
    *finCampo = '\0';
    eliminarComillas(lineaAux);
    convertirFecha(lineaAux, registro->periodo);

    // Nivel General y Aperturas
    ptr = finCampo + 1;
    finCampo = mi_strchr(ptr, ';');
    if (finCampo == NULL)
        return ERR_FORMATO_LINEA;
    *finCampo = '\0';
    eliminarComillas(ptr);
    desencriptarCadena(ptr);
    normalizarNivelGeneralAperturasItems(ptr);
    strncpy(registro->nivGenApertura, ptr, TAM_NIV_GEN_APERTURA - 1);
    registro->nivGenApertura[TAM_NIV_GEN_APERTURA - 1] = '\0';

    // Indice
    ptr = finCampo + 1;
    reemplazarComaPorPunto(ptr);
    registro->indice = atof(ptr);

    return BIEN;
}
/*Punto 7*/
void desencriptarCadena(char* cadena)
{
    while(*cadena)
    {
        *cadena = desencriptarCaracter(*cadena); //en el contenido de cadena se guarda lo que devuelve la funcion
        cadena++;

    }

}
char desencriptarCaracter(char caracter)
{
    int i;

        for(i = 0 ; i < CANTIDAD_DICCIONARIO ; i++)
        {
            if(diccionario[i].Encriptado == caracter)
            {
                return diccionario[i].Desencriptado;
            }

}

return caracter; // en caso de que no este encriptado el caracter devuelve el mismo
}
//PUNTO 8
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
void generarClasificadorItems(void *reg)
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
//Punto 11 y Punto 12
bool mismoMesAnioAnterior(const char *fechaActual, const char *fechaAnterior, char * tipoVariacion)
{

    int anio1, mes1, dia1;
    sscanf(fechaActual, "%d-%d-%d", &anio1, &mes1, &dia1);

    int anio2, mes2, dia2;
    sscanf(fechaAnterior, "%d-%d-%d", &anio2, &mes2, &dia2);



    if(mi_strcmp(tipoVariacion,"INTERANUAL")==0)
        return (mes1 == mes2) && (anio1 == anio2 + 1);


    if(mi_strcmp(tipoVariacion, "MENSUAL") == 0) {
        if(mes1 == mes2 + 1 && anio1 == anio2)
            return dia1 == dia2;
        if(mes1 == 1 && mes2 == 12 && anio1 == anio2 + 1)
            return dia1 == dia2;
    }

    return false;
}
bool calculoVariacionMensual(Vector *vec)
{
    if (vec == NULL || vec->ce == 0)
        return false;

     for (size_t i = 0; i < vec->ce; i++)
    {
        RegistroICC *regActual = (RegistroICC*)((char*)vec->vec + i * vec->tamElem);
        int encontrado = 0;


        for (size_t j = 0; j < vec->ce && !encontrado; j++)
        {
            RegistroICC *regAnterior = (RegistroICC*)((char*)vec->vec + j * vec->tamElem);


            if (mismoMesAnioAnterior(regActual->periodo, regAnterior->periodo,"MENSUAL") &&
                mi_strcmp(regActual->nivGenApertura, regAnterior->nivGenApertura) == 0)
            {

                if (regAnterior->indice != 0)
                {
                    regActual->var_mensual = ((regActual->indice/
                                               regAnterior->indice)-1) * 100;
                }
                else
                {
                    regActual->var_mensual = 0;
                }
                encontrado = 1;
            }
        }

        if (!encontrado)
        {
            regActual->var_mensual = 0;
        }
    }
    return true;
}
bool calculoVariacionInteranual(Vector *vec)
{
    if (vec == NULL || vec->ce == 0)
        return false;


    for (size_t i = 0; i < vec->ce; i++)
    {
        RegistroICC *regActual = (RegistroICC*)((char*)vec->vec + i * vec->tamElem);
        int encontrado = 0;


        for (size_t j = 0; j < vec->ce && !encontrado; j++)
        {
            RegistroICC *regAnterior = (RegistroICC*)((char*)vec->vec + j * vec->tamElem);


            if (mismoMesAnioAnterior(regActual->periodo, regAnterior->periodo,"INTERANUAL") &&
                mi_strcmp(regActual->nivGenApertura, regAnterior->nivGenApertura) == 0)
            {

                if (regAnterior->indice != 0)
                {
                    regActual->var_interanual = ((regActual->indice/
                                               regAnterior->indice)-1) * 100;
                }
                else
                {
                    regActual->var_interanual = 0;
                }
                encontrado = 1;
            }
        }

        if (!encontrado)
        {
            regActual->var_interanual = 0;
        }
    }
    return true;
}
//funcion de carga de archivo
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
//funciones auxiliares
void eliminarComillas(char* str)
{
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
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
void imprimirRegistroICC(const void* elem)
{
    const RegistroICC* r = (const RegistroICC*) elem;
    printf("%11s | %41s | %8.2lf | %14s | %8.2lf | %8.2lf\n", r->periodo, r->nivGenApertura, r->indice,r->clasificador,r->var_mensual,r->var_interanual);
}
char mi_toupper(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - ('a' - 'A');
    }
    return c;
}
char *mi_strchr(char *str, int c)
{
    while (str != NULL && *str != '\0')
    {
        if (*str == (char)c) {
            return str;
        }
        str++;
    }
    return NULL;
}




