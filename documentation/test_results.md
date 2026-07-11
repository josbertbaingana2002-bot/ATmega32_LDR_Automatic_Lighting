# Test Results
## Project: LDR-Based Automatic Lighting Control System

## Test Environment
- Simulation Tool: SimulIDE
- Microcontroller: ATmega32 @ 16MHz
- Light Sensor: Potentiometer (simulating LDR)
- Date: July 2026

## Test Cases

### Test 1 — System Startup
| Item | Expected | Result |
|---|---|---|
| LCD Line 1 | "SMART LIGHT" | ✅ PASS |
| LCD Line 2 | "SYSTEM READY" | ✅ PASS |
| Duration | 2 seconds | ✅ PASS |
| Transition | Enters automatic mode | ✅ PASS |

### Test 2 — Bright Light Condition (ADC > 800)
| Item | Expected | Result |
|---|---|---|
| Green LED | ON | ✅ PASS |
| Yellow LED | OFF | ✅ PASS |
| Red LED | OFF | ✅ PASS |
| Lamp (Relay) | OFF | ✅ PASS |
| Buzzer | OFF | ✅ PASS |
| LCD Line 1 | ADC: [value] | ✅ PASS |
| LCD Line 2 | "BRIGHT LAMP:OFF" | ✅ PASS |

### Test 3 — Medium Light Condition (ADC 601–800)
| Item | Expected | Result |
|---|---|---|
| Green LED | OFF | ✅ PASS |
| Yellow LED | ON | ✅ PASS |
| Red LED | OFF | ✅ PASS |
| Lamp (Relay) | OFF | ✅ PASS |
| Buzzer | OFF | ✅ PASS |
| LCD Line 2 | "MEDIUM LAMP:OFF" | ✅ PASS |

### Test 4 — Low Light Condition (ADC 401–600)
| Item | Expected | Result |
|---|---|---|
| Green LED | OFF | ✅ PASS |
| Yellow LED | OFF | ✅ PASS |
| Red LED | ON | ✅ PASS |
| Lamp (Relay) | ON | ✅ PASS |
| Buzzer | OFF | ✅ PASS |
| LCD Line 2 | "LOW LAMP:ON" | ✅ PASS |

### Test 5 — Very Low Light Condition (ADC < 400)
| Item | Expected | Result |
|---|---|---|
| Red LED | ON | ✅ PASS |
| Lamp (Relay) | ON | ✅ PASS |
| Buzzer | Short beep (200ms) | ✅ PASS |
| LCD Line 2 | "V.LOW LAMP:ON" | ✅ PASS |

### Test 6 — Manual Override
| Item | Expected | Result |
|---|---|---|
| 1st button press | Enters MANUAL MODE | ✅ PASS |
| LCD Line 1 | "MANUAL MODE" | ✅ PASS |
| 2nd button press | Lamp toggles ON/OFF | ✅ PASS |
| LCD Line 2 | "LAMP: ON / LAMP: OFF" | ✅ PASS |

### Test 7 — Sensor Error
| Item | Expected | Result |
|---|---|---|
| LDR disconnected | Error detected | ✅ PASS |
| Red LED | ON | ✅ PASS |
| Buzzer | ON | ✅ PASS |
| LCD Line 1 | "SENSOR ERROR!" | ✅ PASS |
| LCD Line 2 | "Check LDR" | ✅ PASS |

### Test 8 — Reset Operation
| Item | Expected | Result |
|---|---|---|
| Reset button pressed | System restarts | ✅ PASS |
| Startup message shown | "SMART LIGHT" | ✅ PASS |
| Returns to auto mode | manualMode = 0 | ✅ PASS |

## Summary
| Total Tests | Passed | Failed |
|---|---|---|
| 8 | 8 | 0 |

## Conclusion
The LDR-Based Automatic Lighting Control System successfully demonstrates
all required functions including ADC-based light sensing, automatic lamp
control, manual override, sensor error detection, and system reset.
