# Smart Zebra Crossing System with Emergency Traffic Control

## Overview

The Smart Zebra Crossing System with Emergency Traffic Control is an
intelligent traffic management prototype designed to improve
pedestrian safety and traffic efficiency using computer vision and
embedded systems.

The system uses a webcam to capture the road environment and YOLOv8
to detect pedestrians and vehicles in real time. The detected objects
are analysed using a defined Region of Interest (ROI), and decision
logic determines whether pedestrian or vehicle traffic should be
prioritized.

The resulting decision is transmitted to a Raspberry Pi using socket
communication. The Raspberry Pi controls LEDs through GPIO pins to
simulate vehicle and pedestrian traffic signals.

## Aim

To design and implement an intelligent zebra crossing system that
improves pedestrian safety and traffic efficiency using real-time
object detection and automated traffic signal control.

## Objectives

- Detect pedestrians and vehicles in real time using YOLOv8.
- Identify pedestrians within the zebra crossing region.
- Count vehicles approaching the crossing area.
- Analyse traffic conditions using decision-making logic.
- Prioritize pedestrian or vehicle traffic based on detected
  conditions.
- Detect potential emergency vehicles and provide immediate traffic
  priority.
- Establish communication between the detection system and Raspberry
  Pi using socket programming.
- Control traffic signal indicators using Raspberry Pi GPIO pins.

## System Architecture

The system follows a pipeline consisting of:

**Camera → Object Detection → ROI Analysis → Decision Making →
Socket Communication → Raspberry Pi → Traffic Signal LEDs**

The camera continuously captures video of the crossing area. YOLOv8
processes the frames and detects objects such as persons, cars,
buses, trucks, and motorbikes.

The decision-making module analyses the detected objects and sends the
appropriate command to the Raspberry Pi.

## Working Principle

### 1. Video Capture

A USB webcam captures live video of the road and zebra crossing area.

### 2. Object Detection

YOLOv8 is used to detect relevant objects including:

- Person
- Car
- Bus
- Truck
- Motorbike

The model provides bounding boxes and confidence scores for detected
objects.

### 3. Region of Interest Analysis

A Region of Interest (ROI) is defined around the zebra crossing.

Only pedestrians detected within this region are considered for the
pedestrian crossing decision. This prevents pedestrians outside the
crossing area from affecting the traffic control logic.

### 4. Decision Logic

The system analyses the number of pedestrians and approaching
vehicles.

- If an emergency vehicle is detected, vehicle traffic is given
  immediate priority.
- If pedestrians outnumber vehicles, pedestrian crossing is
  prioritized.
- Otherwise, vehicle traffic continues.

### 5. Communication

The detection system communicates with the Raspberry Pi using Python
socket programming.

The system sends messages representing the detected condition:

- `PERSON`
- `VEHICLE`
- `EMERGENCY`

### 6. Hardware Control

The Raspberry Pi receives the messages and controls the traffic
signal LEDs through its GPIO pins.

For pedestrian priority:

- Pedestrian green → ON
- Vehicle red → ON

For vehicle priority:

- Vehicle green → ON
- Pedestrian red → ON

For emergency mode:

- Vehicle green → ON

## Hardware Components

- Raspberry Pi
- USB Webcam
- LEDs
- Resistors
- Breadboard
- Jumper Wires

The prototype uses LEDs to represent vehicle and pedestrian traffic
signals.

## GPIO Configuration

The project uses the following Raspberry Pi GPIO pins:

| GPIO Pin | Function |
|---|---|
| GPIO17 | Traffic signal |
| GPIO27 | Traffic signal |
| GPIO22 | Traffic signal |
| GPIO23 | Traffic signal |

The LEDs are connected through current-limiting resistors, with their
negative terminals connected to common ground.

## Software and Technologies

- Python
- YOLOv8
- Ultralytics
- OpenCV
- Socket Programming
- RPi.GPIO
- Raspberry Pi
- Computer Vision

## Implementation

The software implementation is divided into two modules.

### Detection Module

Runs on the laptop and performs:

- Video capture
- YOLOv8 object detection
- ROI analysis
- Traffic condition analysis
- Decision making
- Socket communication

### Raspberry Pi Control Module

Runs on the Raspberry Pi and performs:

- Receiving commands
- Interpreting signal states
- GPIO control
- LED traffic signal operation

This modular architecture allows the detection and hardware-control
modules to be developed or upgraded independently.

## Results

The system successfully detects pedestrians and vehicles in real time
using YOLOv8.

The Raspberry Pi receives the generated commands and controls the LED
signals according to the detected traffic conditions.

The emergency priority feature allows vehicle traffic to be given
immediate priority when an emergency vehicle is detected.

The prototype demonstrates communication between a computer-vision
system and an embedded hardware controller for intelligent traffic
management.

## Advantages

- Improves pedestrian safety.
- Uses real-time traffic conditions.
- Reduces dependence on fixed signal timing.
- Provides emergency vehicle priority.
- Uses affordable hardware.
- Uses open-source software tools.
- Provides a modular and scalable architecture.
- Can be extended for smart-city applications.

## Limitations

- Detection accuracy may decrease under poor lighting conditions.
- Rain and fog can affect detection performance.
- Emergency vehicle detection currently relies on general vehicle
  classification.
- The prototype uses LEDs instead of real traffic signals.
- The system is primarily intended as a demonstration prototype.

## Future Scope

Possible improvements include:

- High-resolution cameras for improved detection.
- Deployment of optimized AI models directly on edge devices.
- Emergency vehicle detection using siren recognition.
- Wireless communication for city-wide traffic coordination.
- Integration with centralized traffic management systems.
- Additional sensors such as LiDAR, radar, or infrared sensors.
- Improved detection reliability under varying environmental
  conditions.

## Applications

The system can be adapted for:

- Smart city infrastructure
- Intelligent pedestrian crossings
- Traffic management
- Emergency vehicle prioritization
- Automated traffic signal systems
- Computer-vision-based transportation systems

## Project Outcome

The project demonstrates the integration of artificial intelligence,
computer vision, network communication, and embedded systems to create
an intelligent traffic control prototype.

The combination of YOLOv8 object detection, decision-making logic,
socket communication, and Raspberry Pi GPIO control provides a
practical implementation of adaptive traffic management.

## Project Documentation

- [Figures](Figures/)
- [Hardware](Hardware/)
- [Software](Software/)
- [Working Videos](Working-Videos/)
- [Project Report](Report/)
