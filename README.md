# smart-battery-monitoring-system

ESP32 Smart Battery Monitoring System (IoT Project)
## Overview

This project is an advanced IoT-based Smart Battery Monitoring System using ESP32. It reads battery voltage, calculates percentage, detects charging status, and sends real-time updates to a Telegram bot. It also supports remote commands like /status for live battery information.

## Features

Real-time battery voltage monitoring
Battery percentage calculation
Status detection (Full, Medium, Low)
Charging detection (Start/Stop alerts)
Low battery warning system
Telegram bot integration
Remote command support (/status)
WiFi-based IoT system

## Technologies Used

ESP32 (Arduino IDE)
WiFi + HTTPClient
ArduinoJson library
Telegram Bot API
Analog voltage sensing
WiFiClientSecure (HTTPS communication)

## Project Structure

esp32-smart-battery-telegram/
│
├── main.ino
└── README.md

## How to Run

install Arduino IDE
Install ESP32 board support
Install ArduinoJson library
Connect voltage sensor to GPIO 34
Update WiFi credentials
Add Telegram bot token and chat ID
Upload code to ESP32

## How It Works

ESP32 connects to WiFi network

Battery voltage is read from analog pin (GPIO 34)

Voltage is converted into real battery value using calibration

System calculates:
Battery percentage
Battery status (Full, Medium, Low)

Telegram Integration:
Sends real-time battery updates
Responds to /status command from Telegram

Smart Features:
Low battery alert when voltage drops below threshold
Charging detection when voltage increases or decreases
Prevents spam using state tracking

Telegram Commands:
/status → Shows current voltage, percentage, and status

## Future Improvements

Add cloud database logging
Add mobile app dashboard
Improve voltage calibration accuracy
Add solar charging analytics
Support multiple batteries
Add MQTT support for IoT scalability

## Author
Harsha G
Learning Python | Embedded Systems | IoT
