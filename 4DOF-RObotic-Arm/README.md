# 🤖 4-DOF Robotic Arm with ESP32 & Mobile App Control

## 📌 Overview

This project involved the development and integration of a **4-DOF robotic arm system** during my internship at **Finstein Robotics**.

The robotic arm uses an **ESP32 microcontroller** to control four servo motors responsible for the movement of its joints. As part of the project, I specifically worked on the **CAD design and development of the robotic arm base**, with emphasis on structural support, servo mounting, stability, and integration with the existing arm assembly.

The designed base was fabricated using **3D printing** and integrated with the robotic arm. The project also involved ESP32-based servo control and a dedicated mobile application for wireless operation of the robotic arm. A **QR-code-based access system** was implemented to provide convenient access to the mobile control application.

This project provided practical experience in **CAD design, mechanical prototyping, 3D printing, servo motor control, ESP32 programming, mobile application integration, and hardware-software system integration**.

---

## 🎯 Objectives

- To contribute to the development and integration of a functional 4-DOF robotic arm system.
- To design a stable and functional **base for the robotic arm**.
- To develop the base considering servo mounting, structural support, and integration requirements.
- To fabricate the designed base using 3D printing.
- To integrate four servo motors with an ESP32-based control system.
- To implement wireless control of the robotic arm through a mobile application.
- To provide convenient access to the mobile application using a QR code.
- To gain practical experience in mechanical design and robotic system integration.
- To understand the interaction between mechanical, electronic, and software components in a robotic system.

---

## ⚙️ System Features

- 4-degree-of-freedom robotic arm
- Custom-designed robotic arm base
- 3D-printed base
- Four servo motors for joint actuation
- ESP32-based control system
- Wireless robotic arm control
- Dedicated mobile application
- QR-code-based application access
- Individual joint movement control
- Coordinated robotic arm movement
- Modular hardware-software integration

---

## 🏗️ My Contribution

My primary mechanical contribution to this project was the **design and development of the robotic arm base**.

The base was designed using CAD software with consideration for:

- Structural stability
- Servo motor mounting
- Proper alignment
- Mechanical support
- Integration with the existing robotic arm
- Ease of assembly
- Overall footprint and mounting requirements

After completing the CAD design, the base was fabricated using **3D printing** and integrated with the robotic arm assembly.

In addition to the mechanical design contribution, I was involved in the integration and testing of the robotic arm control system, including ESP32-based servo control and mobile application interaction.

### Key Areas of My Work

- Robotic arm base design
- CAD modelling
- Mechanical design considerations
- 3D-printing preparation and fabrication
- Servo mounting and integration
- ESP32 integration
- Servo motor control
- Mobile application integration
- Hardware-software integration
- Testing and calibration

---

## 🧩 Mechanical Design – Robotic Arm Base

The robotic arm base serves as the primary structural support for the arm assembly.

The base was designed to provide a stable mounting platform while accommodating the required servo and mechanical connections.

### Design Considerations

The base design considered:

- Stability of the robotic arm
- Mounting requirements
- Servo placement
- Alignment of the rotating joint
- Structural support
- Ease of fabrication
- Ease of assembly and maintenance
- Compatibility with the existing robotic arm structure

The final base design was fabricated using **3D printing** and assembled with the remaining robotic arm components.

---

## 🖨️ 3D Printing

The designed robotic arm base was fabricated using a **3D printing process**.

The general workflow was:

```text
CAD Design
     ↓
Design Verification
     ↓
3D Printing Preparation
     ↓
3D Printing
     ↓
Post-Processing
     ↓
Assembly
     ↓
Integration of Designed Base with Robotic Arm
```

3D printing enabled rapid fabrication and testing of the custom-designed base while allowing modifications to be made efficiently during the development process.

---

## 🔌 Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 | 1 | Main microcontroller |
| Servo Motors | 4 | Robotic arm joint actuation |
| 3D-Printed Base | 1 | Custom-designed structural support |
| Robotic Arm Assembly | 1 | Mechanical arm structure |
| Power Supply | 1 | System power |
| Connecting Wires | As required | Electrical connections |

---

## 💻 Software & Tools

### Hardware Programming

- ESP32
- Arduino IDE
- Embedded C/C++

### Mechanical Design

- CAD software
- 3D-printing software / slicer

### Mobile Control

- Dedicated mobile application
- Wireless communication
- QR-code-based application access

---

## 🔌 System Architecture

The overall system follows the architecture:

```text
              Mobile Application
                     │
                     │ Wireless Commands
                     ↓
                   ESP32
                     │
              Servo Control Signals
                     │
          ┌──────────┼──────────┐
          ↓          ↓          ↓
       Servo 1    Servo 2    Servo 3 ... Servo 4
          │          │          │          │
          └──────────┴──────────┴──────────┘
                         ↓
                 4-DOF Robotic Arm
                         │
                         ↓
              Custom-Designed Base
```

