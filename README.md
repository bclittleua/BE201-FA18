# BE201-FA18

This .ino utilizes an Arduino Uno and a 4chan 12v relay.

It uses these sensors and components:
- dht11 temp/humidity sensor
- standard LDR photoresistor, 20k i think
- YL-69 moisture sensor
- 10k resistor for the LDR
- 5v 4-channel relay

And controls these devices:
- 28BYJ48 Stepper Motor+ Sheild
- HG-16 small fountain pump
- terrarium fogger (will add specifics later)
- 12v array of LEDs


Wire as indicated in the .ino, but for the tl;dr:
- stepper to pwm 4-7
- relay to pwm 8-11
- dht to pwm 2
- LDR to analog 0
- moisture sensor to analog 1

Splice positive/hot wire of device through the relay, use normally open (NO) and COMmon so appliances will run if sensors/arduino go offline.
