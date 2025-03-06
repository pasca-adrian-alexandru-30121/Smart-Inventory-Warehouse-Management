# 🛢 Smart Warehouse Inventory Management

## 🔍 Project Overview

This repository contains the implementation of a Smart Warehouse Inventory Management system, developed as a semester project for the course "Sisteme Bazate pe Cunoaștere". The system features two autonomous robots capable of performing warehouse management tasks, such as controlled movement and path detection.

## 🔢 Objectives

- Collect performance data from DC motors to analyze their behavior.

- Pre-process data to remove noise and prepare it for modeling.

- Identify transfer functions for both motors.

- Design and implement a PI Controller using the Ziegler-Nichols method.

- Implement a lane detection algorithm to enable autonomous robot navigation.

 ## 📊 System Specifications

- Two DC motors controlled by individual PI controllers.

- Optical sensors for path detection.

- Development Environments:

    - Python for data pre-processing

    - MATLAB for system modeling and control

    - Arduino for functional testing

## 📖 Dataset Analysis and Pre-processing

### Data Collection

- Three different PWM values: 85, 128, and 255.

- Motors powered by 4 batteries (3.7V each), arranged in two series pairs, delivering a total of 7.4V.

- Data sampling period: 0.01s.

- Data saved in individual Excel files and imported into MATLAB for visualization.

### Pre-processing

- Applied noise filtering techniques:

  - Low-pass filter for right motor (PWM = 85, 128, 255)

  - Median filter for left motor (PWM = 85)

- Data was split into:

  - Identification data

  - Training/validation data
 
## 🎨 System Modeling

Transfer Function Identification

- Applied step response analysis to derive a grey-box model.

- Transfer Function: First-order system:

#### G(s) = K / (T * s + 1)

- Identified Parameters:

  - Left Motor: K = 24.2915, T = 0.01

  - Right Motor: K = 30.1339, T = 0.06

## ⚙️ Controller Design

PI Controller Design

- Used the Ziegler-Nichols method.

- Brought system to the stability limit to determine:

  - Kcr (Critical Gain)

  - Tcr (Critical Period)

- Calculated PI controller parameters based on standard Ziegler-Nichols table.

- Observed good response times for both motors, eliminating the need for derivative action (PI instead of PID).

## 🌍 Lane Detection

- Developed lane detection using two digital optical sensors.

- Behavior logic:

  - Both sensors detect line: Move forward.

  - Right sensor detects line: Stop left motor, turn left.

  - Left sensor detects line: Stop right motor, turn right.

## 📈 Results and Evaluation

- Evaluated step response of PI-controlled motors.

- Achieved effective tracking and stable behavior.

