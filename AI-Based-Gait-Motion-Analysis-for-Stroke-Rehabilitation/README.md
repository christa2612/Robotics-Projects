# AI-Based Gait Motion Analysis for Stroke Rehabilitation

## Overview

This project presents an AI-based framework for automated detection of stroke-related gait abnormalities using biomechanical simulation and deep learning.

The proposed approach combines OpenSim-based musculoskeletal modelling with a hybrid Graph Convolutional Network (GCN) and Gated Recurrent Unit (GRU) architecture. OpenSim is used to obtain biomechanical gait information, while the hybrid deep learning model captures both the spatial relationships between lower-limb joints and the temporal patterns present during the gait cycle.

In addition to classifying gait patterns as normal or stroke-affected, the framework performs joint-level abnormality analysis to identify lower-limb features associated with pathological gait.

The project explores the application of artificial intelligence, biomechanics, and computational gait analysis for data-driven stroke rehabilitation.

---

## Objectives

- Develop an automated framework for detecting stroke-related gait abnormalities.
- Use OpenSim for biomechanical modelling and gait analysis.
- Extract joint kinematics and gait-related parameters from motion data.
- Apply Graph Convolutional Networks to learn spatial relationships between connected joints.
- Apply Gated Recurrent Units to capture temporal gait patterns.
- Combine spatial and temporal information using a hybrid GCN-GRU architecture.
- Classify gait cycles into normal and stroke-affected categories.
- Analyze joint-level abnormality patterns for improved interpretability.
- Explore the potential of AI-assisted gait analysis for rehabilitation applications.

---

## Problem Statement

Stroke can cause significant changes in walking patterns, including abnormalities in joint movement, coordination, symmetry, and gait timing.

Traditional gait assessment methods often depend on clinical observation and scoring systems. Conventional machine learning approaches can classify gait patterns, but they may not fully capture the spatial relationships between joints and the temporal nature of human movement.

This project addresses this challenge by combining biomechanical modelling with deep learning to analyze gait patterns in a spatial-temporal manner.

---

## Methodology

The proposed workflow consists of the following stages:

    Motion Capture Data
            ↓
    OpenSim Model Scaling
            ↓
    Inverse Kinematics
            ↓
    Kinematic Data Extraction
            ↓
    Data Preprocessing
            ↓
    Feature Engineering
            ↓
    Hybrid GCN-GRU Model
            ↓
    Gait Classification
            ↓
    Joint-Level Abnormality Analysis
            ↓
    Performance Evaluation

---

## OpenSim-Based Biomechanical Analysis

OpenSim 4.3 was used as the musculoskeletal simulation platform.

A generic lower-limb musculoskeletal model was scaled according to participant-specific body measurements. Model scaling was performed to adapt the model's bone segment lengths and joint centres to the physical characteristics of individual participants.

Inverse Kinematics was then used to determine joint angles and segment orientations throughout the gait cycle.

The extracted motion data included:

- Hip joint angles
- Knee joint angles
- Ankle joint angles
- Pelvic tilt and rotation
- Step length
- Stride duration
- Lower-limb segment displacement

The resulting kinematic time-series data was stored in `.mot` files for machine learning analysis.

---

## Data Preprocessing

The extracted gait datasets were processed before being provided to the deep learning model.

The preprocessing pipeline included:

- Noise and outlier handling
- Data normalization
- Temporal alignment
- Feature extraction
- Gait-cycle standardization

Each gait cycle was rescaled to the complete stride period so that samples from different subjects could be compared on a common temporal scale.

---

## Feature Engineering

The selected gait features included:

- Hip flexion/extension
- Knee flexion/extension
- Ankle dorsiflexion/plantarflexion
- Stride length
- Pelvic obliquity

These parameters were selected to capture important differences between normal and stroke-affected gait patterns.

---

## Hybrid GCN-GRU Architecture

### Graph Convolutional Network (GCN)

The lower-limb musculoskeletal system was represented as a graph.

- Nodes represent joints.
- Edges represent biomechanical connections between joints.
- GCN layers learn spatial relationships between connected joints.

This allows the model to capture interactions between different parts of the lower limb.

### Gated Recurrent Unit (GRU)

GRU layers were used to process sequential gait data throughout the walking cycle.

The GRU captures temporal characteristics such as:

- Joint movement variations
- Sequential gait patterns
- Rhythmic motion
- Temporal differences between normal and stroke gait

