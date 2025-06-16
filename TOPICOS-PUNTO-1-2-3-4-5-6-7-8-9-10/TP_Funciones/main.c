#include "cabeceras.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    system("chcp 1252 >nul");
    Vector vItems;
    Vector vRegistros;


    int res = cargarDesdeCSV(
                  NOM_ARCH_GENERAL,
                  &vRegistros,
                  sizeof(RegistroICC),
                  parsearLineaRegistroICC
              );

    if (res == BIEN)
    {
        res = agregarClasificador(&vRegistros, generarClasificadorIndiceGeneral);
        if(res != BIEN)
        {
            puts("Error al agregar clasificadores");
            vectorDestruir(&vRegistros);

        }

        printf("Registros cargados: %d\n", vRegistros.ce);
//        vectorMostrarGen(&vRegistros, imprimirRegistroICC);
    }
    else
    {
        printf("\n Error al hacer la carga en el archivo : %s",NOM_ARCH_GENERAL);
    }

    puts("Archivo 2\n");
    puts("-------------------------------------------------------------------------------\n");

    int res2=cargarDesdeCSV(NOM_ARCH_ITEMS,&vItems, sizeof(RegistroICC),parsearLineaItems);
     if (res2 == BIEN)
    {
        res2 = agregarClasificador(&vItems, ClasificadorItems);
        if(res2 != BIEN)
        {
            puts("Error al agregar clasificadores");
            vectorDestruir(&vItems);

        }

        printf("Registros cargados: %d\n", vItems.ce);
//        vectorMostrarGen(&vItems, imprimirRegistroICC);
    }
    else
    {
        printf("\n Error al hacer la carga en el archivo : %s",NOM_ARCH_ITEMS);
    }

    vectorCopiar(&vRegistros, &vItems);

    vectorDestruir(&vItems);

    puts("Archivo Unificado\n");
    puts("-------------------------------------------------------------------------------\n");
    vectorOrdenar(&vRegistros, compararRegistrosICC);
    vectorMostrarGen(&vRegistros, imprimirRegistroICC);
    vectorDestruir(&vRegistros);

    return 0;
}
