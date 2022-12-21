/*
 * File:   Practica_3.c
 * Author: luisnunez.
 *
 * Created on February 17, 2021, 1:02 PM
 */

// Manejo de 2 interrupciones externas 
#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator) 
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored)) 
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled) 
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled) 
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled) 

#include <xc.h> 
#define _XTAL_FREQ 1000000      // Frecuencia por default La

//Enciende y apaga los pines del puerto D, 10 veces
void __interrupt(high_priority)  myHiIsr(void)  
{     	
    char A = 10;
    if(INTCONbits.INT0IF)
    {
        while(A)   
        {        
            PORTD = 0x33;  //Salida por el puerto A      
            __delay_ms(400);
            PORTD = 0xCC;        
            __delay_ms(400); 
            PORTD = 0x00;  //Se apagan todos los leds.    
            __delay_ms(400);        
            A -= 1;    
        }   
    }
    else
    {    
        while(A)     
        {       
            PORTD = A;
           __delay_ms(100);        
           A -= 1;     
        }     
        INTCON3bits.INT1F=0;  //Limpia la bandera de interrupcion.
    }
  
    //INTCONbits.INT0IF=0; //Limpia la bandera de interrupcion.
    //if para poder cambiar de interrupcion.
}

//----------------------------------------------------------------------------
//Despliega un contador decrementado del 10 al 1 en binario.
/*
void __interrupt(low_priority) myLoIsr(void) //Nombre de la funcion.
{    
    char A = 10;     
    while(A)     
    {       
        PORTD = A;
       __delay_ms(100);        
       A -= 1;     
    }     
    //INTCON3bits.INT1F=0; //Limpia la bandera de interrupcion.
    INTCON3bits.INT1F=0;  //Limpia la bandera de interrupcion.
}*/
//----------------------------------------------------------------------------

//Se definen los puertos A y D como salidas digitales. 
//El puerto B son entradas digitales
//RB0: activan las interrupciones de alta prioridad.
//RB1: activan las interrupciones de baja prioridad.
//Preguntar por este error.
MainLoop(void);
void Configuracion(void) 
{         
    TRISA=0;     
    TRISB=0xFF; //Carga todos los pines con 1, son entradas digitales.    
    TRISD=0;     
    ANSELA=0;     
    ANSELB=0;     
    ANSELD=0;     
    LATA = 0;     
    LATD = 0;     
    //En los registros INTCON, INTCON2: ACTIVAN interrupciones y su prioridad
    INTCON=0x08;    // Habilita interrupcion externa RBO = PIN 33 INT0  //Siempre sera de alta prioridad.
    INTCON3=0xD0;   // Habilita interrupción externa RB1 = PIN 34 INT1, baja prioridad
    RCONbits.IPEN = 0;  // Habilita niveles de interrupción alta y baja
    //IPEN: baja y alta prioridad.
} 

void main(void) 
{     
    Configuracion();     
    while(1)    
    {        
        PORTA ^= 1; // Parpadea LED RA0        
        __delay_ms(500);            
    }     
    return; 
}

