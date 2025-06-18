#include "fecha.h"

void ingresarFecha (Fecha *fecha)
{
    puts("Ingrese fecha (D/M/A): ");
    scanf("%d/%d/%d",&fecha->dia,&fecha->mes,&fecha->anio);
    while(!esFechaValida(fecha))   //fecha manda la direccion
    {
        //ingresa cuando no es fecha valida
        puts("FECHA INVALIDA");
        puts("Ingrese fecha (D/M/A): ");
        scanf("%d/%d/%d",&fecha->dia,&fecha->mes,&fecha->anio);
    }
}

void mostrarFecha(const Fecha *fecha)
{
    printf("Fecha:\t%02d/%02d/%04d\n",fecha->dia,fecha->mes,fecha->anio);

}

bool esFechaValida(const Fecha* fecha)
{
    if(fecha->anio >= 1582) //valida año
        if(fecha->mes >=1 && fecha->mes <= 12)  //valida mes
            if(fecha->dia >=1 && fecha->dia <= cantDiasMes(fecha->mes, fecha->anio)) //valida el dia teniendo en cuenta el mes.
                return true;    //si es verdad
    return false;   // si no se cumple
}

int cantDiasMes(int mes, int anio) //como empieza con cant retorna un entero
{
    //retorna 31 si es enero, 28 si febrero y no es bisiesto, 29 si es bisiesto, 31 marzo,30 abril,31 mayo .....
    int diasDelMes[13]= {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    //vector con el maximo de dias que tiene cada mes
    //dejamos el primero en 0 porque no tenemos un mes 0

    if(mes == 2 && esBisiesto(anio))
        return 29;
    return diasDelMes[mes];
    //diasDelMes[1] = 31,dm[3] = 31,dm[4] = 30 ....
}

bool esBisiesto(int anio) //como empieza con es retorna un booleano
{
    /*
    si el año no es divisible por 4. NO ES BISIESTO
    si el año es divisible por 4 y si el año no es divisible por 100 es BISIESTO.
    si el año es divisible por 100 y divisible por 400. es BISIESTO
    sino no es BISIESTO
    */
    return( (anio%4==0 && anio%100!=0) || anio%400==0);
}


Fecha diaSiguiente(const Fecha* fecha)
{
    Fecha fSiguiente=*fecha;
    int diasEnMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Verificar si el año es bisiesto y actualizar la cantidad de días en febrero
    if (esBisiesto(fSiguiente.anio))
    {
        diasEnMes[2] = 29;
    }

    // Incrementar el día
    fSiguiente.dia++;

    // Verificar si el día supera la cantidad de días en el mes actual
    if (fSiguiente.dia > diasEnMes[fSiguiente.mes])
    {
        fSiguiente.dia = 1;
        fSiguiente.mes++;

        // Verificar si el mes supera diciembre (incrementar año)
        if (fSiguiente.mes > 12)
        {
            fSiguiente.mes = 1;
            fSiguiente.anio++;
        }
    }
    return fSiguiente;
}
Fecha obtenerDiaSiguiente(Fecha fecha)
{
    Fecha diaSiguiente = fecha; // Inicializamos con la fecha actual

    diaSiguiente.dia++; // Incrementamos el día

    int diasEnMes = cantDiasMes(diaSiguiente.mes, diaSiguiente.anio);

    if (diaSiguiente.dia > diasEnMes)
    {
        // Si el día excede los días del mes
        diaSiguiente.dia = 1;
        diaSiguiente.mes++;
        if (diaSiguiente.mes > 12)
        {
            // Si el mes excede 12
            diaSiguiente.mes = 1;
            diaSiguiente.anio++;
        }
    }

    return diaSiguiente;
}
Fecha sumarNDias_2(Fecha fechaInicial, int n)
{
    Fecha fechaResultado = fechaInicial;
    for (int i = 0; i < n; i++)
    {
        fechaResultado = obtenerDiaSiguiente(fechaResultado);
    }
    return fechaResultado;
}
Fecha sumarNDias(const Fecha* f,const int dias)
{
    int cdm;    //cantidad del dia del mes
    Fecha fSuma = *f;  //fSuma guarda una copia de la estructura y datos mandados
    fSuma.dia+=dias;  //sumamos los dias
    while(fSuma.dia > (cdm = cantDiasMes(fSuma.mes, fSuma.anio))) //mientras que sea menor que 30, 31 , 28,29 seguira restando
    {
        fSuma.dia -= cdm; //almaceno el valor en cdm porque sino tengo q llamar a la funcion 2 veces
        fSuma.mes += 1;
        if(fSuma.mes >12) //en caso de que supere los 12 meses
        {
            fSuma.anio +=1;
            fSuma.mes =1;
        }
    }
    return fSuma;
}
Fecha restarNDias(const Fecha* f, const int dias)
{
    Fecha fResta = *f; // Guarda una copia de la fecha inicial
    int diasRestantes = dias;

    while (diasRestantes > 0)
    {
        if (fResta.dia > diasRestantes)
        {
            fResta.dia -= diasRestantes;
            diasRestantes = 0;
        }
        else
        {
            diasRestantes -= fResta.dia;
            fResta.dia = 0;
            fResta.mes--;
            if (fResta.mes < 1)
            {
                fResta.mes = 12;
                fResta.anio--;
                if (fResta.anio < 1582)
                {
                    // Considerar cómo manejar fechas anteriores al calendario Gregoriano
                    // Por ahora, simplemente detenemos la resta.
                    fResta.anio = 1582;
                    fResta.mes = 1;
                    fResta.dia = 1;
                    diasRestantes = 0;
                    return fResta;
                }
            }
            fResta.dia = cantDiasMes(fResta.mes, fResta.anio);
        }
    }
    return fResta;
}
int diferenciaEnDias(Fecha fecha1, Fecha fecha2)
{
    int dias = 0;
    Fecha menor = fecha1;
    Fecha mayor = fecha2;

    if (fecha2.anio < fecha1.anio || (fecha2.anio == fecha1.anio && fecha2.mes < fecha1.mes) || (fecha2.anio == fecha1.anio && fecha2.mes == fecha1.mes && fecha2.dia < fecha1.dia))
    {
        menor = fecha2;
        mayor = fecha1;
    }

    while (menor.anio < mayor.anio || menor.mes < mayor.mes || menor.dia < mayor.dia)
    {
        menor = obtenerDiaSiguiente(menor);
        dias++;
    }

    return dias;
}
int diasEnAnio(int anio)
{
    return esBisiesto(anio) ? 366 : 365;
}

int diasDesdeInicioAnio(Fecha fecha)
{
    int dias = fecha.dia;
    for (int mes = 1; mes < fecha.mes; mes++)
    {
        dias += cantDiasMes(mes, fecha.anio);
    }
    return dias;
}

int diferenciaEnDiasOptimizado(Fecha fecha1, Fecha fecha2)
{
    long int dias1 = 0;
    long int dias2 = 0;
    Fecha menor = fecha1;
    Fecha mayor = fecha2;

    if (fecha2.anio < fecha1.anio || (fecha2.anio == fecha1.anio && fecha2.mes < fecha1.mes) || (fecha2.anio == fecha1.anio && fecha2.mes == fecha1.mes && fecha2.dia < fecha1.dia))
    {
        menor = fecha2;
        mayor = fecha1;
    }

    // Calcular días desde el 0000 hasta la fecha menor
    for (int anio = 1; anio < menor.anio; anio++)
    {
        dias1 += diasEnAnio(anio);
    }
    dias1 += diasDesdeInicioAnio(menor);

    // Calcular días desde el 0000 hasta la fecha mayor
    for (int anio = 1; anio < mayor.anio; anio++)
    {
        dias2 += diasEnAnio(anio);
    }
    dias2 += diasDesdeInicioAnio(mayor);

    return (int)(dias2 - dias1);
}
