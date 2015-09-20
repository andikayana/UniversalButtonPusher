# UniversalButtonPusher

This project is a button pusher that help push buttons on a remote control or anything else whenever the user is not able to do so themselves. This button pusher itself can be controlled through an accessibility switch, eye-gaze, voice recognition, etc. Any method that is convenient.

The .ino file runs on an Arduino and controls the servos.
The .pde file runs in Processing on your computer as a means to more easily calibrate the servo positions. Your computer and Processing are not used at all and are unnecessary in the normal usage of the button pusher.

You can also send commands to the arduino over it's USB port to control the servos or calibrate them.
Command set:
s 0 1500 //set servo 0 to position 1500
b 2 1000 //press button 2 for 1000 milliseconds
p 1400 //momentarily press 'finger' servo to position 1400, other servos hold current position 

More info at: http://bobparadiso.com/2015/08/03/universal-button-pusher/
