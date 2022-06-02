# Electronic_Binary_Calculator
Calculator using infrared sensors as buttons, currently used as a binary calculator, using an STM32 Nucleo64 microcontroller.
Designed a PCB in Altium Designer and Altium CircuitMaker to the microcontroller. It contains a 16x2 LCD and 16 infrared sensors (and a supply circuit, with  reverse current protection). The infrared sensors are used as buttons, as an object gets close to them, their  output voltage changes, making them applicable as buttons. All 16 sensors are connected to 8 analog inputs of the Nucleo board in pairs, their  values are read simultaneously, one per pair at a time (pairs are multiplexed with transistors). 

The board:

![IMG_20220602_010716](https://user-images.githubusercontent.com/82604073/171653381-bea7c9e6-313c-424d-a305-5c3f1c364a1b.jpg)



The schematic:

![Kép1](https://user-images.githubusercontent.com/82604073/171651510-337e83d1-6683-4950-b547-f7fdc2185fff.png)



I also designed a  firmware in C, using the Eclipse-based STM32CubeIDE.

To use all 16 buttons, I have to desolder two solder bridges on the microcontroller. Instead, I only use 8 buttons, and made a binary calculator: it uses only 0 and 1, but displays the value in base 10.

The firmware, using the analog inputs of the microcontroller, detects which sensors are pressed, and based on a state machine I modelled, decides what to write on the LCD and sends this data via UART to the C# client app (see below). The state machine:

![diag](https://user-images.githubusercontent.com/82604073/171515569-0da1a519-1b1e-4274-a282-5564f1be1bc6.png)

The LCD's anode pin isn't working correctly, so I made a client application to aid that problem, displaying the calculator's display (and it's buttons). The client app:

![kep2](https://user-images.githubusercontent.com/82604073/171652173-1e579eb3-8269-4177-9914-b80b25ffff78.png)

This way I could take the LCD off the board.
