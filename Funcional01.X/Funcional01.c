//SENSOR DE TEMPERATURA POSITIVA: VALORES APROXIMADOS.
#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)

// CONFIG1L
#pragma config CFGPLLEN = OFF   // PLL Enable Configuration bit (PLL Disabled (firmware controlled))

// CONFIG1H
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config nPWRTEN = OFF    // Power-up Timer Enable (Power up timer disabled)
#pragma config nLPBOR = OFF     // Low-Power Brown-out Reset (Low-Power Brown-out Reset disabled)

// CONFIG4L
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1 = OFF        // Block 1 Code Protect (Block 1 is not code-protected)
#pragma config CP2 = OFF        // Block 2 Code Protect (Block 2 is not code-protected)
#pragma config CP3 = OFF        // Block 3 Code Protect (Block 3 is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protect (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)

#include<xc.h>
#include<stdio.h>
#include<pic18F45K50.h>

#define _XTAL_FREQ 8000000

//Configuracion y asignacion de pines para la LCD.
#define RS TRISDbits.RD0
#define EN TRISDbits.RD2

//Definicion de valores de los pines en la LCD
#define PIN_RS LATDbits.LD0
#define PIN_EN LATDbits.LD2
#define PIN_D4 LATDbits.LD4
#define PIN_D5 LATDbits.LD5
#define PIN_D6 LATDbits.LD6
#define PIN_D7 LATDbits.LD7

//Comandos relacionados a la LCD.
#define _LCD_CLEAR          1
#define _LCD_CURSOR_OFF     12

//Variables globales para el uso del LM35.
    //Nota: Se declaran como gobales par poder usar la funcion SensorLM35
int ADC;
float R_LM35, R_CAD;
float temperatura;
char Valores[16];

//Variable globales para el reloj desplegable
int cambio;
int minutos, horas, segundos;   //Estas variables se encargan de contmplar las divisiones de tiempo.
//Estas variables se encargan de llevar el conteo.
int Unidades_Horas, Decenas_Horas, Unidades_Minutos, Decenas_Minutos, Unidades_Segundos, Decenas_Segundos;

//Variables globales para programar la alarma.
int HORA, MINUTO, SEGUNDO;
int DH=0;   //Decenas de hora.
int UH=0;   //Unidades de hora.
int DM=0;   //Decenas de minuto.
int UM=0;   //Unidades de minuto.
int DS=0;   //Decenas de segundo.
int US=0;   //Unidades de segundo.

//Toma los datos analogicos, que vienen del pin an0
short Read_ADC(char canal) 
{
    //Voltage references.
    ADCON1bits.PVCFG = 0b00;    //Positive voltage: esta conectado a una señal interna AVdd.
    ADCON1bits.NVCFG = 0b00;    //Negative voltage: esta conectado a una señal interna AVss.
    //Convierte en analogicos los canales:
	ADCON0bits.CHS = 0b00000;	//AN0
	ADCON0bits.CHS = 0b00010;	//AN2

	switch(canal)
	{
        case 0: 
            ADCON0 = 0b00000010;    //Inicia la conversion en el canal AN-0.
            //Bit 7:    siempre se lee como 0.
            //Bit 6-2:  Se selecciona el canal analogico.
            //Bit 1:    Se inicia con la conversion.
            //Bit 0:    el ADC esta deshabilitado.
            break;
	}
	//Para configurar la velocidad, tanto del reloj como del sensor.
            //PARA FINES DE DEMOSTRACCION:
            //Recordar ajustar la frecuencia en: 10 000 000 
    //NOTA: FOSC/8 es la frecuencia minima mas estable para el tiempo de adquisicion.
	ADCON2 = 0b00100001;
	//bit 7:    justificado a la izquierda.
	//bit 6:    siempre se lee como cero.
	//bit 5-3:  8 TAD
	//bit 2-0:  FOSC/8
    //---------------------------------------------
            //En tiempo real.
            //Ajustar la frecuencia en: 2 500 000
            //Recordar quitar la restriccion de decenas y unidades de segundo en la funcion de Alarma_LED()
    /*ADCON2bits.ADFM = 0;
    ADCON2bits.ACQT = 0b011;    //6 TAD :Minimo aceptable.
     *                          //8 TAD: Recomendado.
    ADCON2bits.ADCS = 0b000;    //FOSC/2*/
    
	ADCON0bits.ADON = 1; 		//Habilita el ADC.
    
    ADCON0bits.GO = 1;          //Accede al bit numero 1, Habilita el ADC.
    while(ADCON0bits.GO);       //Hssta que el bit 1 termine la conversion.
    
    return ADRESL + ADRESH*256;//Devuelve los datos de la conversion ADC.    
    //ADRESL: Tipo entero, tiene los 8 bits menos significativos, valor maximo de 255.
    //ADRESH: tipo entero, tiene los dos bits mas significativos.
        //Se multiplica por 256, se hace un recorrimiento de 8 lugares.
    //VALOR MAXIMO A TENER: 1023, es decir 10´s unos
        //Regresa un valor variable entre 0 y 1023.
}

