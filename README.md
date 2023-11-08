# Electronic_Binary_Calculator
Calculator using infrared sensors as buttons, currently used as a binary calculator, using an STM32 Nucleo64 microcontroller.
Designed a PCB in Altium Designer and Altium CircuitMaker to the microcontroller. It contains a 16x2 LCD and 16 infrared sensors (and a supply circuit, with  reverse current protection). The infrared sensors are used as buttons, as an object gets close to them, their  output voltage changes, making them applicable as buttons. All 16 sensors are connected to 8 analog inputs of the Nucleo board in pairs, their  values are read simultaneously, one per pair at a time (pairs are multiplexed with transistors). 

The board and the LCD:

![IMG_20220602_010716](https://user-images.githubusercontent.com/82604073/171653381-bea7c9e6-313c-424d-a305-5c3f1c364a1b.jpg)



The schematic:

![Kép1](https://user-images.githubusercontent.com/82604073/171651510-337e83d1-6683-4950-b547-f7fdc2185fff.png)



I also designed a  firmware in C, using the Eclipse-based STM32CubeIDE.

To use all 16 buttons, I have to desolder two solder bridges on the microcontroller. Instead, I only use 8 buttons, and made a binary calculator: it uses only 0 and 1, but displays the value in base 10.

The firmware, using the analog inputs of the microcontroller, detects which sensors are pressed, and based on a state machine I modelled, decides what to write on the LCD and sends this data via UART to the C# client app (see below). The state machine:

![diag](https://user-images.githubusercontent.com/82604073/171515569-0da1a519-1b1e-4274-a282-5564f1be1bc6.png)

The LCD's anode pin isn't working correctly, I probably broke the LCD when putting it wrongly into the board one time. So I made a client application to aid that problem, displaying the calculator's display (and it's buttons). The client app:

![kep2](https://user-images.githubusercontent.com/82604073/171652173-1e579eb3-8269-4177-9914-b80b25ffff78.png)

This way I could take the LCD off the board. (The two cables were soldered there to see if it would work this way, but it did not fix it, those cables were desoldered later)

![bo](https://user-images.githubusercontent.com/82604073/171654441-8fc4320f-d7cb-4a22-8cf3-09e6cece9e68.jpg)

Initially, the project started off as a decimal calculator. However, two solder bridges were needed to be soldered differently than default, causing every sensor "on the sides" not be able to function due to disconnected pins. I figured this out by that time we were only allowed to work on the software, not the hardware anymore, thus I couldn't change it, I had to use only the 8 "inbetween" sensors, creating a binary calculator: works great, but is not very nice. (The client app showcased the numbers and results in decimal, just the input was 1's and 0's, converted immediately).


## About the firmware
Can be found under /Firmware_C/Core/Src/.

## About the hardware
The Altium designs of the PCB can be found in the respective subfolder.

The board gets 5 Volts of power supply, but most pins and the sensors use 3.3V, so I used a Buck converter to get stable 3.3V.
I used CNY70 diodes for infrared light detection. For less power usage, I designed the diodes to drain current when they are touched, not in passive (untouched) state. From the below diagram (upper one), I've found that with 1.1V input voltage results in 10mA forward current, to assert to this, I put a serial resistor in there with 220Ohm resistance (required resistance had to be calculated: 3.3V-1.1V voltage needs to drop on it, and the current equals 10mA, hence R = 2.2V/10mA = 220Ohm). The sensor has a phototransistor, based on the lower diagram, I put a 22kOhm resistor in series with it.

![image](https://user-images.githubusercontent.com/82604073/236656536-a4673fc2-9eca-4f08-8029-78050d2a2d00.png)

## About the client app
UART communication with the microcontroller.

Files can be found under /ClientApp_CSharp/calc/ , if using Visual Studio, the solution file (calc.sln) can be ran to load the project, else all files are found under /ClientApp_CSharp/calc/calc/.
