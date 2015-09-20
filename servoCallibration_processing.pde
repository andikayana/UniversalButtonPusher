import controlP5.*;
import processing.serial.*; 

ControlP5 cp5;
Serial arduino; 

final int SERVO_CENTER = 1500;

final int SERVO_MIN = 1000;
final int SERVO_MAX = 2000;

final int SLIDER_WIDTH = 20;
final int SLIDER_HEIGHT = 100;

int servo0 = SERVO_CENTER;
int servo1 = SERVO_CENTER;
int servo2 = SERVO_CENTER;

void setup() {
  size(700,400);
  noStroke();

  arduino = new Serial(this, "/dev/tty.usbmodem1421", 115200);  
  
  cp5 = new ControlP5(this);

  cp5.addSlider("servo0")
     .setPosition(100, 140)
     .setSize(SLIDER_WIDTH, SLIDER_HEIGHT)
     .setRange(SERVO_MIN, SERVO_MAX)
     ;

  cp5.addSlider("servo1")
     .setPosition(200, 140)
     .setSize(SLIDER_WIDTH, SLIDER_HEIGHT)
     .setRange(SERVO_MIN, SERVO_MAX)
     ;

  cp5.addSlider("servo2")
     .setPosition(300, 140)
     .setSize(SLIDER_WIDTH, SLIDER_HEIGHT)
     .setRange(SERVO_MIN, SERVO_MAX)
     ;
     
}

void draw() {
  background(0);
}

void servo0(float val) {
  println("servo 0: " + val);
  arduino.write("s 0 " + (int)val + "\n");
}

void servo1(float val) {
  println("servo 1: " + val);
  arduino.write("s 1 " + (int)val + "\n");
}

void servo2(float val) {
  println("servo 2: " + val);
  arduino.write("s 2 " + (int)val + "\n");
}
