# Integrated-Smart-Home-Automation
ESP32-based smart home automation and security project with sensor monitoring, servo control, and ThingSpeak integration.



# Integrated Smart Home Automation & Security Simulation System

## Abstract

The Integrated Smart Home Automation & Security Simulation System is an IoT-based embedded project designed to simulate the core functions of a modern smart home. The system combines multiple sensors and actuators with real-time embedded logic to perform automated climate control, intelligent lighting, motion-based security, and smart door access management. It uses an ESP32 microcontroller as the central controller and sends sensor data to the ThingSpeak cloud platform for remote monitoring and analysis.

This project demonstrates how environmental sensing, decision-making, actuator control, and IoT communication can be integrated into a single embedded system.

---

## Project Overview

This system is built to replicate important smart home automation principles used in real-world residential environments. It continuously monitors environmental conditions such as temperature, humidity, light intensity, and motion, then performs automatic actions based on those readings.

The project includes the following major functions:

- Automatic climate control based on temperature
- Smart lighting based on ambient light intensity
- Intrusion detection using motion sensing
- Smart door control using a servo motor
- Dual operating modes: Normal Mode and Security Mode
- Cloud monitoring using ThingSpeak
- Real-time embedded control using non-blocking timing logic

---

## Objectives

- To design and implement a smart home automation prototype using ESP32
- To automate lighting and climate response using sensors
- To build a motion-based home security alert system
- To simulate an automated smart door access mechanism
- To upload live sensor data to the cloud using ThingSpeak
- To apply state-based embedded programming for real-time control

---

## Features

- Real-time sensor monitoring
- Intelligent light automation using LDR
- Temperature-based climate indication/control
- Motion-triggered buzzer alarm
- Servo-based smart door opening and closing
- Normal and Security mode switching
- Serial Monitor output for live debugging
- ThingSpeak cloud integration for IoT data logging
- Non-blocking control logic using `millis()`

---

## Core Functional Modules

### 1. Adaptive Climate Control Module
This module monitors temperature using the DHT22 sensor. When the sensed temperature exceeds the defined threshold, the system activates the climate indicator output, simulating a cooling or HVAC response.

### 2. Intelligent Ambient Illumination Module
This module uses an LDR sensor to detect ambient light intensity. When the surroundings become dark, the lighting output automatically turns on. When sufficient light is available, the lighting output turns off.

### 3. Intrusion Detection and Security Module
This module uses a PIR motion sensor to detect movement. If motion is detected, the system triggers an alarm through the buzzer. The buzzer response changes depending on whether the system is in Normal Mode or Security Mode.

### 4. Automated Access and Door Control Module
A push button is used to simulate a doorbell or access trigger. When pressed, the servo motor rotates to open the door and then automatically closes after a fixed time delay using non-blocking timing logic.

### 5. Global Mode Control Module
A separate button is used to toggle the system between two operating states:
- **Normal Mode**: Standard monitoring and normal alert behavior
- **Security Mode**: Higher alert sensitivity with faster alarm response

### 6. Cloud Monitoring and Integration Module
Sensor data such as temperature, humidity, light level, motion detection, and mode state are uploaded to ThingSpeak for remote monitoring and visualization.

---

## Hardware Components

- ESP32 Development Board
- DHT22 Temperature and Humidity Sensor
- LDR (Light Dependent Resistor)
- PIR Motion Sensor
- Servo Motor
- Buzzer
- LEDs
- Push Buttons
- Resistors
- Breadboard
- Jumper Wires

---

## Software and Tools Used

- Arduino IDE
- Embedded C/C++
- ESP32 Board Package
- DHT Sensor Library
- ESP32 Servo Library
- ThingSpeak Library
- Serial Monitor
- ThingSpeak Cloud Platform

---

## Working Principle

The system continuously reads values from the connected sensors and responds according to programmed logic.

### Smart Lighting Logic
- Read ambient light level from the LDR
- If light intensity is below the threshold, turn the light LED ON
- Otherwise, turn it OFF

### Climate Control Logic
- Read temperature from DHT22
- If temperature exceeds the threshold, activate climate output
- Otherwise, keep it OFF

### Motion Security Logic
- Read motion status from the PIR sensor
- If motion is detected:
  - In **Normal Mode**, trigger a standard alert
  - In **Security Mode**, trigger a faster buzzer alert pattern

### Door Access Logic
- Check if the door button is pressed
- Open the servo-controlled door
- Keep it open for a fixed time
- Automatically close the door using `millis()` timing

### Cloud Monitoring Logic
- Send sensor values and system status to ThingSpeak at regular intervals
- Display live output on the Serial Monitor

---

## Block Diagram / System Architecture

### Input Devices
- DHT22 Sensor
- LDR Sensor
- PIR Motion Sensor
- Door Button
- Mode Button

### Processing Unit
- ESP32 Microcontroller

### Output Devices
- Light LED
- Climate LED
- Buzzer
- Servo Motor

### Cloud Output
- ThingSpeak IoT Dashboard

---

## Pin Connections

| Component | ESP32 Pin |
|----------|-----------|
| LDR | 34 |
| PIR Sensor | 5 |
| Light LED | 2 |
| Climate LED | 18 |
| Buzzer | 4 |
| DHT22 Data Pin | 15 |
| Door Button | 27 |
| Servo Motor | 32 |
| Mode Button | 25 |

---

## Algorithm

1. Start the system
2. Initialize Wi-Fi, sensors, servo, LEDs, buzzer, and serial communication
3. Read sensor values from DHT22, LDR, and PIR
4. Check mode button status
5. Toggle between Normal and Security mode if needed
6. Control lighting based on LDR value
7. Control climate output based on temperature threshold
8. Trigger buzzer alert if motion is detected
9. Check door button and open/close servo door
10. Display system status on Serial Monitor
11. Upload data to ThingSpeak
12. Repeat continuously

---

## Project Structure

```text
Smart_Home_Automation.ino
README.md
Technical_Documentation.docx
Circuit_Diagram.png
Architecture_Flowchart.png
Evidence_Photos/
├── Serial_Monitor_Output.png
└── ThingSpeak_Output.png