The mobile application sends movement commands wirelessly to the ESP32. The ESP32 processes these commands and generates the required control signals for the servo motors.

The servo motors actuate the corresponding joints, allowing the robotic arm to perform the required movements.

The custom-designed base provides the mechanical support and mounting interface for the arm assembly.

---

## 📱 Mobile Application

A dedicated mobile application was used to provide wireless control of the robotic arm.

The application allows the user to send movement commands to the robotic arm without requiring direct physical interaction with the controller.

### Main Functions

- Wireless robotic arm control
- Joint movement commands
- Individual servo control
- Coordinated movement
- User-friendly control interface

A **QR code** was also implemented to provide convenient access to the mobile application.

### Control Flow

```text
User
 ↓
Mobile Application
 ↓
Wireless Command
 ↓
ESP32
 ↓
Servo Control
 ↓
Robotic Arm Movement
```

---

## 🔄 Working Principle

The robotic arm operates through communication between the mobile application, ESP32, servo motors, and mechanical assembly.

### Step 1 – Application Access

The user accesses the robotic arm control application using the provided mobile application or QR-code-based access.

### Step 2 – Movement Command

The user selects the required robotic arm movement through the mobile application.

### Step 3 – Wireless Communication

The movement command is transmitted wirelessly to the ESP32.

### Step 4 – Command Processing

The ESP32 receives and processes the command.

### Step 5 – Servo Actuation

The ESP32 generates the appropriate control signals for the corresponding servo motors.

### Step 6 – Robotic Arm Movement

The servo motors move the respective joints of the robotic arm.

### Step 7 – Mechanical Support

The robotic arm assembly is supported by the custom-designed 3D-printed base.

---

## 🧪 Testing & Calibration

Testing was carried out to verify the operation and integration of the robotic arm system.

The testing process included:

1. Individual servo motor testing.
2. Checking servo connections.
3. Calibrating servo positions.
4. Testing individual joint movements.
5. Testing ESP32 control.
6. Testing wireless communication.
7. Testing mobile application commands.
8. Integrating the control system with the complete arm.
9. Testing coordinated movement.
10. Verifying the stability and integration of the custom-designed base.

---

## 📂 Repository Structure

```text
4DOF-RObotic-Arm/
│
├── README.md
│
├── Design and model Images/
│
├── Software-code/
│
└── Working Videos/
```

---

## 📸 Project Documentation

The repository contains documentation related to the development and demonstration of the robotic arm system.

### Design and Model Images

The design documentation includes:

- CAD model of the custom-designed robotic arm base
- Base design views
- Fabricated base
- Robotic arm assembly
- Final integrated prototype

### Software

The software section contains the code and related files used for controlling the robotic arm through the ESP32.

### Working Videos

Demonstration videos show the operation of the robotic arm and its control system.

---

## 🧠 Learning Outcomes

This project provided practical experience in:

- Mechanical CAD design
- Robotic arm base design
- 3D modelling
- Design for fabrication
- 3D printing
- Servo motor integration
- ESP32 programming
- Embedded systems
- Wireless communication
- Mobile application integration
- Hardware-software integration
- Mechanical assembly
- System testing and calibration
- Robotics prototyping

---

## 🚀 Future Improvements

The robotic arm system can be further enhanced by incorporating:

- A dedicated robotic gripper
- Inverse kinematics-based control
- End-effector position control
- Joystick-based control
- Preset movement sequences
- Pick-and-place functionality
- Camera-based object detection
- Autonomous object manipulation
- Feedback-based servo positioning
- Trajectory planning
- Computer vision-based robotic arm control

---

## 👩‍💻 Internship Project

**Organization:** Finstein Robotics  
**Domain:** Robotics | Embedded Systems | Mechatronics  
**Project:** 4-DOF Robotic Arm

### Role and Contribution

During my internship at Finstein Robotics, I contributed to the development and integration of a 4-DOF robotic arm system.

My primary mechanical contribution was the **CAD design and development of the robotic arm base**, followed by its fabrication using 3D printing and integration with the robotic arm assembly.

I also gained hands-on experience in:

- CAD modelling
- Mechanical prototyping
- 3D printing
- Servo motor integration
- ESP32 programming
- Wireless control
- Mobile application integration
- Hardware-software integration
- Testing and debugging

---

## 📌 Project Outcome

A functional **4-DOF robotic arm system** was developed and demonstrated with ESP32-based servo control and wireless mobile application interaction.

The project provided practical experience in integrating **mechanical design, 3D-printed components, embedded control, wireless communication, and mobile application control**.

A key outcome of the project was the successful **design, fabrication, and integration of a custom robotic arm base**.

---

## 📁 File Access Note

Some CAD, software, and project files may be large and may not be previewed directly on GitHub. If a file does not open in the GitHub preview, use the **View raw** option on the corresponding file page to access the original file.

---

## 📂 Project Documentation

- [Design and model Images](Design%20and%20model%20Images/)
- [Software-code](Software-code/)
- [Working Videos](Working%20Videos/)
