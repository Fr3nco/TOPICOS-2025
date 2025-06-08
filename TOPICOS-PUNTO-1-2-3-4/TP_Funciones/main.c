#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../fecha/fecha.h"
#include "../vector/vector.h"
#include "../nivGenApertura/nivGeneral.h"

#define TAM_LINEA 501
#define TAM_PERIODO 11      // "DD/MM/AAAA" o "AAAA-MM-DD" + '\0'
#define TAM_NIV_GEN_APERTURA 31   // 30 caracteres + '\0'
#define ERR_ARCHIVO 1
#define ERR_FORMATO_LINEA 10
#define NOM_ARCH_ORIG "indices_icc_general_capitulos.csv"
#define NOM_ARCH_MOD "indices_icc_general_capitulos_modificado.csv"


typedef struct
{
    char periodo[TAM_PERIODO];
    char nivGenApertura[TAM_NIV_GEN_APERTURA];
    double indice;
} RegistroICC;

void convertirFecha(char *fOrig, char *fConv);//1
void reemplazarComaPorPunto(char *indice);//2
void desencriptar(char* cadena);
int mostrarArchivo(const char* nomArch);
//Carga
int cargarRegistrosDesdeCSV(const char* nomArch, Vector* vecRegistros);
int parsearLineaRegistroICC(char* linea, RegistroICC* registro);
void eliminarComillas(char* str);
//imprimir
void imprimirRegistroICC(const void* elem);

int main()
{
    Vector vRegistro;
    int res = cargarRegistrosDesdeCSV(NOM_ARCH_ORIG,&vRegistro);

    if(res == TODO_OK)
    {
        printf("Vector cargado con %d registros.\n", vRegistro.ce);
        printf("Mostrando registros cargados:\n");
        vectorMostrarGen(&vRegistro, imprimirRegistroICC);
    }
    else
    {
        printf("Error al cargar los registros. Código: %d\n", res);
    }

    vectorDestruir(&vRegistro);

    return 0;
}
// --- Cargar registros desde el archivo CSV usando Vector genérico ---
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
        } else
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
    printf("%s,%s,%.12f\n",e->periodo,e->nivGenApertura,e->indice);
}
// Función para parsear una línea del CSV a una estructura RegistroICC
// Retorna TODO_OK si se parseó, ERR_FORMATO_LINEA si hubo problemas
int parsearLineaRegistroICC(char* linea, RegistroICC* registro)
{
    // Eliminar el salto de línea al final, si existe
    //linea[strcspn(linea_csv, "\r\n")] = '\0';

    char lineaAux[TAM_LINEA];
    strcpy(lineaAux, linea);

    char* ptr;

    //  Periodo (DD/MM/AAAA)
    ptr = strtok(lineaAux, ";");
    if (ptr == NULL) { return ERR_FORMATO_LINEA; }
    eliminarComillas(ptr);
    convertirFecha(ptr, registro->periodo);

    //  Nivel General y Aperturas
    ptr = strtok(NULL, ";");
    if (ptr == NULL) { return ERR_FORMATO_LINEA; }
    eliminarComillas(ptr);
    desencriptar(ptr);
    strncpy(registro->nivGenApertura, ptr, TAM_NIV_GEN_APERTURA - 1);
    registro->nivGenApertura[TAM_NIV_GEN_APERTURA - 1] = '\0'; // Asegurar terminación nula

    // Tercer token: Indice
    ptr = strtok(NULL, ";");
    if (ptr == NULL) { return ERR_FORMATO_LINEA; }
    // Reemplazar la coma por punto para atof
    reemplazarComaPorPunto(ptr);
    registro->indice = atof(ptr); // Convertir a double

    return TODO_OK;
}
void eliminarComillas(char* str) {
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
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

void desencriptar(char* cadena)
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
