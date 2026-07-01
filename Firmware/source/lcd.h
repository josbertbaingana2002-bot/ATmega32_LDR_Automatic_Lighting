#ifndef LCD_H
#define LCD_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// LCD Control Pins
#define LCD_RS PD2
#define LCD_EN PD3

// LCD Port
#define LCD_PORT PORTD
#define LCD_DDR  DDRD

// Function Prototypes
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const char *str);
void LCD_Clear(void);
void LCD_SetCursor(unsigned char row, unsigned char col);

#endif