void Configuracion(void)
{
    /*
     * TRISX: Define los pines en los puertos como entradas = 1 o salidas = 0.
     * ANSELX: Define los puertos como digitales = 0 o analogicos = 1.
     * PORTX: Limpia el puerto correspondiente si se tiene 1 = 5v, 0 = 0v
     * LATX: Establece que en cada bit se tendra una salida de 5v o 0v.
     */
    //Configuracion relacionada al sensor LM35.
    TRISA = 0b11111111; 		//Puerto A como entradas.
	TRISD = 0b00000000; 		//puerto D como salidas.
    
    //Configuracion del oscilador.
        //Necesario para mejorar la velocidad de respuesta en la actualizacion de valores en la LCD
    OSCCON = 0b01101100;	 
	//bit 7: 		0 = Device enters Sleep mode on SLEEP instruction
	//bit 6-4:		110 = HFINTOSC/2 ? (8 MHz)
	//bit 3:		0 = Device is running from the internal oscillator (HFINTOSC or INTRC)
	//bit 2:		0 = HFINTOSC frequency is stable
	//bit 1-0:		1x = Internal oscillator block
    
    //Configuracion relacionada al reloj desplegable.
    ANSELAbits.ANSA3 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELAbits.ANSA5 = 0;
    TRISCbits.RC6 = 0;
    ANSELCbits.ANSC6 = 0;
    LATCbits.LATC6=0;
    
    //Para hacer encender los leds.
    TRISB = 0;
    ANSELB = 0;
    LATB = 0;
}

//INICIO DE LA MNIPULACION DE LA LCD
//Se hace un disparao poitivo y negativo en el pin "enable" para limpiarlo.
void Set_DATA()
{
    PIN_EN = 1;
    __delay_ms(1);
    PIN_EN = 0;
    
    return;
}

//Se obtienen los datos que se muestran en la LCD y se hace un recorrimiento para almacenarlos.
void Get_DATA(char data)
{
    PIN_D4 = data;
    PIN_D5 = data >> 1;
    PIN_D6 = data >> 2;
    PIN_D7 = data >> 3;
          
    Set_DATA();
    return;
}

//Para poder invocar los comandos de la LCD.
void LCD_COMANDOS(char comando)
{
    PIN_RS = 0;
    Get_DATA((comando >> 4)&0b00001111);
    Get_DATA((comando)&0b00001111);
    
    return;
}

void putch(char data) //Envia datos a la LCD
{   //El dato guradado de 8 bits esta en data
    char Activa;            //Guarda lo que hay en data 
    Activa = data & 0xF0;   //Deja los 4 bits mas sifnigicativos.
    LATD = Activa | 0x05;   //5: dado que RS y E tienen un 1 logico, por lo tanto RS = 1 RW = 0 E = 1 == 101 == 5. 
    //0bxxxx0101
    __delay_us(10);
    LATD = Activa | 0x01;   //La LCD tiene un disparo negativo.
    //0bxxxx0001        
    __delay_ms(1);
    Activa = (unsigned char)(data << 4);    //Los bits menos signifiativos se van a data
    LATD = Activa | 0x05;
    __delay_us(10);
    LATD = Activa | 0x01;    
}

void putcm(char data)       //Se usa para comandos.
{
    char Activa;
    Activa = data & 0xF0;   //Se tienen los 4 bits mas significativos en 1.
    LATD = Activa | 0x04;
    __delay_us(10);
    LATD = Activa;
    __delay_ms(1);
    Activa = (unsigned char)(data << 4);
    LATD = Activa | 0x04;
    __delay_us(10);
    LATD = Activa;
}

