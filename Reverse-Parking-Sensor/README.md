# Reverse Parking Sensor

## Overview

The Reverse Parking Sensor is an electronic proximity-warning system designed to assist a vehicle during reverse parking. The system uses an infrared (IR) sensing pair to detect nearby obstacles and provides visual and audible feedback through three LEDs and a buzzer.

The prototype was developed using LM358 operational amplifier ICs, adjustable potentiometers, resistors, LEDs, and an IR sensing pair. The circuit was demonstrated using Proteus 8 and implemented as a physical breadboard prototype integrated with a small vehicle model.

## Objectives

- To develop a proximity-based warning system for reverse parking.
- To detect obstacles using an infrared sensing pair.
- To process the sensing signal using LM358-based circuitry.
- To provide visual feedback using multiple LED indicators.
- To provide an audible warning using a buzzer.
- To develop and demonstrate a functional parking-assistance prototype.
- To gain practical experience in analog circuit design, sensing, and hardware prototyping.

## System Overview

The system consists of an infrared sensing section, signal-processing circuitry, and visual and audible indication components.

The IR pair is used for obstacle detection. The detected signal is processed using LM358-based circuitry and adjustable threshold settings. Based on the detected proximity condition, the system provides feedback through three LEDs and a buzzer.

### Main Functional Blocks

```text
Obstacle
    ↓
IR Sensing Pair
    ↓
LM358 Signal Processing
    ↓
Threshold / Comparison Circuit
    ↓
Parking Status Indication
    ├── LED 1
    ├── LED 2
    ├── LED 3
    └── Buzzer
```

## Working Principle

The reverse parking sensor operates by detecting the presence and proximity of an obstacle behind the vehicle.

1. The IR sensing pair detects an object in the sensing region.
2. The resulting sensing signal is processed using the LM358-based circuit.
3. Adjustable potentiometers are used to set the required threshold conditions.
4. The processed signal determines the corresponding parking warning condition.
5. Three LEDs provide visual feedback.
6. The buzzer provides an audible warning to alert the user.

The combination of visual and audible feedback helps the user identify the proximity condition while reversing.

## Hardware Components

| Component | Quantity |
|---|---:|
| LM358 IC | 2 |
| 10 kΩ Resistor | 1 |
| 1 kΩ Resistor | 3 |
| 10 kΩ Potentiometer | 3 |
| 150 Ω Resistor | 1 |
| Breadboard | 1 |
| 9 V Battery | 1 |
| Battery Connector | 1 |
| LED | 3 |
| Buzzer | 1 |
| IR Pair | 1 |

## Circuit Design

The electronic circuit is implemented on a breadboard using LM358 operational amplifier ICs, resistors, potentiometers, an IR sensing pair, LEDs, and a buzzer.

The LM358-based circuitry is used for signal processing and comparison, while the potentiometers provide adjustable threshold settings. The LEDs and buzzer act as the primary output indicators.

The prototype is powered using a 9 V battery.

## Visual and Audible Indication

The system uses three LED indicators to provide visual status information and a buzzer to provide an audible warning.

The multiple LED indicators allow different parking conditions to be represented, while the buzzer provides an additional warning to improve user awareness during reverse parking.

## Software and Simulation

### Proteus 8

Proteus 8 was used for circuit simulation and demonstration of the electronic parking sensor system.

The simulation environment allows the circuit operation to be examined before or alongside physical hardware implementation.

## Prototype Development

The physical prototype was assembled on a breadboard and integrated with a small vehicle model.

The development process involved:

1. Designing the parking sensor circuit.
2. Setting up the IR sensing section.
3. Integrating LM358 ICs into the signal-processing circuit.
4. Connecting resistors and potentiometers.
5. Integrating the three LED indicators.
6. Connecting the buzzer.
7. Providing power using a 9 V battery.
8. Integrating the circuit with the vehicle prototype.
9. Testing the warning indications.
10. Demonstrating the complete system.

## Applications

The concept can be applied to:

- Reverse parking assistance
- Vehicle obstacle detection
- Parking proximity warning systems
- Automotive safety prototypes
- Low-cost parking assistance systems
- Educational electronics and embedded systems projects

## Advantages

- Simple electronic implementation
- Low-cost components
- Visual and audible warning system
- Adjustable threshold settings
- Compact breadboard-based prototype
- Does not require a microcontroller for the basic implementation
- Suitable for demonstrating proximity-based parking assistance

## Limitations

- IR-based sensing can be affected by environmental and surface conditions.
- The prototype provides proximity indication rather than complete autonomous parking.
- The sensing range is limited by the characteristics of the IR sensing arrangement.
- The breadboard implementation is intended as a prototype and would require further development for automotive deployment.

## Learning Outcomes

This project provided practical experience in:

- Analog electronic circuit design
- IR-based sensing
- LM358 operational amplifier circuits
- Comparator and threshold-based circuits
- Breadboard prototyping
- LED and buzzer interfacing
- Circuit simulation using Proteus
- Hardware testing and troubleshooting
- Automotive sensing and safety concepts

## Project Outcome

A functional reverse parking sensor prototype was developed using an IR sensing pair, LM358-based signal-processing circuitry, adjustable threshold components, three LEDs, and a buzzer.

The system demonstrates how obstacle proximity can be converted into visual and audible feedback to assist a user during reverse parking.

## Project Documentation

- [Hardware](Hardware/)
- [Working Videos](Working-Videos/)
