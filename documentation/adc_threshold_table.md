# ADC Threshold Table
## Project: LDR-Based Automatic Lighting Control System

## ATmega32 ADC Specifications
- Resolution: 10-bit (0 to 1023)
- Reference Voltage: AVcc (5V)
- Input Channel: ADC0 (PA0)
- Prescaler: 64 (250 kHz ADC clock at 16MHz)

## Voltage Divider
- R1 (LDR): Variable resistance based on light intensity
- R2: 10kΩ fixed resistor
- Output voltage = VCC × R2 / (R_LDR + R2)

## ADC Threshold Values

| Light Condition | ADC Min | ADC Max | Voltage Range | Lamp | LED | Buzzer |
|---|---|---|---|---|---|---|
| Bright | 801 | 1023 | 3.92V – 5.0V | OFF | Green | OFF |
| Medium | 601 | 800 | 2.94V – 3.91V | OFF | Yellow | OFF |
| Low | 401 | 600 | 1.96V – 2.93V | ON | Red | OFF |
| Very Low | 0 | 400 | 0V – 1.96V | ON | Red | ON (beep) |
| Sensor Error | ≤5 or ≥1018 | - | Abnormal | OFF | Red | ON |

## Threshold Definitions in Firmware
| #define | Value | Description |
|---|---|---|
| BRIGHT_THRESHOLD | 800 | Above this = Bright condition |
| MEDIUM_THRESHOLD | 600 | Above this = Medium condition |
| LOW_THRESHOLD | 400 | Above this = Low condition |
| Below LOW_THRESHOLD | <400 | Very Low condition |

## Notes
- Higher ADC value = More light (brighter)
- Lower ADC value = Less light (darker)
- Thresholds can be adjusted by instructor as per project brief
- Error detection: ADC ≤ 5 or ≥ 1018 indicates disconnected or faulty LDR