void InicializaLCD(void)
{
    __delay_ms(50);
    putcm(0x02);    // Inicializa en modo 4 bits
    __delay_ms(1);

    putcm(0x28);    // Inicializa en 2 líneas 5x7
    __delay_ms(1);

    putcm(0x2C);
    __delay_ms(1);

    putcm(0x0C);
    __delay_ms(1);

    putcm(0x06);
    __delay_ms(1);

    putcm(0x80); //Posiciona el curson en 1,1
    __delay_ms(1);
}
//FIN DE FUNCIONES DE MANIPULACION DEL LCD

//INCIO PARA MANIPULACION DEL LM35.
void SensorLM35(void)
{
    //------------------------------------------PRESICION ALTA
    //Se mantiene estable en un rango de -55 a 150°C
    ADC = Read_ADC(0)/64;           //Despliega los 8 bits menos significativos.
    R_LM35 = 500.10 / 1024;       //Formula suministraada en el datasheet del LM35.
        //500.15-499.999999: es en referencia a los 500mV que se suministran al pic para operar.
        //1023: Este dato es debido a que del puerto analogico en este caso el puerto A, se da una entrada de entre 0 a 1023 bits.
        //Es el valor maximo de lectura para el ADC
    temperatura = ADC*R_LM35;
    putcm(0x80);
    sprintf(Valores, "Temp. = %3.2f %cC",temperatura, 0xDF);
    __delay_ms(1000);    //Para mostrar una vez inicializada la LCD.
    printf(Valores);
}
//FIN PARA MANIPULACION DEL LM35.

//INICIO Manipulacion del reloj desplegable.
    //Configuramos timer 0 como contador.
    //Base de tiempo necesaria para utilizar el Timer0 como contador.
void Timer0Configuration(void)
{
    T0CONbits.T08BIT = 1;   //El timer 0 estara en modo de 8 bits.
    T0CONbits.T0CS = 1;     //Habilitamos el modo contador.
    T0CONbits.T0SE = 0;     //Incriementa de arriba a abajo.
    T0CONbits.PSA = 1;      //Deshabilitamos la asignacion de preescala.
    T0CONbits.T0PS = 0b000; //Como la preescala no importa se deja en ceros.
    
    TMR0L = 0xFF;           //Valor = 255: esto hace que el timer 0 se desborde cada que se preciona el pulsador. 
    T0CONbits.TMR0ON = 1;   //Habilitamos el timer 0.
    
    //Habilitamos las interurrpciones relacionadas al timer 0.
    INTCONbits.GIE = 1;         //Habilitamos interrupciones globales.
    INTCONbits.PEIE = 1;        //Habilitamosinterrupciones por perifericos en este caso los botones.
    INTCONbits.TMR0IE = 1;      //Se habilita la interrupcion por desbordamiento.
    INTCONbits.TMR0IF =0;       //Se desactiva la bandera por desbordamiento.
}

void Mostrar_Reloj()
{
	//Proceso de division donde el maximo son 23 horas /10
	Decenas_Horas = horas / 10;		//Parte del resultado 
	Unidades_Horas = horas % 10; 	//Parte  del residuo.
	Decenas_Minutos = minutos / 10;
	Unidades_Minutos = minutos % 10;
	Decenas_Segundos = segundos / 10;
	Unidades_Segundos = segundos % 10;

    //Si se quiere el formato de 24 horas.
	putcm(0xC0);
	printf("RELOJ:  %d%d:%d%d:%d%d",Decenas_Horas, Unidades_Horas, Decenas_Minutos, Unidades_Minutos, Decenas_Segundos, Unidades_Segundos);

    /*if(horas < 12)
    {
        putcm(0xC2);
        printf("%d%d:%d%d:%d%d A.M",Decenas_Horas, Unidades_Horas, Decenas_Minutos, Unidades_Minutos, Decenas_Segundos, Unidades_Segundos);
	}
    else
    {
        putcm(0xC2);
        printf("%d%d:%d%d:%d%d P.M",Decenas_Horas, Unidades_Horas, Decenas_Minutos, Unidades_Minutos, Decenas_Segundos, Unidades_Segundos);
    }*/
}

void Cronometro()
{
    Mostrar_Reloj();
    segundos = segundos + 1;
	if(segundos > 59)
	{
		segundos = 0;
		minutos = minutos + 1;
		if(minutos > 59)
		{
			minutos = 0;
			horas = horas + 1;
			if(horas > 23) //if(horas > 23)
			{
				horas=0; //horas = 0;
            }
		}
	}
    TMR0L = 0xFF;           //Valor = 255: esto hace que el timer 0 se desborde cada que se preciona el pulsador. 
    
    //Habilitamos las interurrpciones relacionadas al timer 0.
    INTCONbits.GIE = 1;         //Habilitamos interrupciones globales.
    INTCONbits.PEIE = 1;        //Habilitamosinterrupciones por perifericos en este caso los botones.
    __delay_ms(1000);
}
//FIN Manipulacion del reloj desplegable.

