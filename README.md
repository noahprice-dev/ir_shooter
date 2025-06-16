# Arduino Infrared Shooter Game

#### This is a small project that integrates both Bluetooth and an IR Sensor to make a extremely simple game where you can 'fire' at an IR receiver using an IR remote. The result is visible by interfacing via COM, using a tool such as PuTTy.
#### This was built using an Arduino Uno R3 and an HC-05 Bluetooth module. I believe the IR Receiver is a [KY-022 Infrared Receiver](https://arduinomodules.info/ky-022-infrared-receiver-module/), the controller I used was a simple remote control from Elegoo that came with my Starter Kit.
#### When firing, assuming a successful hit (read from IR receiver) your ammo will decrease by 1, and your score will increase by 1. You start with 5 ammo, but can reload as frequently as you wish. The goal of the project was to combine a sensor with the Bluetooth Serial transceiver and communicate wirelessly back to my PC using Bluetooth.

## Circuit

The following circuit demonstrates the approximate setup required. This is a very straightforward circuit, mostly we are connecting 5v to 5v and ground to ground. However, the HC-05 Bluetooth module **must** take 3.3v on it's Rx pin. The Arduino Uno R3 Digital output by default sends 5v. To avoid frying your module, we must step this voltage down using a voltage divider. To achieve this, I have used a 1kΩ and 2kΩ resistor, providing to the Rx pin from the center. Be careful when switching pins in the main script, and be sure that the Arduino Tx pin is going through a voltage divider before it reaches the BT Rx pin.
![Arduino_IR_BT](https://github.com/user-attachments/assets/ba0cf552-0bb9-45f9-a38e-389b6e439914)

## Code
Some defaults you may be interested in changing:
- `RxPin`: The Arduino's receive pin. Digital Pin 9 by default.
- `TxPin`: The Arduino's transmit pin. Digital Pin 8 by default.
- `IRRcvPin`: Data pin from our IR Receiver. Digital Pin 7 by default.
- `IRCommands`: Enum that lists the hex codes received from the remote. This may be different depending on your IR Remote, or if you want to customize the buttons.
- `ammo`: Default 5.
