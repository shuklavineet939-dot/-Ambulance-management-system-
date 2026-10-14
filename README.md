# Ambulance Management System (DSA Mini Project)

## Overview
A menu-driven C program that demonstrates a basic ambulance dispatch workflow.

## Features
- Register a patient and pickup location
- Maintain a FIFO waiting queue
- Assign an available ambulance to the next waiting patient
- Display ambulance status and patient records
- Mark an ambulance trip as complete
- Display the waiting queue

## Data structures and concepts
- `struct` for patient and ambulance records
- Array for storing records
- Circular queue for waiting patients (FIFO)
- Linear search to find an available ambulance

## Requirements
A C compiler such as GCC, Code::Blocks, or Dev-C++.

## Compile and run (GCC)
```bash
gcc ambulance.c -o ambulance
./ambulance
```
On Windows:
```bash
gcc ambulance.c -o ambulance.exe
ambulance.exe
```

## Quick test
1. Run the program.
2. Choose `1` and enter a patient name, age, pickup location, and emergency level.
3. Choose `5` to view the waiting queue.
4. Choose `2` to assign an ambulance.
5. Choose `3` to see ambulance status.
6. Choose `6` and enter the assigned ambulance ID to mark the trip complete.

## Limitations
This is an educational console-based DSA project. Records are stored in memory and are cleared when the program exits. It is not connected to real emergency services and must not be used for actual dispatch.

## Submission checklist
- Add your name, roll number, class, and college to your report if required.
- Compile and test the program on your own computer.
- Take screenshots of the running program if your teacher requires them.
- Upload the source code and README to your own GitHub repository.