//INICIO DE LA PROGRAMACION DE LA ALARMA.
void Alarma()
{
    if(PORTAbits.RA1 == 0)  //Siempre que el boton concetado a RA1 no este precionado.
    {
        PORTCbits.RC6 = 0;  //Se enciende el led conectado a RC6.
        __delay_ms(300);
        INTCONbits.GIE = 0;     //Deshabilitamos las interrupciones globales.
        INTCONbits.TMR0IF = 0;  //Apagmos la bandera por desbordamiento.
        Cronometro();
    }
    else
    {
        PORTCbits.RC6 = 1;
        __delay_ms(300);
        //Desde aqui se deben incrementar las horas y minutos.
        putcm(0xC0);
        printf(" !!Modo Alarma!! ");
        __delay_ms(1000);
        //Habilitamos interupciones globales.
        INTCONbits.GIE = 1;         //Interrupciones globales permitidas.
        INTCONbits.PEIE = 1;        //Interrupciones por perifericos (Botones) activas
        while(PORTAbits.RA1 == 1)
		{
            if(PORTAbits.RA3 == 1)
			{   
                DH = HORA / 10;		//Parte del resultado 
                UH = HORA % 10; 	//Parte  del residuo.
                __delay_ms(500);
                HORA = HORA + 1;
                if(HORA > 23) //if(horas > 12)
                {
                    HORA=0; //horas = 1;
                }
            }
            
			if(PORTAbits.RA5 == 1)
            {
                DM = MINUTO / 10;
                UM = MINUTO % 10;
                __delay_ms(500);
                MINUTO = MINUTO + 1;
                if(MINUTO > 59)
                {
                    MINUTO = 0;
                }
            }
            
            putcm(0xC0);
            printf("Alarma: %d%d:%d%d:%d%d",DH, UH, DM, UM, DS, US);
            
            /*if(HORA < 12)
            {
                putcm(0xC2);
                printf("%d%d:%d%d:%d%d A.M",DH, UH, DM, UM, DS, US);
            }
            else
            {
                putcm(0xC2);
                printf("%d%d:%d%d:%d%d P.M",DH, UH, DM, UM, DS, US);
            }*/
		}
		DS = 0;
		US = 0;
		//Habilitarr interrupciones globales.
        INTCONbits.GIE = 1;     //Habilita interrupciones globales por medio del timer 0.
        INTCONbits.PEIE = 1;    //Habilita las interrupciones mediante botones.
    }
}

//Funcion encargaada de encender los leds.

void Alarma_LED(void)
{
    //Dado que se esta programado considerando tiempo real:
        //El ciclo es de 10 repeticiones dado que 1 Seg = 1000 ms, por lo tanto:
            //1 cilo de 10 repeticiones a 100 ms = 1 seg
    if(DH == Decenas_Horas && UH == Unidades_Horas && DM == Decenas_Minutos && UM == Unidades_Minutos && DS == Decenas_Segundos && US == Unidades_Segundos)
    {
        char A = 10;
        while(A)
        {
            PORTB = 0xFF;       //Se encienden todos los leds conectados al puerto B.
            __delay_ms(100);
            PORTB = 0x00;       //Se apagan todos los leds conectados al puerto B.
            __delay_ms(100);    
            A -= 1;             //Decremento del conteo.
        }
    }
    else
    {
        PORTB = 0x00;
        __delay_ms(100);
    }
}
//FIN DE LA PROGRAMACION DE LA ALARMA.

void main(void)
{        
	Configuracion();
        
    Timer0Configuration();
    
    InicializaLCD();
    LCD_COMANDOS(_LCD_CLEAR);
    LCD_COMANDOS(_LCD_CURSOR_OFF);
    
    //Inicializa el reloj 
    horas = 12;
	minutos = 59;
	segundos = 50;
    
	while(1)
	{
        SensorLM35();
        //Funcion en conjunto:
            //A) Invoca al reloj desplegable.
            //B) Resetea el reloj para programar la alarma.
            //C) Devuleve el reloj al estado posterior a accionar la alarma.
        Alarma();
        //Funcion encargada de encender los leds.
        Alarma_LED();
    }
   
    return;
}