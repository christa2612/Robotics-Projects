# 🚗 Autonomous Line Following Robot

## 📌 Overview

This project is an **autonomous line-following robot** developed during my internship at **Finstein Robotics**. The robot is designed to follow a predefined path using an **8-array infrared (IR) sensor**, an **Arduino Nano**, DC geared motors, and a motor driver.

The IR sensor array continuously detects the position of the line relative to the robot. The Arduino Nano processes the sensor readings and determines the appropriate motor response, allowing the robot to move forward and correct its direction while following straight and curved sections of the track.

The project provided hands-on experience in **sensor interfacing, embedded programming, motor control, autonomous navigation, sensor calibration, and hardware integration**.

---

## 🎯 Objectives

- To develop an autonomous robot capable of following a predefined line.
- To interface an 8-array IR sensor with an Arduino Nano.
- To control DC geared motors using a motor driver.
- To process sensor readings to determine the position of the line.
- To implement real-time direction correction based on sensor feedback.
- To calibrate the sensor array for reliable line detection.
- To test the robot on different sections of a predefined track.
- To gain practical experience in embedded robotics and autonomous navigation.

---

## ⚙️ Features

- Autonomous line tracking
- 8-array IR sensor-based line detection
- Arduino Nano-based control
- Dual DC motor drive
- Motor driver-based direction and speed control
- Real-time path correction
- Sensor calibration
- Compact robotic platform
- Feedback-based movement correction

---

## 🧰 Hardware Components

| Component | Quantity | Function |
|---|---:|---|
| Arduino Nano | 1 | Main controller |
| 8-Array IR Sensor | 1 | Detects the line position |
| Motor Driver | 1 | Controls motor direction and speed |
| DC Geared Motors | 2 | Drives the robot |
| Robot Chassis | 1 | Supports the components |
| Wheels | 2 | Provides movement |
| Caster Wheel | 1 | Provides balance and support |
| Battery | 1 | Powers the robot |
| Jumper Wires | As required | Electrical connections |
| Switch | 1 | Power control |

---

## 💻 Software & Tools

- **Arduino IDE**
- **Embedded C/C++**
- Arduino Nano
- 8-channel IR sensor module

---

## 🔧 Hardware Architecture

The major hardware components are connected as follows:

```text
              ┌───────────────────────┐
              │      8-Array IR       │
              │        Sensor         │
              └───────────┬───────────┘
                          │
                    Sensor Readings
                          │
                          ▼
              ┌───────────────────────┐
              │     Arduino Nano      │
              │                       │
              │  Sensor Processing    │
              │  Decision Making      │
              └───────────┬───────────┘
                          │
                    Motor Commands
                          │
                          ▼
              ┌───────────────────────┐
              │     Motor Driver      │
              └───────────┬───────────┘
                          │
                 ┌────────┴────────┐
                 ▼                 ▼
           ┌──────────┐       ┌──────────┐
           │  Motor L │       │  Motor R │
           └─────┬────┘       └────┬─────┘
                 │                 │
                 ▼                 ▼
              Left Wheel       Right Wheel
```

---

## 🔍 Working Principle

The robot follows a predefined line by continuously monitoring the surface using the **8-array IR sensor**.

The sensor consists of multiple infrared sensing elements arranged in a row. Each sensing element detects the reflectivity of the surface below it.

For a typical black-line-on-white-surface track:

- **White surface** reflects more infrared light.
- **Black line** absorbs more infrared light.
- The sensor array therefore produces different readings depending on the position of the line.

The Arduino Nano continuously reads the sensor outputs and determines the approximate position of the line relative to the robot.

Based on the detected position, the controller determines whether the robot needs to:

- Correct towards the left
- Continue forward
- Correct towards the right

This process is continuously repeated while the robot is operating.

---

## 🔄 Line Following Logic

The basic control concept can be represented as:

```text
              Start
                │
                ▼
       Read IR Sensor Array
                │
                ▼
       Determine Line Position
                │
       ┌────────┼────────┐
       │        │        │
       ▼        ▼        ▼
     LEFT     CENTER    RIGHT
       │        │        │
       ▼        ▼        ▼
 Turn Left   Move       Turn Right
             Forward
       │        │        │
       └────────┼────────┘
                │
                ▼
        Read Sensors Again
                │
                └──────► Repeat
```

