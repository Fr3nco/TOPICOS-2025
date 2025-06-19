#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../fecha/fecha.h"
#include "../vector/vector.h"
#include "../palabra/Palabra.h"
#include "../palabra/SecuenciaPalabras.h"

#define TAM_LINEA 501
#define TAM_PERIODO 11      // "DD/MM/AAAA" o "AAAA-MM-DD" + '\0'
#define TAM_NIV_GEN_APERTURA 41
#define TAM_CLASIFICADOR 20
#define TAM_TIPO_VARIABLE 15
#define ERR_ARCHIVO 2
#define ERROR_VEC 3
#define ERR_FORMATO_LINEA 10
#define NOM_ARCH_ICC "indices_icc_general_capitulos.csv"
#define NOM_ARCH_ITEMS_OBRA "Indices_items_obra.csv"
#define NOM_ARCH_UNIDO "archivo_unido.bin"

typedef struct
{
    char periodo[TAM_PERIODO];
    char nivGenApertura[TAM_NIV_GEN_APERTURA];
    double indice;
    char clasificador[TAM_CLASIFICADOR];
    double var_mensual;
    double var_interanual;
} RegistroICC;

typedef struct
{
    char periodo[TAM_PERIODO];
    char clasificador[TAM_CLASIFICADOR];
    char nivel_general_aperturas[TAM_NIV_GEN_APERTURA];
    char tipo_variable[TAM_TIPO_VARIABLE]; // "indice_icc", "var_mensual", "var_interanual"
    double valor;
} RegistroSalida;

void convertirFecha(char *fOrig, char *fConv);//1
void reemplazarComaPorPunto(char *indice);//2
int mostrarArchivo(const char* nomArch);
//Carga
int cargarRegistrosDesdeCSV(const char* nomArch, Vector* vecRegistros);
int parsearLineaRegistroICC(char* linea, RegistroICC* registro);
void eliminarComillas(char* str);
//imprimir
void imprimirRegistroICC(const void* elem);
//descencriptar
void desencriptarNivelApertura(char* cadena);//3
//normalizar
void normalizarNivelApertura(char* cadena);//4
void palabraAMinuscula(Palabra* pal);
//calificador
void generarClasificador(RegistroICC* registro);//5
//2do archivo
typedef int (*ParsearLineaFunc)(char* linea, RegistroICC* registro);
int cargarRegistrosDesdeCSVGen(const char* nomArch, Vector* vecRegistros, ParsearLineaFunc parsear);
//desencriptar
void desencriptarNivelAperturaItems(char* cadena); //7
//normalizar
void normalizarNivelAperturaItems(char* cadena);//8
bool  normalizarNGAItems( char *str);//otra forma
//calificador
void generarClasificadorItems(RegistroICC* registro);//9
//unificar
int vectorCopiar(Vector* vRegistro, const Vector* vItems);//10
int compararRegistrosICC(const void* a, const void* b);
int ordenClasif(const char* clasif);
//Variacion mensual
bool calculoVariacionMensual(Vector *vec);//11
bool mismoMesAnioAnterior(const char *fechaActual, const char *fechaAnterior, char * tipoVariacion);
//variacion Interanual
bool calculoVariacionInteranual(Vector *vec);//12
//grabar vector
int vectorGrabar(const Vector* vector, const char* nombreArch);
int grabarRegistrosEnBinario(const Vector* vectorOrigen, const char* nombreArchSalida);
int mostrarArchivoBinario(const char* nombreArchBinario);


