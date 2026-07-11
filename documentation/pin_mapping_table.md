# Pin Mapping Table
## ATmega32 Automatic Lighting Control System
**Project:** PBL 18 | Course: CT 321 Embedded Systems | **Institution:** University of Dodoma (UDOM) – CIVE

---

| ATmega32 Pin | Port | Physical Pin | Direction | Connected To | Component Ref | Value | Description |
|---|---|---|---|---|---|---|---|
| PA0 | PORTA | 40 | INPUT | LDR Voltage Divider | R5 (LDR03), R1 | 10kΩ | ADC Channel 0 – Light level reading |
| PA1 | PORTA | 39 | INPUT | Push Button | SW1 | – | Manual override toggle (Active LOW, internal pull-up) |
| PB0 | PORTB | 1 | OUTPUT | Red LED | D4, R7 | 220Ω | Red LED – Low light / Error indicator |
| PB1 | PORTB | 2 | OUTPUT | Yellow LED | D6, R6 | 220Ω | Yellow LED – Medium light indicator |
| PB2 | PORTB | 3 | OUTPUT | Green LED | D5, R4 | 220Ω | Bright light indicator |
| PC0 | PORTC | 22 | OUTPUT | Relay Driver | Q1 (BC547), R3 | 1kΩ | NPN transistor drives relay coil K1 (lamp control) |
| PC1 | PORTC | 23 | OUTPUT | Buzzer Driver | Q2 (BC547), R8 | 1kΩ | NPN transistor drives buzzer BZ1 (alert output) |
| PD0 | PORTD | 14 | OUTPUT | LCD RS (Pin 4) | DS1 (WC1602A) | – | Register Select (0=Command, 1=Data) |
| PD1 | PORTD | 15 | OUTPUT | LCD EN (Pin 6) | DS1 (WC1602A) | – | Enable strobe signal |
| PD4 | PORTD | 18 | OUTPUT | LCD D4 (Pin 11) | DS1 (WC1602A) | – | LCD data bit 4 (4-bit mode) |
| PD5 | PORTD | 19 | OUTPUT | LCD D5 (Pin 12) | DS1 (WC1602A) | – | LCD data bit 5 (4-bit mode) |
| PD6 | PORTD | 20 | OUTPUT | LCD D6 (Pin 13) | DS1 (WC1602A) | – | LCD data bit 6 (4-bit mode) |
| PD7 | PORTD | 21 | OUTPUT | LCD D7 (Pin 14) | DS1 (WC1602A) | – | LCD data bit 7 (4-bit mode) |
| VCC | – | 10 | POWER | +5V Supply | Battery/Regulator | 5V | Main digital supply |
| GND | – | 11 | POWER | Ground | – | – | Digital ground |
| AVCC | – | 30 | POWER | +5V Supply | Battery/Regulator | 5V | ADC analog supply |
| AGND | – | 31 | POWER | Ground | – | – | ADC analog ground |
| RESET | – | 9 | INPUT | Reset Circuit | R11, SW2, C6 | 10kΩ, 1µF | Active LOW reset with pull-up and debounce |
| XTAL1 | – | 13 | INPUT | Crystal | Y1, C1 | 16MHz, 22pF | External clock input |
| XTAL2 | – | 12 | OUTPUT | Crystal | Y1, C2 | 16MHz, 22pF | External clock output |

---

> **LCD Notes:** R/W pin (LCD Pin 5) tied permanently to GND (write-only). Vo/contrast pin (LCD Pin 3) tied to GND for maximum contrast. Backlight LED+ connected via R10 (220Ω) to VCC.  
> **DDR Config:** `DDRA=0x00` | `DDRB=0x07` | `DDRC=0x03` | `DDRD=0xF3`

---

