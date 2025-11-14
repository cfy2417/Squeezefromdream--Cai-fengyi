## Psychophysics-Based Serious Game

## A lightweight serious game designed to measure human perceptual thresholds and decision responses through interactive tasks.

## 🌟 Overview
This project is a human-computer interaction research project that combines game design. Its inspiration comes from workplace violence (bullying at work). Its core objective is, on the one hand, to provide a low-cost and highly user-engaged solution for similar psychological disorders; on the other hand, it also offers a technologically innovative perspective for optimizing the therapeutic effect and user experience of serious games.

## 🎮 Features
Game-based perceptual discrimination tasks
Reaction time (RT) tracking with high temporal resolution
Automatic data export (CSV / JSON)
The designed interactive supporting hardware

## 🧪 Implemented Psychophysics Methods
Method	Purpose
JND Measurement	Estimates smallest detectable difference
Adaptive Staircase	Converges toward perceptual threshold
Signal Detection Theory	Computes sensitivity and decision bias
Reaction Time Analysis	Tracks cognitive load and latency

## 🎯 Project Goals
Bring psychophysical measurement into a game context
Provide a reusable tool for perceptual and cognitive research
Combine scientific rigor with engaging interaction design

## 🧩 Structure
/Users/caifengyi/Desktop/Squeezefromdream
├── assets
│   └── squeezebrief.pdf
├── datacollection
│   ├── fix_my_data.m
│   ├── gsrcollectiontool
│   │   └── gsrpifudian.ino
│   └── TOngji_formchart.m
├── Experiment
│   ├── figure
│   │   ├── joystick ver2.txt
│   │   └── test_keyboard.txt
│   ├── joystickgroup
│   │   └── joystick ver2.mat
│   └── keyboardGroup
│       ├── test_keyboard_scrlist.mat
│       └── test_keyboard.mat
├── hardware
│   ├── shoubingleft
│   │   └── shoubingleft.ino
│   └── shoubingright
│       └── shoubingright.ino
├── result
│   ├── figurechart
│   │   ├── figure1.fig
│   │   ├── figure2.fig
│   │   └── figure3.fig
│   ├── leda_batchanalysis.m
│   └── SCR_Txt_Comparison_Result.mat
└── UEscript
    └── signalreciver
        ├── ArduinoInputComponent.cpp
        ├── ArduinoInputComponent.h
        ├── JoystickBlueprintLibrary.cpp
        ├── JoystickBlueprintLibrary.h
        ├── NewActorComponent.cpp
        ├── NewActorComponent.h
        ├── OSCReceiver.cpp
        ├── OSCReceiver.h
        ├── workvoilencegame.cpp
        └── workvoilencegame.h

## 📘 Note
This repository serves as a demonstration of the project's technical research and development process.  
Some implementation details, data, and experimental records have been simplified for public release.

For further information or collaboration, please contact:
📩 nissen2417@gamil.com