int main()
{
    system("chcp 1252 >nul");//para los acentos

    Vector vRegistroICC;
    Vector vRegistroItems;
    printf("\n--- Procesando el primer archivo: %s ---\n\n", NOM_ARCH_ICC);
    int res = cargarRegistrosDesdeCSVGen(NOM_ARCH_ICC,&vRegistroICC,parsearLineaRegistroICC);

    if(res == TODO_OK)
    {
        // Iterar sobre el vector y aplicar la desencriptación a cada registro
        for (int i = 0; i < vRegistroICC.ce; i++)
        {
            RegistroICC* reg = (RegistroICC*)((char*)vRegistroICC.vec + (i * vRegistroICC.tamElem));
            desencriptarNivelApertura(reg->nivGenApertura);
            normalizarNivelApertura(reg->nivGenApertura);
            generarClasificador((reg));
        }

        printf("Vector '%s' cargado y procesado con %d registros.\n", NOM_ARCH_ICC, vRegistroICC.ce);
        //printf("Mostrando registros cargados:\n");
        //vectorMostrarGen(&vRegistroICC, imprimirRegistroICC);
    }
    else
    {
        printf("Error al cargar los registros. Código: %d\n", res);
    }
    //2do archivo
    printf("\n--- Procesando el segundo archivo: %s ---\n\n", NOM_ARCH_ITEMS_OBRA);

    int res2 = cargarRegistrosDesdeCSVGen(NOM_ARCH_ITEMS_OBRA, &vRegistroItems, parsearLineaRegistroICC);

    if(res2 == TODO_OK)
    {
        for (int i = 0; i < vRegistroItems.ce; i++)
        {
            RegistroICC* reg = (RegistroICC*)((char*)vRegistroItems.vec + (i * vRegistroItems.tamElem));
            desencriptarNivelAperturaItems(reg->nivGenApertura);
            normalizarNivelAperturaItems(reg->nivGenApertura);
            //normalizarNGAItems(reg->nivGenApertura);
            generarClasificadorItems(reg);
        }
        printf("Vector '%s' cargado y procesado con %d registros.\n", NOM_ARCH_ITEMS_OBRA, vRegistroItems.ce);
        //printf("Mostrando registros del archivo de items de obra:\n");
        //vectorMostrarGen(&vRegistroItems, imprimirRegistroICC);
    }
    else
    {
        printf("Error al cargar los registros del archivo de items de obra. Código: %d\n", res2);
    }
    vectorCopiar(&vRegistroICC,&vRegistroItems);

    vectorDestruir(&vRegistroItems);
    //ordenar
    qsort(vRegistroICC.vec, vRegistroICC.ce, vRegistroICC.tamElem, compararRegistrosICC);
    //calculos
    calculoVariacionMensual(&vRegistroICC);
    calculoVariacionInteranual(&vRegistroICC);
    //mostrar vector --> remplazar por guardar vector
    //vectorMostrarGen(&vRegistroICC, imprimirRegistroICC);
    //vectorGrabar(&vRegistroICC,NOM_ARCH_UNIDO);//falta grabar por tipo de variable

    // --- Grabar el vector procesado en el archivo binario ---
    printf("\n--- Grabando el vector procesado en archivo binario: %s ---\n", NOM_ARCH_UNIDO);
    int resGrabar = grabarRegistrosEnBinario(&vRegistroICC, NOM_ARCH_UNIDO);

    if (resGrabar == TODO_OK)
    {
        printf("Archivo binario generado exitosamente.\n");
    }
    else
    {
        printf("Error al generar el archivo binario. Código: %d\n", resGrabar);
    }

    mostrarArchivoBinario(NOM_ARCH_UNIDO);//solo para mostrar como quedo guardado


    vectorDestruir(&vRegistroICC);

    return 0;
}
int cargarRegistrosDesdeCSV(const char* nomArch, Vector* vecRegistros)
{
    FILE* archivo = fopen(nomArch, "rt");
    if (!archivo)
    {
        printf("Error al abrir el archivo %s\n",nomArch);
        return ERR_ARCHIVO;
    }

    // Asegurarse de que el vector esté creado antes de usarlo
    if (!vectorCrear(vecRegistros, sizeof(RegistroICC)))
    {
        fclose(archivo);
        return NO_HAY_MEM;
    }

    char linea[TAM_LINEA];
    RegistroICC nuevoRegistro; // Variable temporal para un solo registro

    // La primera línea la salteamos
    fgets(linea, TAM_LINEA, archivo);

    // Leer el resto del archivo línea por línea
    while (fgets(linea, TAM_LINEA, archivo) != NULL)
    {
        // Parsear la línea y llenar la estructura nuevoRegistro
        if (parsearLineaRegistroICC(linea, &nuevoRegistro) == TODO_OK)
        {
            // Insertar la estructura completa en tu vector genérico
            if (vectorInsertarAlFinal(vecRegistros, &nuevoRegistro) != TODO_OK)
            {
                printf("Error al insertar registro en el vector");
                fclose(archivo);
                vectorDestruir(vecRegistros); // Liberar la memoria del vector antes de salir
                return NO_HAY_MEM;
            }
        }
        else
        {
            printf("Advertencia: Línea con formato incorrecto: %s\n", linea);
            // Podrías decidir si este error debería detener el programa o solo advertir.
        }
    }

    fclose(archivo);
    return TODO_OK;
}
void imprimirRegistroICC(const void* elem)
{
    const RegistroICC* e = ( const RegistroICC*) elem;
    printf("%11s | %41s | %8.2lf | %14s | %8.2lf | %8.2lf\n",e->periodo,e->nivGenApertura,e->indice,e->clasificador,e->var_mensual,e->var_interanual);
}
int parsearLineaRegistroICC(char* linea, RegistroICC* registro)
{
    // Eliminar el salto de línea al final, si existe
    linea[strcspn(linea, "\r\n")] = '\0';

    char lineaAux[TAM_LINEA];
    strcpy(lineaAux, linea);

    char* ptr;

    //  Periodo (DD/MM/AAAA)
    ptr = strtok(lineaAux, ";");
    if (ptr == NULL)
    {
        return ERR_FORMATO_LINEA;
    }
    eliminarComillas(ptr);
    convertirFecha(ptr, registro->periodo);

    //  Nivel General y Aperturas
    ptr = strtok(NULL, ";");
    if (ptr == NULL)
    {
        return ERR_FORMATO_LINEA;
    }
    eliminarComillas(ptr);
    strncpy(registro->nivGenApertura, ptr, TAM_NIV_GEN_APERTURA - 1);
    registro->nivGenApertura[TAM_NIV_GEN_APERTURA - 1] = '\0'; // Asegurar terminación nula

    // Tercer token: Indice
    ptr = strtok(NULL, ";");
    if (ptr == NULL)
    {
        return ERR_FORMATO_LINEA;
    }
    // Reemplazar la coma por punto para atof
    reemplazarComaPorPunto(ptr);
    registro->indice = atof(ptr); // Convertir a double

    return TODO_OK;
}
void eliminarComillas(char* str)
{
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        // Mueve el contenido para eliminar la comilla inicial
        memmove(str, str + 1, len - 2);
        // Pon el terminador nulo donde estaba la comilla final
        str[len - 2] = '\0';
    }
}
void convertirFecha(char *fOrig, char *fConv)
{
    int dia, mes, anio;
    sscanf(fOrig, "%d/%d/%d", &dia, &mes, &anio);
    sprintf(fConv, "%04d-%02d-%02d", anio, mes, dia);
}
//en caso de usar una estructura fecha
// Función para convertir de string "DD/MM/YYYY" a la estructura Fecha
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

