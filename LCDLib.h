#ifndef LCDLib

#define LCDLib

#include "stm32f2xx.h"
#include "stdio.h"

#define LCDD0Pin D0_Pin
#define LCDD0Port D0_GPIO_Port
#define LCDD1Pin D1_Pin
#define LCDD1Port D1_GPIO_Port
#define LCDD2Pin D2_Pin
#define LCDD2Port D2_GPIO_Port
#define LCDD3Pin D3_Pin
#define LCDD3Port D3_GPIO_Port
#define LCDD4Pin D4_Pin
#define LCDD4Port D4_GPIO_Port
#define LCDD5Pin D5_Pin
#define LCDD5Port D5_GPIO_Port
#define LCDD6Pin D6_Pin
#define LCDD6Port D6_GPIO_Port
#define LCDD7Pin D7_Pin
#define LCDD7Port D7_GPIO_Port
#define LCDEPin E_Pin
#define LCDEPort E_GPIO_Port
#define LCDRWPin RW_Pin
#define LCDRWPort RW_GPIO_Port
#define LCDRSPin RS_Pin
#define LCDRSPort RS_GPIO_Port
#define delayBE 4000
#define delayBD 8000


void EDelay(int timeDelay)
{
	volatile int i;
	for (i = 0; i < timeDelay; i++)
	{}
}

void setPortPin(GPIO_TypeDef *port, int pinNumber)
{
	if(port == GPIOA)
	{
	    RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	if(port == GPIOB)
	{
    	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
	if(port == GPIOC)
	{
		RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
	if(port == GPIOD)
	{
		RCC ->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
	if(port == GPIOF)
	{
		RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	}

	port ->MODER &= ~(1 << ((pinNumber*2)+1));
	port ->MODER |= (1 << pinNumber*2);

	port ->OTYPER &= ~(1<< pinNumber);

	port ->OSPEEDR |= (1 << ((pinNumber*2)+1) | ((1 << pinNumber*2)));

	port ->PUPDR &= ~(1<< pinNumber);
}

void initPort()
{
	setPortPin(LCDEPort, LCDEPin);
	setPortPin(LCDRSPort, LCDRSPin);
	setPortPin(LCDRWPort, LCDRWPin);

	setPortPin(LCDD0Port, LCDD0Pin);
	setPortPin(LCDD1Port, LCDD1Pin);
	setPortPin(LCDD2Port, LCDD2Pin);
	setPortPin(LCDD3Port, LCDD3Pin);
	setPortPin(LCDD4Port, LCDD4Pin);
	setPortPin(LCDD5Port, LCDD5Pin);
	setPortPin(LCDD6Port, LCDD6Pin);
	setPortPin(LCDD7Port, LCDD7Pin);
}

void sendBit(GPIO_TypeDef *port, int pinNumber, uint8_t bitState)
{
	if (bitState)
	{
	 port ->BSRR |= (1 << pinNumber);
	}
	else
	{
	 port ->BSRR |=(1 << (pinNumber + 16));
	}
}

void sendByte(char character)
	{
		sendBit(LCDD0Port, LCDD0Pin, character & 0b00000001);
		sendBit(LCDD1Port, LCDD1Pin, character & 0b00000010);
		sendBit(LCDD2Port, LCDD2Pin, character & 0b00000100);
		sendBit(LCDD3Port, LCDD3Pin, character & 0b00001000);
		sendBit(LCDD4Port, LCDD4Pin, character & 0b00010000);
		sendBit(LCDD5Port, LCDD5Pin, character & 0b00100000);
		sendBit(LCDD6Port, LCDD6Pin, character & 0b01000000);
		sendBit(LCDD7Port, LCDD7Pin, character & 0b10000000);
		EDelay(delayBD);
		sendBit(LCDEPort, LCDEPin, 0);
	}

void enableLCD()
{
	EDelay(delayBE);
	sendBit(LCDEPort,LCDEPin,1);
}

void SetWrite()
{
	sendBit(LCDRWPort,LCDRWPin,0);
}

void SetCMD()
{
	sendBit(LCDRSPort, LCDRSPin, 0);
}

void SetCharMod()
{
	sendBit(LCDRSPort, LCDRSPin, 1);
}

void SetRead()

{
	sendBit(LCDRWPort,LCDRWPin,1);
}

void sendChar(char character)
	{
		SetWrite();
		SetCharMod();
		enableLCD();
		sendByte(character);
	}

void sendCMD(char character)
		{
			SetWrite();
			SetCMD();
			enableLCD();
			sendByte(character);
		}

void sendCLR()
	{
		sendCMD(0b00000001);
	}

void sendWrite(char *stringof)
		{
		while(*stringof){
			sendChar(*stringof++);
	       }
		}

void setLCD()
{
	sendCMD(0b00111000);
	sendCMD(0b00001110);
    sendCMD(0b00000110);
    sendCLR();
}

void sendInt(int display)
{
	char str[16];
	sprintf(str, "%d", display);
	sendWrite(str);
}
void sendFloat(float dsplay)
{
	char stre[16];
	sprintf(stre, "%f", dsplay); // @suppress("Float formatting support")
	sendWrite(stre);
}
void setCursor(uint8_t x, uint8_t y)
{
	sendCMD(0x80 + x + (y * 40));
}

#endif
