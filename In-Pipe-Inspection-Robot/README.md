# Spring-Based In-Pipe Inspection Robot

## Overview

This project presents the design and development of a spring-based
wall-pressed in-pipe inspection robot for large-diameter pipelines.

The robot is designed to operate in pipelines with diameters ranging
from 550–600 mm. It uses a three-arm configuration with spring-based
radial expansion to maintain contact between the wheels and the
inner pipe wall.

The mechanical design combines a rocker-bogie wheel arrangement with
passive spring expansion to provide adaptability to variations in
pipe diameter and surface conditions.

## Objectives

- Design a robot capable of travelling inside large-diameter pipelines.
- Maintain wheel-to-pipe contact using spring-based radial expansion.
- Provide stable locomotion over varying pipe surfaces.
- Develop a rocker-bogie mechanism for passive adaptation.
- Integrate inspection and cleaning capabilities.
- Perform force, torque and stress analysis.
- Validate the mechanical design using Finite Element Analysis (FEA).

## System Configuration

The robot consists of three arms arranged at 120° around the central
body.

Each arm contains:

- Rocker-bogie wheel arrangement
- Three wheels
- DC geared motors
- Spring-based radial expansion mechanism

The complete robot therefore uses nine wheels.

## Working Principle

The robot uses friction-driven locomotion.

The spring mechanism generates the normal force required to maintain
contact between the wheels and the inner pipe wall.

The available traction force is expressed as:

F_traction = μ F_normal

The generated traction must overcome rolling resistance and the
gravitational component when operating on inclined pipelines.

## Mechanical Design

### Rocker-Bogie Mechanism

The rocker-bogie arrangement allows the wheel assemblies to adapt to
uneven surfaces while maintaining wheel contact with the pipeline wall.

### Spring-Based Radial Expansion

Compression springs push the arms outward against the pipe wall.

This passive expansion mechanism allows the robot to accommodate
variations in pipeline diameter without requiring a separate active
expansion actuator.

## Force and Torque Analysis

The robot weight is approximately 44 N for a mass of 4.5 kg.

The calculated rolling resistance is approximately 2.2 N, while the
gravitational component for a 10° inclined pipe is approximately 7.6 N.

A design force of 30 N was considered.

The calculated wheel torque requirement is approximately 1.05 Nm.

## Spring Design

Key spring parameters include:

- Free length = 100 mm
- Deflection = 60 mm
- Outer diameter = 20 mm
- Wire diameter = 1.5 mm
- Mean diameter = 18.5 mm
- Spring index = 12.3
- Wahl factor = 1.1164

The calculated spring load is approximately 29 N.

## Finite Element Analysis

SOLIDWORKS Simulation was used to analyse the mechanical components.

### Arm FEA

The arm analysis produced a maximum stress concentration of
approximately 7.015 MPa.

### Spring FEA

The spring was analysed under an approximately 28 N static load.

The simulation produced:

- Maximum von Mises stress ≈ 168 MPa
- Yield strength = 275 MPa
- Factor of safety ≈ 1.6
- Maximum displacement ≈ 40 mm

## Inspection and Cleaning

The proposed system integrates:

- Onboard camera for visual inspection
- Rotating brush for cleaning
- Water jet for cleaning

This enables inspection and cleaning to be performed using the
same robotic platform.

## Applications

Potential applications include:

- Municipal water-main inspection
- Pipeline inspection
- Internal pipeline cleaning
- Infrastructure maintenance
- Visual inspection of large-diameter pipelines

## Key Features

| Feature | Description |
|---|---|
| Operating diameter | 550–600 mm |
| Arm configuration | 3 arms at 120° |
| Number of wheels | 9 |
| Expansion | Spring-based |
| Locomotion | Friction-driven |
| Suspension | Rocker-bogie |
| Inspection | Onboard camera |
| Cleaning | Brush + water jet |
| CAD/FEA | SOLIDWORKS |

## Tools Used

- SOLIDWORKS
- SOLIDWORKS Simulation
- Mechanical design
- Force and torque analysis
- Finite Element Analysis

## Project Outcome

The project demonstrates a passive mechanical approach for maintaining
traction and adapting to variations in pipeline diameter.

The combination of spring-based radial expansion and rocker-bogie
wheel assemblies provides a mechanically simple approach for
pipeline inspection and cleaning.
