/*
 * File:   Practica_2.c
 * Author: luisnunez.
 *
 * Created on February 15, 2021, 12:06 PM
 */

#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator) #pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored)) #pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled) #pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled) #pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled) 
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored)) 
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled) #pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled) #pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled) 
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled) 
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled) 

#include <xc.h>

#define _XTAL_FREQ 1000000      // Frecuencia por default

void Configuration(void) 
{
    
    TRISA = 0; //define si los puertos A son entradas o salidas.
    //Define los pines digitales como salidas o entradas, si esta en cero todos son salidas digitales.
    ANSELA = 0b00000000; //Los 8 bits del registro Ansel A son igual a cero, ANSEL=15, son los 4 bits mas significativos a 0, 4 bits menos significativos en 1.
    //Define que pines del puerto A van a ser digitales y cuales analogicos.
    
    ADCON0 = 0x39; //0b00111001
    ADCON1 = 0x00;
    ADCON2 = 0x90; //0b10010000: Ciclo de 2 micro segundos, tiempo de adquisicion 4TAD.
    //return;
}

int Conversion(void)
{
    ADCON0bits.GO = 1; //Se inicia la conversion.
    
    while(ADCON0bits.GO); //cuando termina la conversion 
    
    return ADRESL + ADRESH*256; //Retorna los 10 bits como enteros.
    //Carga los 8 bits menos signficativos de la conversion + los dos mas signficativos.
}

void main(void)
{
    Configuration();
    
    while(1)
    {
        PORTA = Conversion()/4; //Despliega los 8 bits mas significativos.
        //Regresa un valor entre 0 y 1023
        //En port a se visuzlaizan los 8 bits mas significativos.
        __delay_ms(100);
    }
    
    return;
}