### Combined Model

The GCN and GRU components were combined to capture both spatial and temporal characteristics of human gait.

    Lower-Limb Joint Relationships
                 ↓
                GCN
                 ↓
          Spatial Features
                 ↓
                GRU
                 ↓
          Temporal Features
                 ↓
          Gait Classification
                 ↓
       Joint-Level Abnormality Analysis

---

## Model Training

The dataset was divided into three subsets:

| Dataset | Split |
|---|---:|
| Training | 70% |
| Validation | 15% |
| Testing | 15% |

The model was trained using:

- Optimizer: Adam
- Learning Rate: 0.001
- Loss Function: Cross-Entropy Loss
- Early Stopping: Used to reduce overfitting

---

## Performance Evaluation

The model was evaluated using multiple performance metrics:

- Accuracy
- Precision
- Recall
- F1-Score
- Confusion Matrix
- ROC Curve
- Area Under the Curve (AUC)

These metrics were used to evaluate the model's ability to distinguish between normal and stroke-affected gait patterns.

---

## Results

The hybrid GRU-GCN model demonstrated strong classification performance on the dataset used in the study.

The reported results included:

- 27,031 healthy gait sequences
- 9,414 stroke gait sequences
- Approximately 100% training and validation accuracy
- ROC-AUC: 1.000

The confusion matrix showed correct classification of the reported healthy and stroke gait sequences.

The results demonstrate the feasibility of combining biomechanical simulation with spatial-temporal deep learning for gait classification.

> Note: These results are based on the simulated/processed data used in this study and should not be interpreted as clinical diagnostic performance.

---

## Joint-Level Abnormality Analysis

Beyond binary gait classification, the project also investigated which lower-limb features contributed to abnormal gait patterns.

The abnormality analysis identified notable differences in features including:

- Tibialis anterior activity
- Pelvic angle-related features
- Rectus femoris activity

This analysis provides an interpretable component by highlighting lower-limb features associated with stroke-affected gait.

---

## Applications

The proposed framework has potential applications in:

- Stroke rehabilitation
- Automated gait assessment
- Computational biomechanics
- Physiotherapy support
- Rehabilitation monitoring
- AI-assisted movement analysis
- Clinical decision-support research
- Future wearable gait-monitoring systems

---

## Future Scope

Future development can focus on:

- Validation using real-world motion-capture datasets
- Expansion of the patient dataset
- Inclusion of different stroke severities and rehabilitation stages
- Integration of additional EMG channels
- Incorporation of three-dimensional gait parameters
- Real-time gait monitoring
- Wearable sensor integration
- Smartphone-based gait assessment
- Personalized rehabilitation guidance
- Automated rehabilitation progress tracking
- Transfer learning and ensemble approaches

---

## Project Workflow

    Data Collection
          ↓
    OpenSim Model Scaling
          ↓
    Inverse Kinematics
          ↓
    Kinematic Data Extraction
          ↓
    Preprocessing & Normalization
          ↓
    Feature Engineering
          ↓
    GCN-GRU Model Development
          ↓
    Model Training
          ↓
    Gait Classification
          ↓
    Joint-Level Analysis
          ↓
    Performance Evaluation

---

## Technologies Used

### Biomechanical Simulation

- OpenSim 4.3

### Artificial Intelligence

- Graph Convolutional Networks (GCN)
- Gated Recurrent Units (GRU)
- Deep Learning
- Spatial-Temporal Learning

### Data Processing

- Kinematic Time-Series Data
- `.mot` Motion Files
- Feature Normalization
- Gait-Cycle Alignment

### Evaluation

- Accuracy
- Precision
- Recall
- F1-Score
- Confusion Matrix
- ROC Curve
- AUC

---

## Project Outcome

The project demonstrates a proof-of-concept framework that combines OpenSim-based biomechanical simulation with a hybrid GCN-GRU deep learning model for automated gait classification and abnormality analysis.

The approach provides both classification of gait patterns and additional insight into lower-limb features associated with abnormal movement.

---

## 📁 File Access Note

Some project files are large and may not be previewed directly on GitHub. For these files, use the **View raw** option on the GitHub file page to access the original file.

---

## Project Documentation

- [OpenSim Models](OpenSim/)
- [Model](Model/)
- [Results](Results/)
- [Figures](Figures/)
- [Project Report](Report/)

---

