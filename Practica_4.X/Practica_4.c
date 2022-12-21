#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)

#include <xc.h>
#include <pic18f45k50.h>

#define _XTAL_FREQ 1000000      // Frecuencia por default

char Value = 0;

void __interrupt(high_priority)  myHiIsr(void) 
{
    Value += 1; //Incrementea en uno 
    
    if (Value > 9) //Cuando llegue a nueve regresa a cero.
    {
        Value = 0;        
    }   
    INTCONbits.TMR0IF=0;
}



void Configuracion(void)
{    
    TRISA=0; //Configura el Puerto A Como salidas
    TRISD=0;
    ANSELA=0; //los pines comienzand apagados
    ANSELD=0; 
    LATA = 0;

    INTCON=0xA0;    // Habilita interrupcion de alta prioridad por TMR0
    RCONbits.IPEN = 1;  // Habilita niveles de interrupción
    T0CON=0x81;      // Habilita TMR0 con razón 1:16 0b10000011
        //Relacion de cambio de numero en el timer, controla cuantos segundos pasa de un numero a otro.
    //frecuencua de 1MHz / 4 = 62500 representa un periodo de 0.000016 micro segundos.
    //el timer trabajara a 16 bits
    //periodo de desbordamiento de 1.04 segundos
}

void main(void) 
{
    //10 bits
    const char Dis7seg[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x73};
    //posiciones:              0      1    2   3       4      5    6     7     8      9

    Configuracion();    
    while(1)
    {
       PORTD = Dis7seg[Value]; // Parpadea LED RA0
       __delay_ms(50); //value se va a estar incrementando con cada interrupcion.    
    }

    return;
}