# MediaFly Express - Hardware Pinout Configuration

## Complete Pin Mapping for Arduino Mega 2560

### Temperature & Humidity Sensors (DHT11)
| Sensor | Pin | Type | Notes |
|--------|-----|------|-------|
| DHT Sensor 1 | 52 | Digital Input | Primary temperature/humidity |
| DHT Sensor 2 | 48 | Digital Input | Redundant for sensor fusion |

**Libraries Required:** Adafruit DHT Sensor Library
**Baud Rate:** I2C/Digital Interface
**Averaging:** Both sensors averaged for accuracy

---

### Ultrasonic Sensors (HC-SR04) - Obstacle Detection
| Function | Trigger Pin | Echo Pin | Range |
|----------|-------------|----------|-------|
| Forward Detection | 2 | 4 | ~400 cm |
| Side Detection | 5 | 6 | ~400 cm |

**Operation:**
- Send 5µs pulse to trigger pin
- Measure echo time on echo pin
- Distance = (duration × 0.034) / 2 cm
- Obstacle Threshold: < 300 cm

**Configuration:**
```cpp
#define trigpin1 2   // Forward sensor
#define echopin1 4
#define trigpin2 5   // Side sensor  
#define echopin2 6
```

---

### Motor Control (Adafruit Motor Shield)
| Motor | Shield Channel | Control Frequency | Speed Control |
|-------|---------------|--------------------|--------------|
| Motor 1 | 1 | 1 KHz | PWM (0-255) |
| Motor 2 | 2 | 1 KHz | PWM (0-255) |
| Motor 3 | 3 | 34 KHz | PWM (0-255) |
| Motor 4 | 4 | 34 KHz | PWM (0-255) |

**Commands:**
- `FORWARD` - Clockwise rotation
- `BACKWARD` - Counter-clockwise rotation
- `RELEASE` - Motor off

**Address:** 0x60 (default I2C)

---

### Bluetooth Module (HC-05)
| Function | Pin | Type | Baud Rate |
|----------|-----|------|-----------|
| RX (Receive) | 10 | Digital | 9600 |
| TX (Transmit) | 11 | Digital | 9600 |
| VCC | 5V | Power | - |
| GND | GND | Ground | - |

**SoftwareSerial Declaration:**
```cpp
SoftwareSerial B(10, 11);  // RX, TX
```

**Data Format Transmitted:**
```
<temperature>,<cooling_status>,<humidity>,<weather>,<obstacle_status>;
```

Example: `24,Cooling,65,Sunny,Clear;`

---

### Rain Sensor
| Function | Pin | Type | Threshold |
|----------|-----|------|-----------|
| Analog Input | A0 | Analog Input | < 350 = Rain |

**Calibration:**
- Dry: ~400+ analog units
- Wet: ~0-350 analog units

---

### Thermal Control Circuit
| Function | Pin | Type | Trigger |
|----------|-----|------|---------|
| Cooler On/Off | 50 | Digital Output | HIGH = ON |
| Test Probe | 44 | Digital Input | HIGH = Extended Range |
| Complement Signal | 46 | Digital Output | Complement to Pin 50 |

**Thermal Control Logic:**
```
If Test Probe HIGH: Max Temperature = 32°C
If Test Probe LOW:  Max Temperature = 20°C

If Current Temp >= Max Temp:
    Set Pin 50 = HIGH (Cooler ON)
    Set Pin 46 = LOW
Else:
    Set Pin 50 = LOW (Cooler OFF)
    Set Pin 46 = HIGH
```

---

## Complete Wiring Summary

### Power Distribution
- **5V Rail:** DHT sensors, Ultrasonic sensors (Vcc), Bluetooth module
- **GND Rail:** All sensors and modules (common ground)
- **External Power:** Motor power supply (separate from Arduino)

### Serial Communication
- **Serial (Hardware UART):** Debugging & Motor Shield communication
- **SoftwareSerial (10, 11):** Bluetooth module

### Safety Considerations
- **Current Draw:** Ensure power supply can handle 4 motors + sensors (~2-3A peak)
- **Voltage Regulation:** Use separate power for motors to avoid brownout
- **Signal Protection:** Add pull-up resistors to DHT data lines if lines > 1 meter

---

## Pin Usage Summary

| Pin Range | Usage | Count |
|-----------|-------|-------|
| 2-6 | Ultrasonic sensors | 4 |
| 10-11 | Bluetooth (Software Serial) | 2 |
| 44, 46, 50 | Thermal control | 3 |
| 48, 52 | DHT sensors | 2 |
| A0 | Rain sensor | 1 |
| I2C (SDA/SCL) | Motor Shield | 2 |

**Total Digital Pins Used:** 11 of 54 (Arduino Mega)
**Total Analog Pins Used:** 1 of 16

---

## Hardware Testing Checklist

- [ ] All sensors read valid data via Serial Monitor
- [ ] Bluetooth module responds to AT commands
- [ ] Motor Shield communicates via I2C
- [ ] Each motor spins when commanded
- [ ] Temperature sensors average properly
- [ ] Ultrasonic sensors report distance < 400cm
- [ ] Rain sensor triggers correctly when wet
- [ ] Cooler circuit activates at temperature threshold
- [ ] Emergency landing triggered by rain detection
- [ ] Obstacle detection works at 300cm threshold

---

## Troubleshooting by Pin

### No Motor Response
1. Check Motor Shield power supply
2. Verify I2C communication (SDA/SCL)
3. Test each motor individually
4. Check motor polarity

### No Sensor Data
1. Verify 5V power to all sensors
2. Check data line connections
3. Confirm proper pull-up resistors
4. Test with known-good sensor

### Bluetooth Not Connecting
1. Verify HC-05 is powered (red LED should light)
2. Check RX/TX pin connections (reversed?)
3. Verify baud rate is 9600
4. Test with mobile Bluetooth terminal app

### Temperature Always High/Low
1. Check DHT11 data pins (often 48, 52)
2. Verify sensor is not damaged (age limit ~5 years)
3. Add pull-up resistor (10kΩ) to data line
4. Check temperature calculations in code

---

## Advanced Configuration

### Adjusting Motor Speed
Edit values in movement functions (currently 255 = max):
```cpp
motor1.setSpeed(255);  // Change to 128 for 50% speed, etc.
```

### Calibrating Temperature Threshold
Edit `setMaxTemp()` function:
```cpp
if (test==HIGH) return 320;  // ~32°C, adjust as needed
if (test==LOW)  return 20;   // ~20°C, adjust as needed
```

### Changing Sensor Thresholds
- **Rain Detection:** Modify rain sensor value check in `rainDrop()`
- **Obstacle Distance:** Modify 300cm value in `obstacleDetection()`
- **Sensor Reading Interval:** Adjust `delay()` values throughout

---

**Last Updated:** 2024
**Compatible Hardware:** Arduino Mega 2560 + Adafruit Motor Shield v2
