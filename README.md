Smart Industrial Safety Monitoring System using LPC2129

1. Project Overview

The Smart Industrial Safety Monitoring System is an embedded systems project designed to monitor industrial working environments and identify potentially unsafe conditions.

The system uses the LPC2129 ARM7 microcontroller to continuously monitor temperature, worker movement, and flame detection. Based on the sensor readings, it determines the machine's safety status and provides real-time indications through LEDs, a 16×2 LCD display, and a UART terminal.

The primary objective of this project is to demonstrate how embedded systems can be used for industrial safety monitoring through real-time sensor interfacing, data processing, and alert generation.

Key Features

- Real-time temperature monitoring using the LM35 temperature sensor.
- Worker movement detection using a PIR sensor.
- Fire detection using a flame sensor.
- Machine safety status classification: SAFE, WARNING, and DANGER.
- Real-time temperature and status display on a 16×2 LCD.
- UART-based serial monitoring through a PC or laptop.
- LED-based visual indication of machine safety conditions.
- Modular driver-based Embedded C implementation.

---

2. System Architecture

The system is built around the LPC2129 ARM7 microcontroller, which receives inputs from the sensors, processes the readings, and generates appropriate outputs.

Block Diagram


             +----------------------+
             |    5V Power Supply   |
             +----------------------+
                       |
                       v
+----------------+  +--------------------+
| LM35 Sensor    |  |                    |
| Temperature    |->|                    |
| P0.28 / AD0.1  |  |                    |
+----------------+  |                    |
                    |                    |
+----------------+  |                    |
| PIR Sensor     |->|                    |
| P0.30          |  |      LPC2129       |
+----------------+  |      ARM7 MCU      |
                    |                    |
+----------------+  |                    |
| Flame Sensor   |->|                    |
| P0.5           |  |                    |
+----------------+  +--------------------+
                             |
              +--------------+--------------+
              |              |              |
              v              v              v
        +-----------+  +-----------+  +-----------+
        | Green LED |  | Yellow LED|  |  Red LED  |
        | SAFE      |  | WARNING   |  | DANGER    |
        +-----------+  +-----------+  +-----------+
                             |
              +--------------+--------------+
              |                             |
              v                             v
        +-------------+               +-------------+
        | 16x2 LCD    |               | UART        |
        | Temperature |               | PC/Laptop   |
        | Machine     |               | Monitoring  |
        | Status      |               +-------------+
        +-------------+


 System Components

| Component | Purpose |
|---|---|
| LPC2129 | Main controller for sensor processing and safety logic |
| LM35 | Measures ambient temperature |
| PIR Sensor | Detects worker movement |
| Flame Sensor | Detects the presence of flame |
| Green LED | Indicates SAFE condition |
| Yellow LED | Indicates WARNING condition |
| Red LED | Indicates DANGER condition |
| 16×2 LCD | Displays temperature and machine status |
| UART | Sends sensor information to a PC terminal |
| 5V Power Supply | Supplies power to the system components |



 3. Working Principle

The system operates through continuous sensor monitoring and safety condition evaluation.

1. Initialization

When the system is powered ON:

- GPIO pins are configured.
- ADC is initialized for temperature measurement.
- UART0 is initialized for serial communication.
- LCD is initialized in 4-bit mode.
- LEDs are initialized to their OFF state.

The LCD displays the system startup message.

2. Temperature Measurement

The LM35 temperature sensor generates an analog voltage proportional to the surrounding temperature.

The LPC2129 ADC converts this analog voltage into a 10-bit digital value.

The temperature is calculated using:


T = \frac{ADC \times 3300}{1023}


Here, T represents temperature in tenths of a degree Celsius, assuming a 3.3V ADC reference and an LM35 output of 10mV/°C.

For example:

```text
ADC Value       : 113
Calculated Temp : 36.4°C approximately
```

 3. Worker Movement Detection

The PIR sensor detects movement in its sensing area.

- HIGH output indicates movement detected.
- LOW output indicates no movement.

The movement status is displayed on the UART terminal.

4. Flame Detection

The flame sensor monitors the surrounding environment for fire.

The system assumes an active-LOW flame sensor output:

