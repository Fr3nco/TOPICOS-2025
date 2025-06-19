#include "cabeceras.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 1252 >nul"); // Configurar consola para caracteres especiales

    Vector vGeneral, vItems, vUnificado;

    // Cargar y procesar archivo general
    printf("Procesando archivo general...\n");
    if(cargarDesdeCSV(NOM_ARCH_GENERAL, &vGeneral, sizeof(RegistroICC), parsearLineaRegistroGeneralICC) != BIEN) {
        printf("Error al cargar archivo general\n");
        return 1;
    }
    agregarClasificador(&vGeneral, generarClasificadorIndiceGeneral);

    // Cargar y procesar archivo de items
    printf("Procesando archivo de items...\n");
    if(cargarDesdeCSV(NOM_ARCH_ITEMS, &vItems, sizeof(RegistroICC), parsearLineaItemsObra) != BIEN) {
        printf("Error al cargar archivo de items\n");
        vectorDestruir(&vGeneral);
        return 1;
    }
    agregarClasificador(&vItems, generarClasificadorItems);

    // Unificar los vectores
    vectorCrear(&vUnificado, sizeof(RegistroICC));
    vectorCopiar(&vUnificado, &vGeneral);
    vectorCopiar(&vUnificado, &vItems);

    // Liberar memoria de los vectores originales
    vectorDestruir(&vGeneral);
    vectorDestruir(&vItems);

    // Ordenar el vector unificado
    vectorOrdenar(&vUnificado, compararRegistrosICC);

    // Calcular variaciones

    calculoVariacionMensual(&vUnificado);
    calculoVariacionInteranual(&vUnificado);


    // Generar archivo de salida
    printf("Generando archivo de salida...\n");
    if(grabarArchivoSalida(&vUnificado, NOM_ARCH_SALIDA) == BIEN) {
        printf("Archivo generado exitosamente: %s\n", NOM_ARCH_SALIDA);
            mostrarArchivoBinario(NOM_ARCH_SALIDA);
    } else {
        printf("Error al generar archivo de salida\n");
    }

    vectorDestruir(&vUnificado);
    return 0;
}
