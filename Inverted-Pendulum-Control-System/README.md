# Inverted Pendulum Control System

## Overview

This project focuses on the modelling, simulation, and control of an
inverted pendulum system using MATLAB Simulink.

An inverted pendulum is an inherently unstable system in which a
pendulum must be maintained in an upright position using a controlled
force applied to a movable cart.

The project models the nonlinear cart-pendulum dynamics and develops
a feedback control system to stabilize the pendulum while allowing the
cart to follow a desired reference trajectory.

## Objectives

- Model the dynamics of an inverted pendulum system.
- Develop the cart-pendulum system in MATLAB Simulink.
- Stabilize the pendulum in the upright position.
- Use state feedback to generate the control input.
- Track a predefined reference trajectory for the cart.
- Analyse the cart position and pendulum angle response.
- Study the application of inverted pendulum control in robotics.

## System Description

The system consists of:

- A movable cart
- An inverted pendulum attached to the cart
- A controlled force applied to the cart
- Feedback from the system states
- A reference position for cart motion

The two main physical variables are:

- q1 — cart displacement
- q2 — pendulum angle

The controller uses the system state information to determine the
control force required to maintain stability.

## System Parameters

| Parameter | Value |
|---|---:|
| Cart mass (mc) | 1.5 kg |
| Pendulum mass (mp) | Variable |
| Pendulum length (L) | 1 m |
| Gravity (g) | 9.82 m/s² |
| Cart damping (d1) | 0.01 |
| Joint damping (d2) | 0.01 |
| Initial state | [0.1; 0.17453; 0; 0] |

## State Representation

The controller receives a four-state vector consisting of:

- Cart position
- Cart velocity
- Pendulum angle
- Pendulum angular velocity

The state vector is used as feedback for generating the control input.

## Control Strategy

The Simulink model uses a state feedback control law of the form:

**u = −Kx**

where:

- `u` is the control input applied to the cart.
- `K` is the state feedback gain matrix.
- `x` is the system state vector.

The gain matrix used in the controller block is denoted as `K_LQR`
in the model and is associated with Linear Quadratic Regulator (LQR)
design.

The controller uses the difference between the desired cart position
and the actual cart position as part of the feedback process.

## Reference Trajectory

A reference trajectory is generated to define the desired motion of
the cart.

The reference subsystem consists of:

- Constant block with a value of -1.5
- Signal generator
- Summation block
- Reference position output (`pos_ref`)

The generated trajectory provides the desired cart position for the
closed-loop control system.

## MATLAB Simulink Model

The complete cart-pendulum system is modelled and simulated using
MATLAB Simulink.

The model consists of:

1. Reference trajectory generation
2. State feedback controller
3. Nonlinear cart-pendulum plant
4. Feedback of system states
5. Closed-loop response analysis

## Simulation Results

The simulation results are analysed using the cart position and
pendulum angle.

### Cart Position

The cart position is represented by `q1` and measured in metres.

### Pendulum Angle

The pendulum angle is represented by `q2` and measured in degrees.

The closed-loop response demonstrates the behaviour of the cart and
pendulum under feedback control.

## Applications

Inverted pendulum control concepts are applicable to:

- Self-balancing robots
- Mobile robotic platforms
- Robotics control systems
- Aerospace control
- Dynamic stabilization systems

A self-balancing robot is a practical example where an IMU can detect
deviations from the upright position and feedback control can generate
corrective motor commands.

## Tools Used

- MATLAB
- Simulink
- Control Systems
- Dynamic System Modelling
- State Feedback Control

## Project Outcome

The project demonstrates the modelling and closed-loop control of an
inherently unstable inverted pendulum system.

The Simulink model provides a platform for analysing system dynamics,
feedback control, reference tracking, and stabilization.

## Future Improvements

Potential extensions include:

- Detailed LQR controller design and tuning
- PID controller implementation and comparison
- Fuzzy logic control
- Sensor noise analysis
- Actuator saturation analysis
- Experimental implementation using a self-balancing robot

## Project Documentation

- [MATLAB Files](MATLAB/)
- [Simulink Model](Simulink/)
- [Simulation Figures](figures/)
- [Results](results/)
- [Project Report](report/)
