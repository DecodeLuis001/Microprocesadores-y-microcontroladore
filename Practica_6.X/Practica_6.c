#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)

#include <xc.h>

#define _XTAL_FREQ 500000

void Configuracion(void)
{    
    TRISC=0;
    
    ANSELC=0; 
    OSCCON = 0x23; //Oscilador interno a 500Khz, se trabaja con el oscilador interno.
    
    ADCON0=0x01; //0b00000001: se selecciona el canal AN-0 y se establece el convertidor analogico digital.
    ADCON1=0x00; //0b00001110: porque se utilizan de referencia los 5v y 0v
    ADCON2=0x08; //0b10010000: utilizamos una frecuencia mas baja, se justifica a la izquierda tomando los 8 bits mas significativos.
    //Los 500 kHz se dividen entre dos, se tienen 4 TAD, en total se tienen 8 milisegundos
    
    
    T2CON=0x07;     //Configuracion del temporizador 2
    PR2=155;        //para obtener el tiempo de dos mili segundos como periodo, es un registro fijo, se carga para comparar con el timer 2.
    CCP1CON=0x0F;   //Para configurar el P.W.M.
    //bit 5 y 4: menos significativos
}

int Conversion(void)
{
    ADCON0bits.GO = 1; 
    
    while (ADCON0bits.GO);
        
    return ADRESH; // Retorna los 10 bits como int
    //regresa los 8 bits mas significativos.
}

void main(void) {
    
    char A = 0; //Se cargan los 8 bits menos significativos 
    char B = 0;
    Configuracion();
  
    while(1)
    {
        
       A = Conversion(); // Despliega los 8 bits más significativos, pueden ser 8 bits = 1 o 8 bits = 0
       A = (A>>3) + 31;    //recorriemiento: 0b00011111, este es el valor maximo.
       //el + 31 es cuando solo llegan puros 0, de esta manera el valor minimo sera de 31.
       //asi se genera un ancho de pulso de 1 mili segundo
       //con el valor precargado que se tiene de 31 + los 31 = 62, es decir un ancho de pulso de 1 a 2 milisegundos. 
       
       B = A & 0x03;     //se enmascaran los bits: b = 0B00000011
       B = B<<4; //Se les hace un corriemiento de 4 lugares a la izquierda para ocupar los bits 5 y 4 en el registro 
       //Antes del corrimiento      B = 0b00000011
       //Despues del corrimiento    B = 0b00110000
       
       //Se cargan los dos bits menos significativos en CCP1CON.
       CCP1CON = CCP1CON | B;   //Aqui solo se pueden modificar los bits 5 y 4
       A = A>>2;                //Dado que se descartan los dos bits que se cargaron en CCP1CON.
       CCPR1L = A;              //Se cargan solo los 4 bits mas significativos.
       //---->De esta forma se puede aplicar la formula del ancho de pulso.
       __delay_ms(100);       
    }

    return;
}
