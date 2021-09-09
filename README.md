# Stepper_motor_driver
Stepper Motor driver for Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board


The Instructions and Assumptions made while creating this Driver:
1. A Stepper Motor is a the 2-coil, bi-polar stepper motor, the driving pulses require 4-phase signals.
2. Stepper Motor takes 200 rotations to complete one full rotation i.e 1.8 degree per step.
3. The maximum speed the motor can reach is 1000 RPM.
4. The PWMA and PWMB pins are connected to Vcc and STBY pin is connected to GND.
5. The driver is developed for the Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board.
6. The STM32CubeMx HAL is used for developing the driver and musted to used to run the driver API.
7. The driver is developed for ARM Cortex M4 but also supports Cortex-M3 and M7 devices.
8. The processor of the Microcontroller speed is assumed to be 84MHz.
9. Timing diagram of the 4 pins.

![image](https://user-images.githubusercontent.com/90426334/132776054-18f89078-9daa-4ae4-9b74-e2cd8d63447f.png)

The cycles required for Step Delay calculation:
speed delay(seconds) = (60 * step angle) / (speed _in_RPM * FULL_Rotation_angle)

CPU cycles required to calculate the speed delay:
step_cycles = (Frequency of processor) / (speed_delay)
