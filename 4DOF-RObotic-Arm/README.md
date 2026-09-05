# 🤖 4-DOF Robotic Arm with ESP32 & Mobile App Control

## 📌 Overview

This project is a **4-DOF robotic arm** designed and developed from scratch during my internship at **Finstein Robotics**. The robotic arm uses an **ESP32 microcontroller** to control four servo motors responsible for the movement of the arm joints.

The mechanical structure and base were designed from scratch and fabricated using 3D printing. A dedicated mobile application was developed to provide wireless control of the robotic arm. A **QR-code-based access system** was also created to allow users to easily access the control application.

The project provided hands-on experience in **3D CAD design, 3D printing, servo motor control, ESP32 programming, mobile application development, and hardware-software integration**.

---

## 🎯 Objectives

- Design and develop a functional 4-DOF robotic arm from scratch.
- Design and fabricate the mechanical structure using 3D printing.
- Control multiple servo motors using an ESP32.
- Develop a mobile application for wireless robotic arm control.
- Integrate QR-code-based access for the mobile application.
- Understand the integration of mechanical, electronic, and software components in a robotic system.

---

## ⚙️ Features

- 4-DOF robotic arm
- Custom-designed mechanical base
- 3D-printed structural components
- Four servo motor actuation
- ESP32-based control system
- Wireless mobile application control
- QR-code-based application access
- Individual and coordinated joint movement
- Modular hardware and software architecture

---

## 🧰 Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 | 1 | Main microcontroller |
| Servo Motors | 4 | Robotic arm joint actuation |
| 3D-Printed Components | 1 set | Mechanical structure |
| Robotic Arm Base | 1 | Structural support |
| Power Supply | 1 | Powering the system |
| Connecting Wires | As required | Electrical connections |

---

## 💻 Software & Tools

- **Arduino IDE** – ESP32 programming
- **Embedded C/C++** – Motor control and firmware development
- **CAD Software** – Mechanical design of the robotic arm
- **3D Printing Software/Slicer** – Preparation of components for fabrication
- **Mobile Application** – Wireless control interface
- **QR Code Generator** – Quick access to the control application

---

## 🏗️ Mechanical Design

The robotic arm was designed from scratch with emphasis on:

- Stable base structure
- Proper servo motor mounting
- Joint alignment
- Suitable range of motion
- Lightweight yet rigid components
- Ease of assembly and maintenance

The designed components were converted into printable models and fabricated using a 3D printer. After printing, the components were assembled and the servo motors were mounted at their respective joints.

---

## 🔌 System Architecture

```text
                 ┌──────────────────────┐
                 │      Mobile App      │
                 │   Control Interface  │
                 └──────────┬───────────┘
                            │
                     Wireless Control
                            │
                            ▼
                 ┌──────────────────────┐
                 │        ESP32         │
                 │  Control & Processing│
                 └──────────┬───────────┘
                            │
                    Servo Control Signals
                            │
             ┌──────────────┼──────────────┐
             ▼              ▼              ▼
        ┌─────────┐    ┌─────────┐    ┌─────────┐
        │ Servo 1 │    │ Servo 2 │    │ Servo 3 │
        └─────────┘    └─────────┘    └─────────┘
                            │
                            ▼
                       ┌─────────┐
                       │ Servo 4 │
                       └─────────┘
                            │
                            ▼
                    4-DOF Robotic Arm
```

---

## 📱 Mobile Application

A dedicated mobile application was developed to control the robotic arm wirelessly.

The application provides a user-friendly interface through which movement commands can be sent to the ESP32. The commands are processed by the ESP32 and translated into servo motor movements.

A **QR code** was also created to provide convenient access to the application, allowing users to quickly scan and launch the required application.

---

## 🔄 Working Principle

1. The user opens the robotic arm control application.
2. The application sends movement commands wirelessly.
3. The ESP32 receives and processes the commands.
4. The corresponding servo motors are actuated.
5. Each servo controls a specific joint of the robotic arm.
6. Coordinated movement of the joints produces the required arm position.
7. The QR code provides a quick and convenient method of accessing the application.

---

## 🧪 Testing & Calibration

The robotic arm was tested progressively during development.

### Motor Testing

Each servo motor was tested individually to verify its movement and operating range.

### Joint Calibration

Servo positions were calibrated to ensure appropriate joint alignment and prevent unwanted mechanical interference.

### System Testing

The complete system was tested by combining:

- Mechanical assembly
- Servo actuation
- ESP32 control
- Wireless communication
- Mobile application

### Final Testing

The complete robotic arm was operated through the mobile application to verify coordinated movement and overall system functionality.

---

## 📂 Repository Structure

```text
4-DOF-Robotic-Arm/
│
├── README.md
│
├── Arduino/
│   └── robotic_arm.ino
│
├── Mobile-App/
│   └── app_files/
│
├── CAD/
│   └── robotic_arm_design/
│
├── 3D-Print/
│   └── STL/
│
├── Images/
│   └── robotic_arm.jpg
│
└── Videos/
    └── demonstration.mp4
```

---

## 📸 Project Documentation

The repository contains project photographs, CAD designs, 3D-printed components, source code, and demonstration videos documenting the development process.

### Suggested Images

- Complete robotic arm
- 3D CAD model
- 3D printing process
- Servo motor assembly
- ESP32 wiring
- Mobile application interface
- QR code
- Final working prototype

---

## 🚀 Learning Outcomes

Through this project, I gained practical experience in:

- Robotic arm design
- CAD modelling
- 3D printing
- Servo motor control
- ESP32 programming
- Wireless communication
- Mobile application development
- Hardware-software integration
- Mechanical assembly
- System testing and debugging

---

## 🔮 Future Improvements

Potential future improvements include:

- Adding a gripper with improved object-handling capability
- Implementing inverse kinematics for position-based control
- Adding preset pick-and-place sequences
- Integrating joystick-based control
- Adding object detection using a camera
- Improving precision through feedback-based servo control
- Developing autonomous pick-and-place functionality

---

## 👩‍💻 Internship Project

**Developed during Internship at Finstein Robotics**

**Domain:** Robotics | Embedded Systems | Mechatronics

**Controller:** ESP32

**Actuation:** Servo Motors

**Fabrication:** 3D Printing
