#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

void USART_Transmit(uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0)))
	{
	}
	UDR0 = data;
}
uint8_t bandera_trig = 1;
uint8_t distancia = 0;
uint8_t data=0;

int main(void)
{
		// CONFIGURACIÓN//////////////////////////////////////
		
		DDRB = 0xFF;
		// Configuracion de Echo pin
		//EIMSK = (1<<INT1);
		EICRA = (0<<ISC11) | (1<<ISC10);
		DDRD |= 0b10000000; 
		
		// Configuración de Timer 01  - Trigger
		// De 10us
		TCCR0A = (1<<WGM01);
		//TCCR0B = (1<<CS01) | (0<<CS00);
		OCR0A = 20;
		TIMSK0 = (1<<OCIE0A);// Habilitar el comparador 0A

		
		// Configuración de Timer 02 - Echo
		// de 58 us
		TCCR2A = (1<<WGM21);
		//TCCR2B = (1<<CS21) | (0<<CS20);
		OCR2A = 116;
		TIMSK2 = (1<<OCIE2A);// Habilitar el comparador 2A

	// USART Configuration /////////////////////
	// No habilitar el modo velocidad doble
	UCSR0A = (0<<U2X0);
	
	// Falta habilitar las interrupciones. Esta solo habilita TX y RX
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02);
	
	// UMSEL Selecciona USART Asíncrono - 
	UCSR0C = (0<<UMSEL01) | (0<<UMSEL00) | (1<<UCSZ01) | (1<<UCSZ00);
	
	// Ratio de Baudaje
	UBRR0H = 0;UBRR0L = 103; // Sin modo de doble velocidad
	//nsigned char data = 'a';
	//unsigned char enter = '\n';
	//uint8_t flag = 0; 
	
	sei();
		
		
    while (1) 
    {
		//if(flag == 0)

		//	flag=1; 
		//else
		//	USART_Transmit(enter);
		//	flag = 0;
		
		USART_Transmit(data);	
		if(bandera_trig)
		{
			PORTD  |= 0x80;
			TCCR0B = (1<<CS01);
			//TIMSK0 = (1<<OCIE0A);
			bandera_trig = 0;
		}
		
   }
}

ISR(INT1_vect)
{
	if (PIND & (1<<PIND3))
	{
		//TIMSK2 = (1<<OCIE2A);
		TCCR2B = (1<<CS21);
	}
	else
	{
		EIMSK = (0<<INT1);
		TCCR2B = (0<<CS21);
		bandera_trig=1;
		data = distancia;
		distancia = 0; 
	}
};

ISR(TIMER0_COMPA_vect)
{
	//TIMSK0 = (0<<OCIE0A);
	TCCR0B = (0<<CS01);
	PORTD  &= 0b01111111 ;
	EIMSK = (1<<INT1);
	distancia = 0;
}

ISR(TIMER2_COMPA_vect)
{
	//if(distancia>200)
	//{
		//TIMSK2 = (0<<OCIE2A);
	//	TCCR2B = (0<<CS21);
	//	distancia = 0xFF;
	//	EIMSK = (0<<INT1);
	//	bandera_trig = 1;
	//}
	//else
	//{
		distancia++;
		
	//}
}


