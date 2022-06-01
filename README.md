# Electronic_Binary_Calculator
Calculator using infrared sensors as buttons, currently used as a binary calculator, using an STM32 Nucleo64 microcontroller.
Designed a PCB in Altium Designer and Altium CircuitMaker to the microcontroller. It contains a 16x2 LCD and 16 infrared sensors (and a supply circuit, with  reverse current protection). The infrared sensors are used as buttons, as an object gets close to them, their  output voltage changes, making them applicable as buttons. All 16 sensors are connected to 8 analog inputs of the Nucleo board in pairs, their  values are read simultaneously, one per pair at a time (pairs are multiplexed with transistors). 

The board:

I also designed a  firmware in C, using the Eclipse-based STM32CubeIDE.
which using the microcontroller, detects which buttons  are pressed, writes on the LCD, based on a state machine I modelled. 


![diag](https://user-images.githubusercontent.com/82604073/171515569-0da1a519-1b1e-4274-a282-5564f1be1bc6.png)
