# ESP32 Motor Health Monitoring and Predictive Maintenance System

A real-time motor condition monitoring system built using an ESP32, FreeRTOS, temperature and vibration sensors, and MQTT-based communication. The system collects motor health parameters, classifies operating conditions, and publishes monitoring data for local or remote visualization.

## Overview

The system is designed to demonstrate how embedded systems and IoT technologies can be applied to motor condition monitoring and predictive maintenance.

The ESP32 performs sensor acquisition using multiple FreeRTOS tasks. Temperature and vibration data are processed locally, fault conditions are identified using defined thresholds, and the resulting data is transmitted through MQTT. Optional Blynk and Node-RED integrations provide remote and local monitoring interfaces.

## Objectives

- Monitor motor temperature in real time.
- Measure vibration using an MPU6050 inertial sensor.
- Use FreeRTOS for concurrent and periodic sensor-processing tasks.
- Detect abnormal operating conditions using threshold-based fault classification.
- Publish sensor data through MQTT.
- Provide monitoring through Node-RED and Blynk integrations.
- Demonstrate an embedded IoT architecture suitable for condition-monitoring applications.

## System Architecture

```text
             +----------------------+
             |      Motor/System    |
             +----------+-----------+
                        |
             +----------v-----------+
             |   Sensors            |
             |                      |
             | NTC Thermistor       |
             | MPU6050              |
             +----------+-----------+
                        |
                        v
             +----------------------+
             |      ESP32           |
             |                      |
             |    FreeRTOS          |
             |  Task Scheduling     |
             |                      |
             | Temperature Task     |
             | Vibration Task       |
             | Fault Detection      |
             | MQTT Task             |
             | Blynk Task            |
             +----------+-----------+
                        |
             +----------v-----------+
             | Communication        |
             |                      |
             | Wi-Fi / MQTT         |
             +----------+-----------+
                        |
             +----------+-----------+
             |                      |
       +-----v-----+          +-----v------+
       | Node-RED  |          |   Blynk    |
       | Dashboard |          | Dashboard  |
       +-----------+          +------------+
```

## Main Features

### Real-Time Temperature Monitoring

The NTC thermistor is connected to an ESP32 ADC input. The measured resistance is converted into temperature using the Steinhart-Hart relationship.

### Vibration Monitoring

The MPU6050 provides three-axis acceleration data through the I2C interface. The acceleration magnitude is used to identify abnormal vibration conditions.

### FreeRTOS-Based Task Scheduling

The firmware separates major functions into independent FreeRTOS tasks. This allows temperature acquisition, vibration sampling, fault detection, communication, and dashboard updates to operate at different intervals.

### Fault Classification

The current firmware uses threshold-based classification:

| Condition | Status |
|---|---:|
| Normal operation | 0 |
| High vibration | 1 |
| Overheat | 2 |

The temperature threshold is greater than 75°C and the vibration threshold is greater than 0.08 g, according to the current project configuration.

### MQTT Communication

The ESP32 publishes monitoring data using MQTT.

Current topics:

| Topic | Example Payload | Description |
|---|---|---|
| `motor/temp` | `32.45` | Temperature in °C |
| `motor/vibration` | `0.0342` | Vibration magnitude in g |
| `motor/status` | `0` / `1` / `2` | Current operating status |

### Monitoring Interfaces

The project includes configurations for:

- MQTT-based local monitoring
- Node-RED dashboard
- Blynk IoT dashboard
- Google Sheets logging
- MongoDB-based data storage

The availability of each interface depends on the corresponding firmware and configuration being used.

## Hardware Requirements

| Component | Specification | Interface |
|---|---|---|
| ESP32 | ESP32 WROOM-32 | — |
| Temperature sensor | 10 kΩ NTC thermistor | ADC |
| Vibration sensor | MPU6050 6-axis IMU | I2C |
| Resistor | 10 kΩ | Voltage divider |
| Power supply | 5 V USB | — |

## Pin Configuration

| ESP32 Pin | Connection |
|---|---|
| GPIO 34 | NTC thermistor ADC input |
| GPIO 21 | MPU6050 SDA |
| GPIO 22 | MPU6050 SCL |
| GND | Common ground |
| 5V | Power supply |

Serial communication is configured for 115200 baud.

## Software and Technologies

- Embedded C
- ESP32
- FreeRTOS
- Arduino IDE
- ESP-IDF components
- MQTT
- Blynk IoT
- Node-RED
- I2C
- ADC
- Wi-Fi

## FreeRTOS Task Structure

The firmware contains five main tasks in the extended configuration.

### 1. TemperatureTask

**Period:** 1000 ms

Responsibilities:

- Read the NTC thermistor through the ESP32 ADC.
- Convert the ADC measurement into temperature.
- Update the global temperature value.

### 2. MPUSensorTask

**Period:** 200 ms

Responsibilities:

- Read acceleration data from the MPU6050.
- Perform startup calibration.
- Calculate vibration magnitude.
- Update the vibration measurement.

### 3. FaultTask

**Period:** 1000 ms

Responsibilities:

- Evaluate temperature and vibration measurements.
- Apply the configured fault thresholds.
- Update the system status.

### 4. MQTTTask

**Period:** 2000 ms

Responsibilities:

- Maintain the MQTT connection.
- Publish temperature, vibration, and status data.
- Attempt reconnection when the broker connection is lost.