The feedback loop allows the robot to continuously respond to changes in the position of the line.

---

## 🚦 Motor Control

The two DC geared motors are independently controlled through the motor driver.

### Moving Forward

```text
Left Motor  → Forward
Right Motor → Forward
```

Both wheels rotate in the forward direction to move the robot along the track.

### Turning Left

```text
Left Motor  → Slow / Stop
Right Motor → Forward
```

The robot corrects its trajectory towards the left.

### Turning Right

```text
Left Motor  → Forward
Right Motor → Slow / Stop
```

The robot corrects its trajectory towards the right.

### Stopping

```text
Left Motor  → Stop
Right Motor → Stop
```

The exact motor response can be adjusted through the control algorithm depending on the sensor readings and track requirements.

---

## 🧠 Sensor-Based Decision Making

The 8 sensor elements provide information about the position of the line.

A simplified representation is:

```text
S1   S2   S3   S4   S5   S6   S7   S8
│    │    │    │    │    │    │    │
←──────────── LINE ────────────────→
```

When the line is detected towards the left side of the sensor array, the robot corrects its movement towards the left.

When the line is detected near the center, the robot continues forward with the appropriate motor response.

When the line is detected towards the right side, the robot corrects its movement towards the right.

This continuous sensor-feedback process enables autonomous path tracking.

---

## 🔌 System Flow

```text
IR Sensor Array
       ↓
Detect Line
       ↓
Generate Sensor Readings
       ↓
Arduino Nano
       ↓
Determine Line Position
       ↓
Calculate Direction Correction
       ↓
Motor Driver
       ↓
Control Left & Right Motors
       ↓
Robot Movement
       ↓
Line Position Changes
       ↓
Read Sensors Again
```

---

## 🧪 Calibration & Testing

Calibration was an important part of the development process.

The IR sensor array was tested over the line and the surrounding surface to understand the sensor response. Sensor positioning and threshold conditions were adjusted to obtain reliable line detection.

The robot was subsequently tested on a predefined track to evaluate:

- Straight-line tracking
- Left turns
- Right turns
- Curved paths
- Sensor response
- Motor response
- Direction correction
- Overall tracking stability

Repeated testing was performed to improve the consistency and accuracy of the robot's movement.

---

## 🛠️ Troubleshooting

### Incorrect Line Detection

Sensor readings were checked and calibrated to improve the distinction between the line and the surrounding surface.

### Uneven Motor Movement

The motor connections and responses were checked to ensure that the robot could maintain a stable trajectory.

### Overshooting at Turns

The motor response was adjusted to improve directional correction and reduce excessive deviation from the line.

### Sensor Alignment

The position and orientation of the IR sensor array were checked to ensure that the sensing elements remained appropriately aligned with the track.

---

## 📂 Repository Structure

```text
Line-Following-Robot/
│
├── README.md
│
├── Arduino/
│   └── line_follower.ino
│
├── Circuit/
│   └── circuit_diagram.png
│
├── Images/
│   ├── robot.jpg
│   ├── sensor_array.jpg
│   └── assembly.jpg
│
└── Videos/
    └── line_following_demo.mp4
```

---

## 🚀 Future Improvements

The robot can be further enhanced by implementing:

- PID-based line-following control
- Encoder-based motor feedback
- Higher-speed navigation
- Junction and intersection detection
- Multiple-path detection
- Wireless monitoring
- Obstacle detection
- Autonomous route selection
- Improved sensor fusion
- More precise motor-speed control

---

## 📌 Project Outcome

A functional **autonomous line-following robot** was developed using an Arduino Nano, 8-array IR sensor, motor driver, and DC geared motors.

The system was able to detect the position of a predefined line and continuously adjust the motor response to follow the track.

The project demonstrated the practical implementation of a **sensor-based autonomous navigation system** using embedded control.

---

## File Access Note

Some project files are large and may not be previewed directly on
GitHub. For these files, use the **View raw** option on the GitHub
file page to access the original file.

---

## 📂 Project Documentation

- [Arduino](Code/)
- [Images](Model-Images/)
- [working Videos](Working-Videos/)
