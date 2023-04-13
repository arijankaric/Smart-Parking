## General info
This project is a simple model of a password protected parking system with the limitation of parking space being 3
With the help of IR sensors the OLED display displays remaining parking spots, current temperature, and humidity.
Ultrasound sensors are used for detecting when the model of the vehicle has passed to the sequence of lowering the ramp(rotating the servo motor) can start
The con of using IR sensors is that they require a source of light for proper signaling.
The con of using US sensors is that they have a medium range at best and they're slow when it comes to measuring, while on the other side the pro is that they possess a quality of high sensitivity which in this case enabled the system to measure was it a model of a vehicle that it was detecting or the other ramp

Why did we use US sensors for detecting whether the vehicle has passed or not? First because they have better range of detection than IR sensors, second because US sensors can detect a wider variety of objects, including soft and non-reflective surfaces.

Obiously IR sensors are faster than US sensors, but in the application of parking sensors that is not needed. So we could have used US sensors for both the detection of occupied/unoccupied paring spots and passing of objects but we used them anyway to get a feel of how they work.

Why did we use IR and US sensors? Well because they're cheap.

microSD card reader was utilized as a way to permanently record who occupied a parking spot at which time. Obviously the Flash could have been used too, but the microSD card was kind of more practical when it came to sharing information unless we wanted to connect the microcontroller to a network with another peripheral, but that's left for some time in the future.


It was the final project for course Embedded Systems programming

Microcontroller that we were using is the STM32 F407

## Perfipherals used
Project is created with:
* IR sensor
* 4x4 keypad
* OLED display
* Servo motor
* microSD card reader
* US sensor

## Summary

As part of a team project implemented on an STM32F407 board, I played a key role in integrating, verifying, and fixing implementations of various peripherals developed by my colleagues. This included an OLED display (SPI), microSD card reader, and servo motor. I also took the initiative to develop a library for these peripherals to streamline their usage in the project.

The project aimed to create a private smart parking system that could be used in houses with spacious garages. My responsibilities included ensuring the proper integration of the peripherals with the main system, verifying their functionality, and fixing any issues or bugs identified during testing. I also developed libraries for the OLED display, microSD card reader, and servo motor, which facilitated their easy integration into the project and simplified their usage by other team members.

My contributions to this project included thorough testing, debugging, and documentation of the peripheral implementations and the library I developed. This helped to ensure the overall functionality and reliability of the smart parking system. My attention to detail, problem-solving skills, and expertise in embedded systems played a critical role in the successful implementation of this project.
