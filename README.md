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

