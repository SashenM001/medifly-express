# MediaFly Express 🚁

An intelligent autonomous quadcopter drone system with advanced environmental sensing, thermal control, and safety features. Designed for autonomous operation in variable environmental conditions with real-time telemetry monitoring.

## Overview

MediaFly Express is a sophisticated drone platform that combines hardware control, sensor integration, and wireless communication to create a responsive, intelligent flying system. The drone continuously monitors environmental conditions and can autonomously respond to threats like rain or obstacles.

## Features

### 🌡️ Environmental Monitoring
- **Dual Temperature Sensors**: Redundant DHT11 sensors for accurate temperature measurement
- **Humidity Monitoring**: Real-time humidity tracking from dual sensors
- **Rain Detection**: Automatic rain detection system for safety
- **Obstacle Avoidance**: Dual ultrasonic sensors for collision detection

### 🎮 Control & Communication
- **Bluetooth Control**: Wireless control interface via Bluetooth connectivity
- **4-Motor Control**: Independent control of all four quadcopter motors
- **Real-Time Telemetry**: Continuous data streaming of sensor readings

### 🛡️ Safety & Autonomy
- **Thermal Management**: Intelligent cooling circuit control based on temperature thresholds
- **Emergency Landing**: Automatic landing protocol triggered by rain detection
- **Obstacle Detection**: Prevents collisions with surroundings

### 📊 Smart Features
- **Dual Sensor Fusion**: Averages readings from multiple sensors for accuracy
- **Thermal Control System**: Automatic activation of cooling circuits when temperature exceeds threshold
- **State-Based Telemetry**: Transmits operational status including:
  - Current temperature and humidity levels
  - Cooling system status
  - Weather conditions
  - Obstacle detection status

## Hardware Requirements

### Microcontroller & Control
- **Arduino Mega 2560** (primary controller)
- **Adafruit Motor Shield** (motor control module)

### Sensors
- **2x DHT11** Temperature & Humidity Sensors (Pins: 52, 48)
- **2x HC-SR04** Ultrasonic Sensors for obstacle detection
  - Sensor 1: Trigger Pin 2, Echo Pin 4
  - Sensor 2: Trigger Pin 5, Echo Pin 6
- **Analog Rain Sensor** (Pin: A0)

### Actuators
- **4x DC Motors** (Controlled via Adafruit Motor Shield)
  - Motor 1: Channel 1 (1KHz)
  - Motor 2: Channel 2 (1KHz)
  - Motor 3: Channel 3 (34KHz)
  - Motor 4: Channel 4 (34KHz)

### Communication
- **HC-05 Bluetooth Module** (Pins: RX 10, TX 11 via SoftwareSerial)
- **UART Serial** (9600 baud rate)

### Power & Control Circuits
- **Cooler Circuit Control** (Pin: 50)
- **Temperature Probe** (Pin: 44)
- **Complement Signal** (Pin: 46)

## Project Structure

```
medifly-express/
├── README.md
├── _finalCode_Final/
│   └── _finalCode_Final.ino          # Main drone control & sensor logic
├── _Motor_Shield_COde_FInal/
│   └── _Motor_Shield_COde_FInal.ino  # Motor driver & movement control
└── Hardware/
    └── pinout_diagram.txt             # GPIO pin configuration reference
```

## Code Architecture

### Main Controller (_finalCode_Final.ino)
Handles all sensor reading and system logic:
- **Temperature Monitoring**: `measureTemp()`, `tecControl()`
- **Humidity Measurement**: `measureHumid()`
- **Rain Detection**: `rainDrop()`
- **Obstacle Detection**: `obstacleDetection()`, `UDSreading1()`, `UDSreading2()`
- **Thermal Control**: `tecControl()` - Auto-activates cooling when needed
- **Data Transmission**: `transferData()` - Streams sensor data via Bluetooth

### Motor Controller (_Motor_Shield_COde_FInal.ino)
Manages motor control and drone movement:
- **Movement Commands**: `forward()`, `back()`, `left()`, `right()`, `Stop()`
- **Serial Command Interface**: Receives single-character commands
  - `F` - Move Forward
  - `B` - Move Backward
  - `L` - Rotate Left
  - `R` - Rotate Right
  - `S` - Stop

## Installation & Setup

### 1. Hardware Assembly
- Mount Arduino Mega 2560 on drone frame
- Connect Adafruit Motor Shield to Arduino
- Connect 4 DC motors to Motor Shield
- Wire DHT11 sensors to pins 52 and 48
- Wire ultrasonic sensors to specified pins
- Connect rain sensor to analog pin A0
- Connect Bluetooth module (HC-05) to serial pins 10-11
- Wire cooler circuit control pins

### 2. Software Setup

