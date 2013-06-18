Arduino Touchscreen Pattern Unlock
==================================
### Purpose
Arduino touchscreen pattern unlock for doors and things

![PatternUnlock](http://i.imgur.com/0XXcLWH.jpg)

### Functionality
The touchscreen shield communicates with the Arduino to let it know when the user has successfully unlocked the pattern.
Upon unlock, the user is greeted with a Welcome screen, and the arduino can be told to activate a lock or other actuator.

### Anatomy of the Repo
All the source code for the Arduino (UNO) is in PatternUnlock.ino. In order to compile that code, the TFT and TouchScreen libraries must  be installed in the Arduino IDE. For more information on installing libraries refer to this link: http://arduino.cc/en/Guide/Libraries

