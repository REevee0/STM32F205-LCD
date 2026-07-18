#ifndef LCDLIB_H
#define LCDLIB_H

#include "stm32f2xx.h"
#include <stdint.h>

#define LCDD0Pin  D0_Pin
#define LCDD0Port D0_GPIO_Port
#define LCDD1Pin  D1_Pin
#define LCDD1Port D1_GPIO_Port
#define LCDD2Pin  D2_Pin
#define LCDD2Port D2_GPIO_Port
#define LCDD3Pin  D3_Pin
#define LCDD3Port D3_GPIO_Port
#define LCDD4Pin  D4_Pin
#define LCDD4Port D4_GPIO_Port
#define LCDD5Pin  D5_Pin
#define LCDD5Port D5_GPIO_Port
#define LCDD6Pin  D6_Pin
#define LCDD6Port D6_GPIO_Port
#define LCDD7Pin  D7_Pin
#define LCDD7Port D7_GPIO_Port
#define LCDEPin   E_Pin
#define LCDEPort  E_GPIO_Port
#define LCDRWPin  RW_Pin
#define LCDRWPort RW_GPIO_Port
#define LCDRSPin  RS_Pin
#define LCDRSPort RS_GPIO_Port

#define LCD_DELAY_E  4000
#define LCD_DELAY_D  8000

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t x, uint8_t y);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *s);
void LCD_WriteInt(int value);
void LCD_WriteFloat(float value);
void LCD_SendCommand(char cmd);

#endif
