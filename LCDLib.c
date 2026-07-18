#include "LCDLib.h"
#include <stdio.h>

static void delay(int cycles)
{
    volatile int i;
    for (i = 0; i < cycles; i++) { }
}

static void enable_gpio_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (port == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (port == GPIOF) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
}

static void config_pin(GPIO_TypeDef *port, int pin)
{
    enable_gpio_clock(port);
    port->MODER   &= ~(1 << ((pin * 2) + 1));
    port->MODER   |=  (1 <<  (pin * 2));
    port->OTYPER  &= ~(1 <<   pin);
    port->OSPEEDR |=  (1 << ((pin * 2) + 1)) | (1 << (pin * 2));
    port->PUPDR   &= ~(1 <<   pin);
}

static void write_pin(GPIO_TypeDef *port, int pin, uint8_t state)
{
    if (state) port->BSRR = (1U << pin);
    else       port->BSRR = (1U << (pin + 16));
}

static void enable_strobe(void)
{
    delay(LCD_DELAY_E);
    write_pin(LCDEPort, LCDEPin, 1);
}

static void send_byte(char b)
{
    write_pin(LCDD0Port, LCDD0Pin, b & 0x01);
    write_pin(LCDD1Port, LCDD1Pin, b & 0x02);
    write_pin(LCDD2Port, LCDD2Pin, b & 0x04);
    write_pin(LCDD3Port, LCDD3Pin, b & 0x08);
    write_pin(LCDD4Port, LCDD4Pin, b & 0x10);
    write_pin(LCDD5Port, LCDD5Pin, b & 0x20);
    write_pin(LCDD6Port, LCDD6Pin, b & 0x40);
    write_pin(LCDD7Port, LCDD7Pin, b & 0x80);
    delay(LCD_DELAY_D);
    write_pin(LCDEPort, LCDEPin, 0);
}

void LCD_SendCommand(char cmd)
{
    write_pin(LCDRWPort, LCDRWPin, 0);  // write
    write_pin(LCDRSPort, LCDRSPin, 0);  // command
    enable_strobe();
    send_byte(cmd);
}

void LCD_WriteChar(char c)
{
    write_pin(LCDRWPort, LCDRWPin, 0);  // write
    write_pin(LCDRSPort, LCDRSPin, 1);  // data
    enable_strobe();
    send_byte(c);
}

void LCD_WriteString(const char *s)
{
    while (*s) LCD_WriteChar(*s++);
}

void LCD_WriteInt(int value)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", value);
    LCD_WriteString(buf);
}

void LCD_WriteFloat(float value)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%f", value);
    LCD_WriteString(buf);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
}

void LCD_SetCursor(uint8_t x, uint8_t y)
{
    LCD_SendCommand(0x80 + x + (y * 40));
}

void LCD_Init(void)
{
    config_pin(LCDEPort,  LCDEPin);
    config_pin(LCDRSPort, LCDRSPin);
    config_pin(LCDRWPort, LCDRWPin);
    config_pin(LCDD0Port, LCDD0Pin);
    config_pin(LCDD1Port, LCDD1Pin);
    config_pin(LCDD2Port, LCDD2Pin);
    config_pin(LCDD3Port, LCDD3Pin);
    config_pin(LCDD4Port, LCDD4Pin);
    config_pin(LCDD5Port, LCDD5Pin);
    config_pin(LCDD6Port, LCDD6Pin);
    config_pin(LCDD7Port, LCDD7Pin);

    LCD_SendCommand(0x38);  // 8-bit, 2 line, 5x8 font
    LCD_SendCommand(0x0E);  // display on, cursor on
    LCD_SendCommand(0x06);  // entry mode: increment, no shift
    LCD_Clear();
}
