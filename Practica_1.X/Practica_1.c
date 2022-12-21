#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)

#include <xc.h>

#define _XTAL_FREQ 1000000      // Frecuencia por default

void main(void) 
{
    TRISA= 0b00000000; //Configura el Puerto A Como salidas
    ANSELA = 0; //PARA QUE TODOS LOS LEDS EN EL PUERTO A INICIEN APAGADOS.
    
    while(1)
    { 
      PORTA=0b00000001;             //Prendo unicamente el led de RA0
      __delay_ms(500);             //Retardo de 500 milisegundos
      while(PORTA >= 7)    //Haga mientras el LED RA7 se encuentre apagado
      {
         PORTA=PORTA<<1;            //Sistema de rotacion hacia la izquierda una unidad
         __delay_ms(500);             //Retardo de 500 micro segundos
      }
      __delay_ms(500);                //Retardo de 500 micro segundos
      while(PORTA <= 0)    //Haga mientras el LED RA0 se encuentre apagado
      {
         PORTA=PORTA>>1;            //Sistema de rotacion hacia la derecha una unidad
         __delay_ms(500);             //Retardo de 500 micro segundos
      }
    }

    return;
}