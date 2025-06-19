#include "cabeceras.h"


//PUNTO 1:
bool esFechaValida(int dia, int mes, int anio) {
    // Validar año
    if(anio < 1900 || anio > 2025)
    return false;

    // Validar mes
    if(mes < 1 || mes > 12)
    return false;

    // Validar día
    if(dia < 1)
    return false;

    // Meses con 31 días
    if(mes == 1 || mes == 3 || mes == 5 || mes == 7 ||
       mes == 8 || mes == 10 || mes == 12) {
        return dia <= 31;
    }
    // Meses con 30 días
    else if(mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return dia <= 30;
    }
    // Febrero (considerando años bisiestos)
    else {
        if((anio % 400 == 0) || (anio % 100 != 0 && anio % 4 == 0)) {
            return dia <= 29;
        } else {
            return dia <= 28;
        }
    }
}
int convertirFecha(char* fOrig, char* fConv) {
    int d, m, a;
    sscanf(fOrig, "%d/%d/%d", &d, &m, &a);

    if(!esFechaValida(d, m, a)) {
        return ERROR_FECHA;
    }

    sprintf(fConv, "%04d-%02d-%02d", a, m, d);
    return BIEN;
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
// PUNTO 3: Desencriptar índice general
void desencriptarIndiceGeneral(char* cadena) {
    secuenciaPalabras secL, secE;
    Palabra pal;

    secuenciaPalabrasCrear(&secL, cadena);
    secuenciaPalabrasCrear(&secE, cadena);
    secuenciaPalabrasDesencriptar(&secL, &pal);
    palabraATitulo(&pal);

    while(!secuenciaPalabrasFin(&secL)) {
        secuenciaPalabrasEscribir(&secE, &pal);
        if(secuenciaPalabrasDesencriptar(&secL, &pal)) {
            secuenciaPalabrasEscribirCaracter(&secE, ' ');
        }
    }
    secuenciaPalabrasCerrar(&secE);
}

// PUNTO 4: Normalizar nivel general aperturas
bool normalizarNivelGeneralAperturas(char* str, bool esItem) {
    if(esItem) {
        char* primerGuion = mi_strchr(str, '_');
        if(!primerGuion)
            return false;
        memmove(str, primerGuion+1, strlen(primerGuion));
    }

    *str = mi_toupper(*str);
    for(char* p = str; *p; p++) {
        if(*p == '_') *p = ' ';
    }
    return true;
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

//PUNTO 6 parsear y desencriptar items
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
    char* ptr = cadena;

    while (*ptr != '\0')
    {
        switch (*ptr)
        {
        case '@':
            *ptr = 'a';
            break;
        case '8':
            *ptr = 'b';
            break;
        case '3':
            *ptr = 'e';
            break;
        case '1':
            *ptr = 'i';
            break;
        case '0':
            *ptr = 'o';
            break;
        case '$':
            *ptr = 's';
            break;
        case '7':
            *ptr = 't';
            break;
        case '|':
            *ptr = 'l';
            break;
        case '5':
            *ptr = 'm';
            break;
        case '9':
            *ptr = 'n';
            break;

        }
        ptr++;
    }
}

//PUNTO 8
bool normalizarNivelGeneralAperturasItems( char *str)
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
bool calculoVariacionMensual(Vector *vec) {
    if (vec == NULL || vec->ce == 0)
        return false;

    for (size_t i = 0; i < vec->ce; i++) {
        RegistroICC *regActual = (RegistroICC*)((char*)vec->vec + i * vec->tamElem);
        int encontrado = 0;

        for (size_t j = 0; j < vec->ce && !encontrado; j++) {
            RegistroICC *regAnterior = (RegistroICC*)((char*)vec->vec + j * vec->tamElem);

            if (mismoMesAnioAnterior(regActual->periodo, regAnterior->periodo, "MENSUAL") &&
                mi_strcmp(regActual->nivGenApertura, regAnterior->nivGenApertura) == 0) {
                if (regAnterior->indice != 0) {
                    double variacion = ((regActual->indice / regAnterior->indice) - 1) * 100;
                    regActual->var_mensual = redondear2Decimales(variacion);
                } else {
                    regActual->var_mensual = 0;
                }
                encontrado = 1;
            }
        }

        if (!encontrado) {
            regActual->var_mensual = 0;
        }
    }
    return true;
}

bool calculoVariacionInteranual(Vector *vec) {
    if (vec == NULL || vec->ce == 0)
        return false;

    for (size_t i = 0; i < vec->ce; i++) {
        RegistroICC *regActual = (RegistroICC*)((char*)vec->vec + i * vec->tamElem);
        int encontrado = 0;

        for (size_t j = 0; j < vec->ce && !encontrado; j++) {
            RegistroICC *regAnterior = (RegistroICC*)((char*)vec->vec + j * vec->tamElem);

            if (mismoMesAnioAnterior(regActual->periodo, regAnterior->periodo, "INTERANUAL") &&
                mi_strcmp(regActual->nivGenApertura, regAnterior->nivGenApertura) == 0) {
                if (regAnterior->indice != 0) {
                    double variacion = ((regActual->indice / regAnterior->indice) - 1) * 100;
                    regActual->var_interanual = redondear2Decimales(variacion);
                } else {
                    regActual->var_interanual = 0;
                }
                encontrado = 1;
            }
        }

        if (!encontrado) {
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
                puts("Error al ingresar elemento al vector\n");
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
    return BIEN;
}

int grabarArchivoSalida(const Vector* vector, const char* nombreArch) {
    FILE* arch = fopen(nombreArch, "wb");
    if(!arch)
    {
        printf("Error al abrir el archivo binario de salida: %s\n", nombreArch);
        return ERR_ARCHIVO;
    }

    RegistroSalida regSalida;

    for(size_t i = 0; i < vector->ce; i++) {
        RegistroICC* regICC = (RegistroICC*)((char*)vector->vec + i * vector->tamElem);

        // Índice ICC
        mi_strcpy(regSalida.periodo, regICC->periodo);
        mi_strcpy(regSalida.clasificador, regICC->clasificador);
        mi_strcpy(regSalida.nivel_general_aperturas, regICC->nivGenApertura);
        mi_strcpy(regSalida.tipo_variable, "indice_icc");
        regSalida.valor = regICC->indice;
        fwrite(&regSalida, sizeof(RegistroSalida), 1, arch);

        // Variación mensual
        mi_strcpy(regSalida.tipo_variable, "var_mensual");
        regSalida.valor = regICC->var_mensual;
        fwrite(&regSalida, sizeof(RegistroSalida), 1, arch);

        // Variación interanual
        mi_strcpy(regSalida.tipo_variable, "var_interanual");
        regSalida.valor = regICC->var_interanual;
        fwrite(&regSalida, sizeof(RegistroSalida), 1, arch);
    }

    fclose(arch);
    return BIEN;
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
int mostrarArchivoBinario( const char* nombreArchBinario)
{
    FILE* arch = fopen(nombreArchBinario, "rb");
    if (!arch)
    {
        printf("Error: No se pudo abrir el archivo binario '%s' para lectura.\n", nombreArchBinario);
        return ERR_ARCHIVO;
    }

    RegistroSalida reg;

    printf("\n--- Contenido del archivo binario: %s ---\n", nombreArchBinario);
    printf("%-10s | %-15s | %-41s | %-15s | %-10s\n",
           "Periodo", "Clasificador", "Nivel Gral. Aperturas", "Tipo Variable", "Valor");
    printf("--------------------------------------------------------------------------------------------------\n");

    int cantReg = 0;

    while (fread(&reg, sizeof(RegistroSalida), 1, arch))
    {
        printf("%-10s | %-15s | %-41s | %-15s | %.2f\n",
               reg.periodo,
               reg.clasificador,
               reg.nivel_general_aperturas,
               reg.tipo_variable,
               reg.valor);
        cantReg++;
    }

    if (feof(arch))
    {
        printf("--------------------------------------------------------------------------------------------------\n");
        printf("Lectura de archivo binario finalizada. Total de registros de salida leídos: %d\n", cantReg);
    }
    else
    {
        printf("Error de lectura al procesar el archivo binario.\n");
        fclose(arch);
        return ERR_ARCHIVO;
    }

    fclose(arch);
    return BIEN;
}

double redondear2Decimales(double valor)
{
    return (double)((int)(valor * 100 + 0.5)) / 100;

}


