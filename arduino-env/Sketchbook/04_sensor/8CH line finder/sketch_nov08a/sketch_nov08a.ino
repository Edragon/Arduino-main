//Robot Zero 
#define F_CPU 20000000UL	// Baby Orangutan frequency (20MHz)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Leds. Salidas.
#define	LEDR PORTB1
#define	LEDV PORTB2

//Interruptores. Entradas.
#define PULSADOR PORTB0
//Sensores. Entradas y salidas.
#define	D3 PORTD7
#define D2 PORTC5
#define D1 PORTC4
#define D0 PORTC3
#define I0 PORTC2
#define I1 PORTC1
#define I2 PORTC0
#define I3 PORTD4
#define LED_ON PORTD2


void inicializar_puertos(void);
void reset(void);
void M1_forward(unsigned char pwm);
void M1_reverse(unsigned char pwm);
void M2_forward(unsigned char pwm);
void M2_reverse(unsigned char pwm);
void motors_init(void);
int obtener_errorp(void);
void inicializar_timer1(void);
int obtener_errord(void);

/*********** Ajuste comportamiento robot *********/
//Constantes Regulador PD. 
int Kp = 50;  //  
int Kd = 2500; //
volatile int velocidad = 160;     
/*************************************************/

int main( void ) 
{

	char pulsador = 1;

	inicializar_puertos();
	motors_init();
	reset();

	while(pulsador != 0 )
	{
		pulsador = PINB & (1<<PULSADOR);
	}

	_delay_ms(500);

	inicializar_timer1();

	PORTD |= (1<<LED_ON); //Encendemos Sensores.
	
	M1_forward(0);		//Motor derecho. 
	M2_forward(0);	    //Motor izquierdo. 


				
	while ( 1 ) 
	{	
	}
	return 0;
}

void inicializar_puertos(void)
{
   DDRD=0x6C;     //0110 1100  
   PORTD=0x00;   
   DDRB=0x26;     //0010 0110  
   PORTB=0x00;
   DDRC=0x00;     //0000 0000
   PORTC=0x00;  
}

void reset(void)
{
	

	PORTB &= ~(1<<LEDV);
	PORTB &= ~(1<<LEDR);
	_delay_ms(300);


	PORTB |= (1<<LEDV);
	PORTB |= (1<<LEDR);
	_delay_ms(300);



	PORTB &= ~(1<<LEDV);
	PORTB &= ~(1<<LEDR); 
	_delay_ms(300);


	PORTB |= (1<<LEDV);
	PORTB |= (1<<LEDR);
	_delay_ms(300);



	PORTB &= ~(1<<LEDV);
	PORTB &= ~(1<<LEDR); 
	_delay_ms(300);


	PORTB |= (1<<LEDV);
	PORTB |= (1<<LEDR);
}

//Funciones para controlar la velocidad y direcci髇 de los 
//motores. PWM controla la velocidad, valor entre 0-255.
void M1_reverse(unsigned char pwm)
{
	OCR0A = 0;
	OCR0B = pwm;
}
void M1_forward(unsigned char pwm)
{
	OCR0B = 0;
	OCR0A = pwm;
}
void M2_forward(unsigned char pwm)
{
	OCR2A = pwm;
	OCR2B = 0;
}
void M2_reverse(unsigned char pwm)
{
	OCR2B = pwm;
	OCR2A = 0;
}

//Configuraci髇 del hardware del micro que controla los motores.
void motors_init(void)
{
	// configure for inverted PWM output on motor control pins:
	// set OCxx on compare match, clear on timer overflow
	// Timer0 and Timer2 count up from 0 to 255
	TCCR0A = TCCR2A = 0xF3;
	// use the system clock/8 (=2.5 MHz) as the timer clock
	TCCR0B = TCCR2B = 0x02;
	// initialize all PWMs to 0% duty cycle (braking)
	OCR0A = OCR0B = OCR2A = OCR2B = 0;
	// set PWM pins as digital outputs (the PWM signals will not
	// appear on the lines if they are digital inputs)
	DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);
	DDRB |= (1 << PORTB3);
}

void inicializar_timer1(void) //Configura el timer y la interrupci髇.
{
    OCR1A= 0x0138; // 1 ms. 0C35 10ms, 0x0271 2ms.
    TCCR1B |=((1<<WGM12)|(1<<CS11)|(1<<CS10));    //Los bits que no se tocan a 0 por defecto
    TIMSK1 |= (1<<OCIE1A);
    sei();
}

