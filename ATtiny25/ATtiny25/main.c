
#define  F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void sender(uint8_t,uint8_t);
void sendByte(uint8_t);

uint8_t counter=0;
uint8_t baseband=0;
uint8_t phase=0;

uint8_t  tmp=0;
uint16_t temp1=0;
uint16_t temp2=0;

ISR(TIMER0_OVF_vect)
{
	counter++;
	
	if(counter == 17 )
	{
		baseband +=1;
		counter=0;
	}
	
}

int main(void)
{
	
	TCCR0B = 0b00000001  ;
	TCCR0A |= (1<<WGM00 ) | (1<<WGM01 ) | (1<<COM0A1);
	DDRB |= 0xff;
	TIMSK |= (1<<TOIE0); //interrupcion del TOV0
	sei();
	
	while (1)
	{
		sender(0x05,0x03);
		_delay_ms(100);
	}
}



void sender(uint8_t address , uint8_t command)
{
	
	/*
	temp1 = address | ~address<<8;
	temp2 = command | ~command<<8;*/
	
	baseband=0;						//SOF
	while(baseband<16)OCR0A=128;	//SOF
	baseband=0;						//SOF
	while(baseband<8)OCR0A=255;		//SOF
	
	sendByte(address);
	sendByte(~address);
	sendByte(command);
	sendByte(~command);
/*for(uint8_t i=0;i<16;i++)
{
	tmp = (temp2 & (1<<i))>>i;

	baseband=0;						//tren de 560uS
	while(baseband<1)OCR0A=128;		//
	
	phase = tmp ? 3:1;
	
	baseband=0;						// 1
	while(baseband<phase)OCR0A=255;		//
}*/
}

void sendByte(uint8_t ad)
{

for(uint8_t i=0;i<8;i++)
{
	tmp = (ad & (1<<i))>>i;

	baseband=0;						//tren de 560uS
	while(baseband<1)OCR0A=128;		//
	
	phase = tmp ? 3:1;
	
	baseband=0;						// 1
	while(baseband<phase)OCR0A=255;		//
}

}
