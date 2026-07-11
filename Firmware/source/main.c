#define F_CPU 16000000UL
#include "lcd.h"
#include <avr/io.h>
#include <stdlib.h> 
#include <util/delay.h>

// --- ADC Initialization ---
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

// --- Thresholds ---
#define BRIGHT_THRESHOLD   800
#define MEDIUM_THRESHOLD   600
#define LOW_THRESHOLD      400

// --- Pin Definitions ---
uint8_t manualMode = 0;
uint8_t lampState  = 0;
uint8_t btnPrev    = 1;
uint16_t prevLight = 9999; 
uint8_t prevMode   = 255;  

// --- LCD Number Print ---
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

// --- Clear All Outputs ---
void Clear_Outputs() {
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4));
}

int main(void) {
    // GPIO Initialization
    DDRA  = 0x00;
    PORTA = (1 << PA1);         // Pull-up on override button
    DDRB  = 0x1F;               // PB0-PB4 outputs
    PORTB = 0x00;
    DDRC  = 0x00;
    PORTC = 0x00;
    DDRD  = 0xFF;               // LCD Data/Control lines
    PORTD = 0x00;

    ADC_Init();
    LCD_Init();

    // System Startup Screen
    LCD_SetCursor(1, 1);
    LCD_String("SMART LIGHT");
    LCD_SetCursor(2, 1);
    LCD_String("SYSTEM READY");
    _delay_ms(2000);
    LCD_Clear();

    while (1) {
        // --- Button Edge Detection ---
        uint8_t btnNow = (PINA & (1 << PA1)) ? 1 : 0;
        if (btnPrev == 1 && btnNow == 0) { 
            _delay_ms(20);                 
            if (!(PINA & (1 << PA1))) {    
                if (!manualMode) {
                    manualMode = 1;
                    lampState = 1;         
                } else {
                    manualMode = 0;        // Pressing it again exits manual mode back to auto
                }
                prevLight = 9999;          
                _delay_ms(200); 
            }
        }
        btnPrev = btnNow;

        // --- ADC Reading ---
        uint16_t light = ReadADC(0);

        // --- Mode Logic and Pin Control ---
        if (manualMode) {
            PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)); 
            if (lampState) PORTB |= (1 << PB3);
            else           PORTB &= ~(1 << PB3);
        } 
        else {
            // Automatic Hardware Control
            if (light > BRIGHT_THRESHOLD) {
                PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4));
                PORTB |=  (1 << PB2); // Green LED ON
            } else if (light > MEDIUM_THRESHOLD) {
                PORTB &= ~((1 << PB0) | (1 << PB2) | (1 << PB3) | (1 << PB4));
                PORTB |=  (1 << PB1); // Yellow LED ON
            } else if (light > LOW_THRESHOLD) {
                PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB4));
                PORTB |=  (1 << PB0) | (1 << PB3); // Red LED and Lamp ON
            } else {
                // VERY LOW LIGHT: Triggers cleanly for anything below 401
                PORTB &= ~((1 << PB1) | (1 << PB2));
                PORTB |=  (1 << PB0) | (1 << PB3); // Red LED and Lamp ON
                
                PORTB |= (1 << PB4);  // Turn Buzzer ON
                _delay_ms(100);
                PORTB &= ~(1 << PB4); // Turn Buzzer OFF
            }
        }

        // --- Display Control ---
        int16_t diff = (int16_t)light - (int16_t)prevLight;
        if (abs(diff) > 5 || manualMode != prevMode) {
            LCD_Clear();
            
            if (manualMode) {
                LCD_SetCursor(1, 1);
                LCD_String("MANUAL MODE");
                LCD_SetCursor(2, 1);
                LCD_String(lampState ? "LAMP: ON " : "LAMP: OFF");
            } 
            else {
                LCD_SetCursor(1, 1);
                LCD_String("ADC: ");
                LCD_PrintNum(light);
                
                LCD_SetCursor(2, 1);
                if (light > BRIGHT_THRESHOLD)        LCD_String("BRIGHT LAMP:OFF");
                else if (light > MEDIUM_THRESHOLD)   LCD_String("MEDIUM LAMP:OFF");
                else if (light > LOW_THRESHOLD)      LCD_String("LOW    LAMP:ON ");
                else                                 LCD_String("V.LOW  LAMP:ON ");
            }
            
            prevLight = light;
            prevMode = manualMode;
        }

        _delay_ms(50); 
    }
    return 0;
}