### 5. BlynkTask

**Period:** 1000 ms

Responsibilities:

- Update the Blynk dashboard.
- Provide mobile monitoring and notifications.

This task is used in the Blynk-enabled firmware configuration.

## Project Structure

```text
esp32-motor-health-monitoring/
│
├── docs/
│   └── images/
│       ├── blockdiagram.png
│       ├── circuit_schematic.png
│       ├── hardware_prototype.png
│       ├── node-red.png
│       ├── blynk_normal.png
│       └── blynk_fault.png
│
├── INSTALLATION.md
├── LICENSE
├── MPUtest.ino
├── PROJECT_REPORT.pdf
├── SAFECODE1.ino
├── final.ino
├── flows_nodered.json
└── README.md
```

## Temperature Measurement

The NTC thermistor is connected as a voltage divider. The measured ADC value is used to determine the thermistor resistance and subsequently calculate temperature.

The project uses the following parameters:

```text
R0   = 10 kΩ
Beta = 3950 K
T0   = 298.15 K
```

The temperature relationship used in the project is based on the thermistor model:

```text
R_NTC = R_fixed × (ADC / (ADC_MAX - ADC))

T_K = 1 / ((1/T0) + (1/Beta) × ln(R_NTC/R0))

T_C = T_K - 273.15
```

## Vibration Measurement

The MPU6050 provides three-axis acceleration data.

The total acceleration magnitude is calculated as:

```text
V_raw = sqrt(ax² + ay² + az²)

V_mag = V_raw - 1.0
```

The current firmware uses:

```text
V_mag <= 0.08 g  -> Normal
V_mag >  0.08 g  -> High vibration
```

## Performance Configuration

| Parameter | Configuration |
|---|---:|
| Temperature sampling | 1 Hz |
| Vibration sampling | 5 Hz |
| MQTT publishing | 0.5 Hz |
| Fault evaluation | 1 Hz |
| FreeRTOS tasks | 5 |
| MQTT QoS | 0 |

The original project documentation also reports a four-hour continuous test without a crash. This figure should be treated as a project test result rather than a general reliability guarantee.

## Installation

### Requirements

- Arduino IDE 2.0 or later
- ESP32 board package
- ESP32 development board
- NTC thermistor
- MPU6050
- 10 kΩ resistor
- Wi-Fi network
- MQTT broker such as Mosquitto

Optional components:

- Blynk IoT account
- Node-RED
- Google Sheets integration
- MongoDB

### Arduino IDE Setup

1. Install Arduino IDE.
2. Open Boards Manager.
3. Install the ESP32 board package by Espressif Systems.
4. Select `ESP32 Dev Module`.
5. Connect the ESP32 board.
6. Select the appropriate serial port.
7. Install the required libraries.
8. Open `final.ino`.
9. Configure Wi-Fi and MQTT settings.
10. Compile and upload the firmware.

## Wi-Fi and MQTT Configuration

Update the network parameters in the firmware:

```cpp
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
const char* mqtt_server = "Your_Broker_IP";
```

Do not commit real Wi-Fi passwords, API keys, authentication tokens, or other credentials to a public repository.

## MQTT Broker

For a local Mosquitto installation, the broker can be started using:

```bash
mosquitto -p 1883
```

To monitor the published topics:

```bash
mosquitto_sub -h <BROKER_IP> -t "motor/#" -v
```

Example output:

```text
motor/temp 32.45
motor/vibration 0.0342
motor/status 0
```

## Node-RED

The repository contains:

```text
flows_nodered.json
```

The flow can be imported into Node-RED and deployed after configuring the required MQTT connection.

## Sensor Calibration

### MPU6050

The project firmware performs an initial calibration using multiple samples. During calibration, keep the sensor stationary on a stable surface.

### NTC Thermistor

Temperature calculation depends on the thermistor parameters used by the firmware. The `Beta` value can be adjusted if a different thermistor is used.

## Troubleshooting

| Problem | Possible Solution |
|---|---|
| ESP32 does not connect to Wi-Fi | Check SSID, password, and network availability |
| MQTT connection fails | Verify broker IP, port, and broker status |
| Vibration reading remains zero | Check MPU6050 SDA/SCL connections |
| Temperature reading is incorrect | Check the thermistor voltage divider and resistor value |
| Node-RED receives no data | Verify MQTT topics and broker configuration |
| Blynk dashboard does not update | Check Blynk credentials and network connectivity |

## Possible Extensions

The system can be extended with additional sensing and analysis capabilities, including:

- Current sensing
- Motor speed measurement
- Additional temperature sensors
- Industrial-grade vibration sensors
- Data logging
- Remote dashboards
- Anomaly detection
- Machine-learning-based condition classification
- Secure MQTT communication
- Industrial communication protocols

## Learning Outcomes

This project demonstrates practical concepts in:

- ESP32 microcontroller programming
- Embedded C
- FreeRTOS task scheduling
- Sensor interfacing
- ADC-based measurement
- I2C communication
- Fault detection
- MQTT communication
- IoT monitoring
- Embedded system debugging
- Industrial condition monitoring

## License

This repository contains an MIT License file. Refer to `LICENSE` for the applicable license terms.

## Author

**Akul Polge**

GitHub: https://github.com/AkulX111

## Repository

https://github.com/AkulX111/esp32-motor-health-monitoring