- LOW indicates flame detected.
- HIGH indicates no flame detected.

When a flame is detected, the machine status is classified as DANGER.

 5. Safety Condition Evaluation

The controller evaluates the sensor readings and determines the machine status.

| Condition | Machine Status | LED Indication |
|---|---|---|
| Temperature below 40°C and no flame | SAFE | Green LED |
| Temperature between 40°C and 49.9°C | WARNING | Yellow LED |
| Temperature 50°C or above | DANGER | Red LED |
| Flame detected | DANGER | Red LED |

6. LED Indication

Based on the machine status, the LPC2129 activates the corresponding LED.

- Green LED indicates normal operating conditions.
- Yellow LED indicates a temperature warning.
- Red LED indicates a dangerous condition.

7. LCD and UART Monitoring

The LCD continuously displays the temperature and machine status.

The UART terminal provides additional information, including:

- Temperature reading
- PIR sensor status
- Flame sensor status
- Machine safety status
- LED status

This enables real-time monitoring and debugging through a PC or laptop.


4. Repository Structure

The project follows a modular driver-based architecture.

Smart-Industrial-Safety-Monitoring-System/
│
├── README.md
│
├── main.c
│   ├── GPIO Initialization
│   ├── Sensor Reading
│   ├── Safety Checking
│   ├── LED Control
│   ├── LCD Display
│   └── UART Display
│
├── header.h
│   └── Common Header File
│
├── adc_driver.c
│   └── ADC Initialization and Reading
│
├── delay_driver.c
│   └── Timer0 Delay Functions
│
├── uart_driver.c
│   └── UART0 Communication Functions
│
├── lcd_driver.c
│   └── 16x2 LCD 4-bit Driver
│
└── project/
    └── LPC2129 Keil Project Files


5. Sample Output

LCD Display

**Normal Condition**

```text
+----------------+
| TEMP: 36.5 C   |
| STATUS: SAFE   |
+----------------+
```

**Warning Condition**

```text
+----------------+
| TEMP: 42.0 C   |
| STATUS: WARNING|
+----------------+
```

**Danger Condition**

```text
+----------------+
| TEMP: 55.0 C   |
| STATUS: DANGER |
+----------------+
```

UART Terminal Output

Normal Condition

--------------------------
SMART INDUSTRIAL SAFETY
--------------------------
Temperature: 36.5 C
PIR Sensor: Worker Detected
Flame Sensor: NO FIRE
Machine Status: SAFE
LED Status: GREEN LED ON
--------------------------

Warning Condition


--------------------------
SMART INDUSTRIAL SAFETY
--------------------------
Temperature: 42.0 C
PIR Sensor: Worker Detected
Flame Sensor: NO FIRE
Machine Status: WARNING
LED Status: YELLOW LED ON
--------------------------


Danger Condition

--------------------------
SMART INDUSTRIAL SAFETY
--------------------------
Temperature: 55.0 C
PIR Sensor: Worker Detected
Flame Sensor: FIRE DETECTED
Machine Status: DANGER
LED Status: RED LED ON
--------------------------

7. Future Enhancements

The project can be extended with additional features to improve monitoring and safety functionality.

1. GSM-Based Alert System

Integrate a GSM module to send SMS alerts when fire or dangerous temperature conditions are detected.

2. IoT-Based Monitoring

Add Wi-Fi connectivity to enable remote monitoring of temperature, worker movement, and machine safety status through a web dashboard.

3. Buzzer-Based Alarm

Integrate a buzzer to provide an audible alert during dangerous conditions.


8. Conclusion

The Smart Industrial Safety Monitoring System demonstrates the implementation of an embedded safety monitoring application using the LPC2129 ARM7 microcontroller.

The project integrates multiple sensors, ADC conversion, UART communication, LCD interfacing, and GPIO control to monitor environmental conditions and identify potentially unsafe situations.

The modular driver-based architecture improves code readability, debugging, and maintainability.

This project provides practical experience in Embedded C programming, ARM7 microcontroller interfacing, peripheral configuration, and real-time monitoring.

The system can serve as a foundation for more advanced industrial safety applications incorporating IoT connectivity, remote alerts, data logging, and automated safety mechanisms.
