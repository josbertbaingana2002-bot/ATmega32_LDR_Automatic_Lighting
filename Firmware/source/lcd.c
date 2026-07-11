#include "lcd.h"

static void LCD_Enable(void)
{
    LCD_PORT |= (1 << LCD_EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    _delay_ms(2);
}

void LCD_Command(unsigned char cmd)
{
    LCD_PORT &= ~(1 << LCD_RS);

    // Send upper nibble
    LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0);
    LCD_Enable();

    // Send lower nibble
    LCD_PORT = (LCD_PORT & 0x0F) | ((cmd << 4) & 0xF0);
    LCD_Enable();
}

void LCD_Char(unsigned char data)
{
    LCD_PORT |= (1 << LCD_RS);

    // Send upper nibble
    LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
    LCD_Enable();

    // Send lower nibble
    LCD_PORT = (LCD_PORT & 0x0F) | ((data << 4) & 0xF0);
    LCD_Enable();
}

void LCD_String(const char *str)
{
    while(*str)
    {
        LCD_Char(*str++);
    }
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
    _delay_ms(2);
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    unsigned char address;

    if(row == 1)
        address = 0x80 + (col - 1);
    else
        address = 0xC0 + (col - 1);

    LCD_Command(address);
}

void LCD_Init(void)
{
    LCD_DDR |= 0xF3;

    _delay_ms(20);

    LCD_Command(0x02);   // 4-bit mode
    LCD_Command(0x28);   // 2 lines, 5x8 font
    LCD_Command(0x0C);   // Display ON, Cursor OFF
    LCD_Command(0x06);   // Entry mode
    LCD_Command(0x01);   // Clear display

    _delay_ms(2);
}