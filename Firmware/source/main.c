#define F_CPU 16000000UL
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

// ??? ADC ???????????????????????????????????????????
void ADC_Init() {
    ADMUX  = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t ReadADC(uint8_t channel) {
    ADMUX &= 0xF0;
    ADMUX |= channel;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

// ??? Thresholds ????????????????????????????????????
#define BRIGHT_THRESHOLD   800
#define MEDIUM_THRESHOLD   600
#define LOW_THRESHOLD      400

// ??? Pin Definitions ???????????????????????????????
// PA1  = Override Button
// PB0  = Red LED
// PB1  = Yellow LED
// PB2  = Green LED
// PB3  = Lamp (Relay)
// PB4  = Buzzer

uint8_t manualMode = 0;
uint8_t lampState  = 0;
uint8_t btnPrev    = 1;

// ??? LCD Number Print ??????????????????????????????
void LCD_PrintNum(uint16_t num) {
    char buf[6];
    uint8_t i = 0;
    if (num == 0) { LCD_Char('0'); return; }
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        LCD_Char(buf[j]);
    }
}

// ??? Clear All Outputs ?????????????????????????????
void Clear_Outputs() {
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) |
               (1 << PB3) | (1 << PB4));
}

int main(void) {

    // ?? GPIO Init ??
    DDRA  = 0x00;
    PORTA = (1 << PA1);         // Pull-up on override button
    DDRB  = 0x1F;               // PB0-PB4 outputs
    PORTB = 0x00;
    DDRC  = 0x00;
    PORTC = 0x00;
    DDRD  = 0xFF;               // LCD
    PORTD = 0x00;

    ADC_Init();
    LCD_Init();

    // ?? System Startup ??
    LCD_SetCursor(1, 1);
    LCD_String("SMART LIGHT");
    LCD_SetCursor(2, 1);
    LCD_String("SYSTEM READY");
    _delay_ms(2000);
    LCD_Clear();

    while (1) {

        // ?? Button Edge Detection ??
        uint8_t btnNow = (PINA & (1 << PA1)) ? 1 : 0;
        if (btnPrev == 1 && btnNow == 0) {
            _delay_ms(20);
            if (!(PINA & (1 << PA1))) {

                if (!manualMode) {
                    // ?? First Press: Enter Manual Mode ??
                    manualMode = 1;
                    LCD_Clear();
                    LCD_SetCursor(1, 1);
                    LCD_String("MANUAL MODE");
                    LCD_SetCursor(2, 1);
                    LCD_String(lampState ? "LAMP: ON " : "LAMP: OFF");
                    _delay_ms(1000);

                } else {
                    // ?? Subsequent Presses: Toggle Lamp ??
                    lampState ^= 1;
                    Clear_Outputs();
                    if (lampState)
                        PORTB |= (1 << PB3);
                    else
                        PORTB &= ~(1 << PB3);

                    LCD_Clear();
                    LCD_SetCursor(1, 1);
                    LCD_String("MANUAL MODE");
                    LCD_SetCursor(2, 1);
                    LCD_String(lampState ? "LAMP: ON " : "LAMP: OFF");
                    _delay_ms(500);
                }
            }
        }
        btnPrev = btnNow;

        // ?? ADC Reading ??
        uint16_t light = ReadADC(0);

        // ?? Sensor Error ??
        if (light <= 5 || light >= 1018) {
            Clear_Outputs();
            PORTB |= (1 << PB0);            // Red LED
            PORTB |= (1 << PB4);            // Buzzer ON
            LCD_Clear();
            LCD_SetCursor(1, 1);
            LCD_String("SENSOR ERROR!");
            LCD_SetCursor(2, 1);
            LCD_String("Check LDR");
            _delay_ms(500);
            PORTB &= ~(1 << PB4);           // Buzzer OFF
            continue;
        }

        // ?? Manual Mode ??
        if (manualMode) {
            LCD_Clear();
            LCD_SetCursor(1, 1);
            LCD_String("MANUAL MODE");
            LCD_SetCursor(2, 1);
            LCD_String(lampState ? "LAMP: ON " : "LAMP: OFF");
            _delay_ms(300);
            continue;
        }

        // ?? Automatic Mode ??
        Clear_Outputs();
        LCD_Clear();
        LCD_SetCursor(1, 1);
        LCD_String("ADC:");
        LCD_PrintNum(light);
        LCD_SetCursor(2, 1);

        if (light > BRIGHT_THRESHOLD) {
            // Bright: Green LED ON, Lamp OFF
            PORTB |=  (1 << PB2);
            PORTB &= ~(1 << PB3);
            PORTB &= ~(1 << PB4);
            LCD_String("BRIGHT LAMP:OFF");

        } else if (light > MEDIUM_THRESHOLD) {
            // Medium: Yellow LED ON, Lamp OFF
            PORTB |=  (1 << PB1);
            PORTB &= ~(1 << PB3);
            PORTB &= ~(1 << PB4);
            LCD_String("MEDIUM LAMP:OFF");

        } else if (light > LOW_THRESHOLD) {
            // Low: Red LED ON, Lamp ON
            PORTB |=  (1 << PB0);
            PORTB |=  (1 << PB3);
            PORTB &= ~(1 << PB4);
            LCD_String("LOW    LAMP:ON ");

        } else {
            // Very Low: Red LED ON, Lamp ON, Buzzer Alert
            PORTB |= (1 << PB0);
            PORTB |= (1 << PB3);
            PORTB |= (1 << PB4);
            LCD_String("V.LOW  LAMP:ON ");
            _delay_ms(200);
            PORTB &= ~(1 << PB4);           // Short beep
        }

        _delay_ms(500);
    }

    return 0;
}