int obtener_errorp(void)
{
	char errorp=0;
	static char ultimo_errorp=0;
	char contador_sensor=0;

	if(((PINC & 0x04) != 0) && ((PINC & 0x08) != 0))
	{
		errorp=0;
		return(0);
	 }

	if((PIND & 0x10) != 0) //I3 PD4 -7
	{
		errorp = errorp - 0x07;
		contador_sensor++;
	}

	if((PINC & 0x01) != 0) //I2 PC0 -5
	{
		errorp = errorp - 0x05;
		contador_sensor++;
	}

	if((PINC & 0x02) != 0) //I1 PC1 -3
	{
		errorp = errorp - 0x03;
		contador_sensor++;
	}

	if((PINC & 0x04) != 0) //I0 PC2 -1
	{
		errorp = errorp - 0x01;
		contador_sensor++;
	}

	if((PINC & 0x08) != 0) //D0 PC3 +1
	{
		errorp = errorp + 0x01;
		contador_sensor++;
	}

	if((PINC & 0x10) != 0) //D1 PC4 +3
	{
		errorp = errorp + 0x03;
		contador_sensor++;
	}

	if((PINC & 0x20) != 0) //D2 PC5 +5
	{
		errorp = errorp + 0x05;
		contador_sensor++;
	}

	if((PIND & 0x80) != 0) //D3 PD7 +7
	{
		errorp = errorp + 0x07;
		contador_sensor++;
	}

	if(contador_sensor != 0)
	{
		errorp = errorp / contador_sensor;
		ultimo_errorp = errorp;
		return(Kp * (int)errorp);
	}
	else
	{
		if(ultimo_errorp < 0)
			errorp = -0x09;
		else
			errorp = 0x09;

		ultimo_errorp = errorp;
		return((int)errorp * Kp);
	}		
}

int obtener_errord(void)
{
	int error = 0;
	static int error_old = 0;
	static int errord=0;
	static int errord_old = 0;
	static int tic = 1;  // 1
	static int tic_old = 1; // 


	int diferencia = 0;

	if(((PINC & 0x04) != 0) && ((PINC & 0x08) != 0))
		error=0;

	else if((PINC & 0x08) != 0) //D0 PC3 +1
		error = 1;

	else if((PINC & 0x04) != 0) //I0 PC2 -1
		error = -1;

	else if((PINC & 0x10) != 0) //D1 PC4 +3
		error = 3;

	else if((PINC & 0x02) != 0) //I1 PC1 -3
		error = -3;

	else if((PINC & 0x20) != 0) //D2 PC5 +5
		error = 5;

	else if((PINC & 0x01) != 0) //I2 PC0 -5
		error = -5;

	else if((PIND & 0x80) != 0) //D3 PD7 +7
		error = 7; 

	else if((PIND & 0x10) != 0) //I3 PD4 -7
		error = -7;

	else
		{
			if (error_old < 0)
				error = -9;
			else if(error_old > 0)
				error = 9;
		}

	//C醠culo de la velocidad media del error.
	if (error == error_old)
	{
		tic = tic + 1;
		if(tic > 30000)
			tic = 30000;
		if(tic > tic_old)
			errord = errord_old/tic;
//		if(tic > tic_old)
//			errord = (errord_old*tic_old)/tic;

	}
	else
	{
		tic++;
		diferencia = error - error_old;
		errord = Kd*(diferencia)/tic; //error medio
		errord_old = errord;
		tic_old=tic;
		tic=0;
	}

	error_old = error;
	return(errord);
}

ISR(TIMER1_COMPA_vect)
{
	int errort=0;
	int proporcional = obtener_errorp();
	int derivativo = obtener_errord();


	errort = proporcional + derivativo;


	if(errort > velocidad)
		errort = velocidad;
	else if(errort < - velocidad)
		errort = - velocidad;
	
	if(errort>0)
	{
    	M1_forward(velocidad - errort);     //Motor derecho.
        M2_forward(velocidad);              //Motor izquierdo.
	    PORTB |= (1<<LEDV);
		PORTB &= ~(1<<LEDR);
	}
	else if(errort<0)
	{
    	M1_forward(velocidad);              //Motor derecho.  
        M2_forward(velocidad + errort);     //Motor izquierdo. 
	    PORTB |= (1<<LEDR);
		PORTB &= ~(1<<LEDV);
	}

	else
	{
    	M2_forward(velocidad);       
        M1_forward(velocidad);
		PORTB &= ~(1<<LEDR);
	    PORTB &= ~ (1<<LEDV);
	}



    TIFR1 |= (1<<OCF1A);
}