void reemplazarComaPorPunto(char *indice)
{
    //Buscos carácter nulo de terminación
    for (int i = 0; indice[i] != '\0'; i++)
    {
        if (indice[i] == ',')
        {
            indice[i] = '.'; // Si encuentra una coma, la reemplaza por un punto
        }
    }
}
int mostrarArchivo(const char* nomArch)
{
    FILE* archivo = fopen(nomArch, "rt");

    if(!archivo)
    {
        printf("Error al abrir el archivo\n");
        return ERR_ARCHIVO;
    }

    printf("Contenido del archivo %s:\n",nomArch);
    printf("----------------------------------------------------\n");

    char linea[TAM_LINEA];

    fgets(linea,TAM_LINEA,archivo);

    while (!feof(archivo))
    {
        puts(linea);
        fgets(linea,TAM_LINEA,archivo);
    }

    printf("----------------------------------------------------\n");
    printf("Fin del archivo.\n");

    // Cierra el archivo
    fclose(archivo);

    return TODO_OK;
}
void desencriptarNivelApertura(char* cadena)
{
    int pos = 0;
    while (cadena[pos] != '\0')
    {
        char caracter = cadena[pos];

        if (isalpha(caracter))
        {
            // Solo procesar si es una letra
            char base; // 'a' o 'A' dependiendo si es minúscula o mayúscula
            int desplazamiento;

            if (islower(caracter))
            {
                base = 'a';
            }
            else
            {
                base = 'A';
            }

            // Determinar el desplazamiento según la posición
            if ((pos + 1) % 2 != 0)
            {
                // Posición impar
                desplazamiento = 4;
            }
            else
            {
                // Posición par
                desplazamiento = 2;
            }
            // (caracter - base) te da la posición 0-25 en el abecedario
            // + desplazamiento lo mueve
            // % 26 lo mantiene dentro del rango 0-25
            // + base lo convierte de nuevo a ASCII
            cadena[pos] = base + ((caracter - base + desplazamiento) % 26);
        }
        // Si no es alfabético se mantiene igual
        pos++;
    }
}
void normalizarNivelApertura(char* cadena)
{
    SecuenciaPalabras secL, secE;
    Palabra pal;
    char buffer[TAM_NIV_GEN_APERTURA];

    secuenciaPalabrasCrear(&secL, cadena); // secL lee de la cadena original
    secuenciaPalabrasCrear(&secE, buffer); // secE escribe en el buffer
    secuenciaPalabrasLeer(&secL, &pal);
    bool esPrimeraPalabra = true; // Flag para manejar la capitalización de la primera palabra

    // mientras no sea fin de las palabras
    while(!secuenciaPalabrasFin(&secL))
    {
        if (esPrimeraPalabra)
        {
            palabraATitulo(&pal);
            esPrimeraPalabra = false;
        }
        else
        {
            palabraAMinuscula(&pal);
        }

        secuenciaPalabrasEscribir(&secE, &pal);

        //si se pudo leer
        if (secuenciaPalabrasLeer(&secL, &pal))
        {
            secuenciaPalabrasEscribirCaracter(&secE, ' '); // Añade el espacio
        }
    }

    secuenciaPalabrasCerrar(&secE);

    // copiamos el buffer a la cadena principal
    strncpy(cadena, buffer, TAM_NIV_GEN_APERTURA - 1);
    cadena[TAM_NIV_GEN_APERTURA - 1] = '\0'; //terminación nula
}
void palabraAMinuscula(Palabra* pal)
{
    char *iPal = pal->vPal;
    while (*iPal != '\0')
    {
        *iPal = tolower(*iPal);
        iPal++;
    }
}
void generarClasificador(RegistroICC* registro)
{
    //Retorna 0 si son iguales.
    if (strcmp(registro->nivGenApertura, "Nivel general") == 0)
    {
        // Usamos strncpy para evitar desbordamiento de buffer
        strncpy(registro->clasificador, "Nivel general", TAM_CLASIFICADOR - 1);
        registro->clasificador[TAM_CLASIFICADOR - 1] = '\0'; // terminación nula
    }
    else
    {
        // Si es distinto, asignamos "Capitulos"
        strncpy(registro->clasificador, "Capitulos", TAM_CLASIFICADOR - 1);
        registro->clasificador[TAM_CLASIFICADOR - 1] = '\0'; // terminación nula
    }
}
int cargarRegistrosDesdeCSVGen(const char* nomArch, Vector* vecRegistros, ParsearLineaFunc parsear)
{
    FILE* archivo = fopen(nomArch, "rt");
    if (!archivo)
    {
        printf("Error al abrir el archivo %s\n", nomArch);
        return ERR_ARCHIVO;
    }

    if (!vectorCrear(vecRegistros, sizeof(RegistroICC)))
    {
        fclose(archivo);
        return NO_HAY_MEM;
    }

    char linea[TAM_LINEA];
    RegistroICC nuevoRegistro;

    // La primera línea la salteamos
    fgets(linea, TAM_LINEA, archivo);

    // Leer el resto del archivo línea por línea
    while (fgets(linea, TAM_LINEA, archivo) != NULL)
    {
        // Usar la función de parseo pasada como argumento
        if (parsear(linea, &nuevoRegistro) == TODO_OK)
        {
            if (vectorInsertarAlFinal(vecRegistros, &nuevoRegistro) != TODO_OK)
            {
                printf("Error al insertar registro en el vector");
                fclose(archivo);
                vectorDestruir(vecRegistros);
                return NO_HAY_MEM;
            }
        }
        else
        {
            printf("Advertencia: Línea con formato incorrecto en %s: %s\n", nomArch, linea);
        }
    }

    fclose(archivo);
    return TODO_OK;
}
void desencriptarNivelAperturaItems(char* cadena)
{
    char* ptr = cadena; // Usamos un puntero para recorrer la cadena

    while (*ptr != '\0') // Recorremos la cadena hasta el caracter nulo
    {
        switch (*ptr) // Evaluamos cada caracter
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
            //si ningun caracter coinside este se mantiene como está.
        }
        ptr++;
    }
}
void normalizarNivelAperturaItems(char* cadena)
{
    char* primerGuion = strchr(cadena, '_');//buscamos comienzo de guion
    char buffer[TAM_NIV_GEN_APERTURA]; // Buffer temporal para la normalización

    //Quita todo lo anterior al primer guión bajo
    if (primerGuion != NULL)
    {
        // Si hay guion, la nueva cadena comienza despues de el guion
        strcpy(buffer, primerGuion + 1);
    }
    else
    {
        // Si no hay guion, la cadena solo se copia
        strcpy(buffer, cadena);
    }

    //Quitar guiones bajos siguientes y normalizar
    SecuenciaPalabras secL, secE;
    Palabra pal;

    secuenciaPalabrasCrear(&secL, buffer); //le del buffer
    secuenciaPalabrasCrear(&secE, cadena);      //escribe directamente en 'cadena'

    secuenciaPalabrasLeer(&secL, &pal);
    bool esPrimeraPalabra = true; // Flag para de la primera palabra

    //mientras no sea fin de las palabras
    while(!secuenciaPalabrasFin(&secL))
    {
        if (esPrimeraPalabra)
        {
            palabraATitulo(&pal);
            esPrimeraPalabra = false;
        }
        else
        {
            palabraAMinuscula(&pal);
        }

        secuenciaPalabrasEscribir(&secE, &pal);

        //si no se pudo leer
        if (secuenciaPalabrasLeer(&secL, &pal))
        {
            secuenciaPalabrasEscribirCaracter(&secE, ' '); // Añade el espacio
        }
    }

    secuenciaPalabrasCerrar(&secE);
}
//sino hay _ no normaliza
//si esta todo en mayuscula todo queda en mayuscula
//pero para nuestro archivo funciona
bool  normalizarNGAItems( char *str)
{
    char *primerGuion= strchr(str,'_');
    if(primerGuion==NULL)
        return false;
    memmove(str,primerGuion+1, strlen(primerGuion));

    *str=toupper(*str);

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
void generarClasificadorItems(RegistroICC* registro)
{
    strcpy(registro->clasificador, "Ìtems");
}
int vectorCopiar(Vector* vRegistro, const Vector* vItems)
{

    if (!vRegistro || !vItems || vItems->tamElem != vRegistro->tamElem)
        return ERROR_VEC;

    for (size_t i = 0; i < vItems->ce; i++)
    {
        void* elem = (char*)vItems->vec + i * vItems->tamElem;
        if (vectorInsertarAlFinal(vRegistro, elem))
        {
            puts("Error al copiar elemento");
            return NO_HAY_MEM;
        }
    }

    return TODO_OK;
}
int compararRegistrosICC(const void* a, const void* b)
{
    const RegistroICC* r1 = (const RegistroICC*)a;
    const RegistroICC* r2 = (const RegistroICC*)b;

    int cmpFecha = strcmp(r1->periodo, r2->periodo);
    if (cmpFecha != 0)
        return cmpFecha;

    return ordenClasif(r1->clasificador) - ordenClasif(r2->clasificador);
}

int ordenClasif(const char* clasif)
{
    if (strcmp(clasif, "Nivel general") == 0) return 0;
    if (strcmp(clasif, "Capitulos") == 0) return 1;
    return 2; // ítems u otros
}
bool mismoMesAnioAnterior(const char *fechaActual, const char *fechaAnterior, char * tipoVariacion)
{

    int anio1, mes1, dia1;
    sscanf(fechaActual, "%d-%d-%d", &anio1, &mes1, &dia1);

    int anio2, mes2, dia2;
    sscanf(fechaAnterior, "%d-%d-%d", &anio2, &mes2, &dia2);



    if(strcmp(tipoVariacion,"INTERANUAL")==0)
        return (mes1 == mes2) && (anio1 == anio2 + 1);


    if(strcmp(tipoVariacion, "MENSUAL") == 0)
    {
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
                    strcmp(regActual->nivGenApertura, regAnterior->nivGenApertura) == 0)
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
                    strcmp(regActual->nivGenApertura, regAnterior->nivGenApertura) == 0)
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
int vectorGrabar(const Vector* vector, const char* nombreArch)
{
    FILE* arch = fopen(nombreArch, "wb");
    if (!arch)
    {
        printf("Error al abrir %s", nombreArch);
        return ERR_ARCHIVO;
    }

    for (int i = 0; i < vector->ce; i++)
    {
        fwrite(vector->vec + i * vector->tamElem, vector->tamElem, 1, arch);
    }

    fclose(arch);
    return TODO_OK;
}
int grabarRegistrosEnBinario(const Vector* vectorOrigen, const char* nombreArchSalida)
{
    FILE* arch = fopen(nombreArchSalida, "wb");
    if (!arch)
    {
        printf("Error al abrir el archivo binario de salida: %s\n", nombreArchSalida);
        return ERR_ARCHIVO;
    }

    RegistroSalida regSalida;

    //para cada registro
    for (int i = 0; i < vectorOrigen->ce; i++)
    {
        const RegistroICC* regICC = (const RegistroICC*)((char*)vectorOrigen->vec + (i * vectorOrigen->tamElem));

        //Registro para "indice_icc"
        strcpy(regSalida.periodo, regICC->periodo);
        strcpy(regSalida.clasificador, regICC->clasificador);
        strcpy(regSalida.nivel_general_aperturas, regICC->nivGenApertura);
        strcpy(regSalida.tipo_variable, "indice_icc");
        regSalida.valor = regICC->indice;
        fwrite(&regSalida, sizeof(RegistroSalida), 1, arch);//1er registro

        //Registro para "var_mensual"
        strcpy(regSalida.periodo, regICC->periodo);
        strcpy(regSalida.clasificador, regICC->clasificador);
        strcpy(regSalida.nivel_general_aperturas, regICC->nivGenApertura);
        strcpy(regSalida.tipo_variable, "var_mensual");
        regSalida.valor = regICC->var_mensual;
        fwrite(&regSalida, sizeof(RegistroSalida), 1, arch);//2do registro

        //Registro para "var_interanual"
        strcpy(regSalida.periodo, regICC->periodo);
        strcpy(regSalida.clasificador, regICC->clasificador);
        strcpy(regSalida.nivel_general_aperturas, regICC->nivGenApertura);
        strcpy(regSalida.tipo_variable, "var_interanual");
        regSalida.valor = regICC->var_interanual;
        fwrite(&regSalida, sizeof(RegistroSalida), 1, arch);//3er registro
    }

    fclose(arch);
    return TODO_OK;
}
int mostrarArchivoBinario(const char* nombreArchBinario)
{
    FILE* arch = fopen(nombreArchBinario, "rb");
    if (!arch)
    {
        printf("Error: No se pudo abrir el archivo binario '%s' para lectura.\n", nombreArchBinario);
        return ERR_ARCHIVO;
    }

    RegistroSalida reg; // Instancia para leer cada registro binario

    printf("\n--- Contenido del archivo binario: %s ---\n", nombreArchBinario);
    printf("%-10s | %-15s | %-41s | %-15s | %-10s\n",
           "Periodo", "Clasificador", "Nivel Gral. Aperturas", "Tipo Variable", "Valor");
    printf("--------------------------------------------------------------------------------------------------\n");

    int cantReg = 0;
    // Leer registros hasta el final del archivo
    while (fread(&reg, sizeof(RegistroSalida), 1, arch) == 1)
    {
        printf("%-10s | %-15s | %-41s | %-15s | %-10.4f\n",
               reg.periodo,
               reg.clasificador,
               reg.nivel_general_aperturas,
               reg.tipo_variable,
               reg.valor);
        cantReg++;
    }

    if (feof(arch)) // Si se alcanzó el final del archivo correctamente
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
    return TODO_OK;
}