**Requirements:**
- Arduino IDE (1.8.0 or later)
- DHT Library (by Adafruit)
- Adafruit Motor Shield V2 Library

**Installation Steps:**
```bash
1. Open Arduino IDE
2. Install libraries via Sketch → Include Library → Manage Libraries:
   - Search "DHT" and install "DHT sensor library by Adafruit"
   - Search "Adafruit Motor Shield" and install
3. Open _Motor_Shield_COde_FInal.ino and upload to Arduino
4. Open _finalCode_Final.ino and upload to Arduino
5. Connect Bluetooth module to mobile app or controller
```

### 3. Configuration
- **Temperature Threshold**: Set in `setMaxTemp()` function
- **Sensor Calibration**: Adjust thresholds in sensor reading functions
- **Motor Speed**: Modify values in `forward()`, `back()`, `left()`, `right()` functions (current: 255 = max speed)

## Usage

### Autonomous Mode
Simply power on the system. The drone will:
1. Monitor all environmental sensors continuously
2. Maintain temperature through cooling system
3. Detect rain and trigger emergency landing
4. Avoid obstacles through ultrasonic detection
5. Stream telemetry data via Bluetooth

### Manual Control via Bluetooth
Send commands through a Bluetooth terminal or mobile app:
- **Forward**: `F`
- **Backward**: `B`
- **Left**: `L`
- **Right**: `R`
- **Stop**: `S`

### Data Telemetry Format
Bluetooth transmission format:
```
<temperature>,<cooling_status>,<humidity>,<weather_status>,<obstacle_status>;
```

Example:
```
24,Cooling,65,Sunny,Clear;
```

## System Behavior

### Temperature Control Logic
```
IF current_temp >= max_temp:
    ACTIVATE cooling circuit
    DISPLAY "Cooler Circuit on"
ELSE:
    DEACTIVATE cooling circuit
    DISPLAY "Cooler Circuit off"
```

### Rain Emergency Response
```
IF rain_detected:
    TRIGGER emergency_landing()
    STOP all motors
```

### Obstacle Detection
Uses dual ultrasonic sensors with 300cm threshold. Confirms detection with dual reading to prevent false positives.

## Technical Specifications

| Parameter | Value |
|-----------|-------|
| Baud Rate | 9600 |
| Sensor Update Interval | 1000-1500 ms |
| Motor Control Frequency | 1 KHz (Motors 1,2), 34 KHz (Motors 3,4) |
| Temperature Sensors | DHT11 (Range: 0-50°C) |
| Humidity Range | 20-95% RH |
| Obstacle Detection Range | Up to 400cm |
| Rain Detection Threshold | < 350 analog units |

## Performance Characteristics

- **Sensor Accuracy**: Dual-sensor averaging for improved precision
- **Response Time**: ~1.5 seconds for full sensor cycle
- **Communication Latency**: Minimal via direct serial connection
- **Thermal Management**: Dynamic cooling based on real-time temperature
- **Safety**: Multiple layers of obstacle and weather detection

## Future Enhancements

- [ ] GPS integration for autonomous navigation
- [ ] Camera module for obstacle detection and navigation
- [ ] Mobile app with real-time data visualization
- [ ] Machine learning-based flight optimization
- [ ] Enhanced obstacle avoidance algorithms
- [ ] Multi-drone coordination system
- [ ] Telemetry data logging and analysis

## Troubleshooting

### Motor Not Responding
- Check Adafruit Motor Shield connections
- Verify motor power supply
- Confirm serial communication is working
- Check Bluetooth module is connected

### Sensor Reading Issues
- Verify DHT11 sensor wiring and data pins
- Check power supply stability
- Ensure proper library installation
- Test with Arduino Serial Monitor

### Bluetooth Connection Problems
- Verify HC-05 baud rate (should be 9600)
- Check TX/RX pin configuration
- Confirm Bluetooth module is powered
- Test with mobile Bluetooth terminal app

## Code Quality & Best Practices

- Modular function design for each subsystem
- Comprehensive sensor calibration
- Redundant safety mechanisms
- Clear variable naming and comments
- Efficient sensor fusion averaging
- Asynchronous communication handling

## Contributing

Contributions are welcome! Areas for improvement:
- Enhanced motor control algorithms
- Advanced sensor fusion techniques
- Improved obstacle avoidance
- Mobile app development
- Documentation expansion

## License

This project is provided as-is for educational and research purposes.

## Author

**Sashen M**

## Project Information

- **Type**: Autonomous Quadcopter Drone
- **Platform**: Arduino-based embedded system
- **Status**: Fully Functional
- **Last Updated**: 2024

---

**For questions, issues, or improvements, please open an issue or submit a pull request.**

*MediaFly Express - Intelligent Autonomous Flight System*
