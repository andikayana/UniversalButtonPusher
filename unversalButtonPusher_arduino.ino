#include <Servo.h> 

#define SHOULDER_SERVO 0
#define ELBOW_SERVO 1
#define FINGER_SERVO 2

#define FINGER_UP_POS 1600

#define SHOULDER_HOME_POS 950
#define ELBOW_HOME_POS 1000

#define SHOULDER_PIN 9
#define ELBOW_PIN 10
#define FINGER_PIN 11
#define SWITCH_PIN A0


int buttonPositions[] = {
  1380, 1500, 1420,
  1380, 1200, 1400,
  1620, 1550, 1380,
  1570, 1300, 1420,
  1800, 1430, 1400,
  1750, 1130, 1400
};

// buffer sizes
#define maxTraceF 128

char trcBuf[maxTraceF];

// debug output
void tracef(const char *fmt, ... )
{
  va_list args;
  va_start (args, fmt );
  vsnprintf(trcBuf, maxTraceF, fmt, args);
  va_end (args);
  trace(trcBuf);
}

// debug output
void trace(const char *str)
{
  Serial.print(str);
}
 
Servo servo[3];

//
void lerpServo(int servoID, int startPos, int endPos, int duration)
{
  servo[servoID].writeMicroseconds(startPos);
  delay(100); //just in case
  
  int startTime = millis();
  int endTime = startTime + duration;
  
  while (1)
  {
    int elapsed = millis() - startTime;
    if (elapsed >= duration)
    {
      servo[servoID].writeMicroseconds(endPos);
      break;
    }

    int pos = map(elapsed, 0, duration, startPos, endPos);
    servo[servoID].writeMicroseconds(pos);
  }
}

//
int pressedBtn = -1;
void pressButton(int btn)
{
  //finger up
  servo[FINGER_SERVO].writeMicroseconds(FINGER_UP_POS);
  delay(500);
  
  int shoulderPos = buttonPositions[btn * 3 + 0];
  int elbowPos = buttonPositions[btn * 3 + 1];
  int fingerPos = buttonPositions[btn * 3 + 2];
  
  //move over button
  servo[SHOULDER_SERVO].writeMicroseconds(shoulderPos);
  servo[ELBOW_SERVO].writeMicroseconds(elbowPos);  
  delay(1000);
  
  //finger down
  lerpServo(FINGER_SERVO, FINGER_UP_POS, fingerPos, 1000);

  pressedBtn = btn;
}

//
void releaseButton()
{  
  int fingerPos = buttonPositions[pressedBtn * 3 + 2];

  //finger up
  lerpServo(FINGER_SERVO, fingerPos, FINGER_UP_POS, 1000);
}

//
void testPress(int endPos)
{
  //finger up
  servo[FINGER_SERVO].writeMicroseconds(FINGER_UP_POS);
  delay(500);
  
  //finger down
  lerpServo(FINGER_SERVO, FINGER_UP_POS, endPos, 1000);
  delay(500);

  //finger up
  lerpServo(FINGER_SERVO, endPos, FINGER_UP_POS, 1000);
} 

#define MAX_CMD_LEN 64
char cmd[MAX_CMD_LEN];
int cmdIdx = 0;

//
void processCmd()
{
  tracef("got cmd: %s\r\n", cmd);
  
  char *typeStr, *idStr, *posStr, *timeStr;
  
  typeStr = strtok(cmd, " ");
  
  if (typeStr[0] == 'b')
  {
    idStr = strtok(NULL, " ");
    timeStr = strtok(NULL, "\n");
    
    int idVal = atoi(idStr);
    int time = atoi(timeStr);

    pressButton(idVal);
    delay(time);
    releaseButton();
  }
  else if (typeStr[0] == 'p')
  {
    posStr = strtok(NULL, "\n");
    int posVal = atoi(posStr);
    testPress(posVal);
  }
  else if (typeStr[0] == 's')
  {
    idStr = strtok(NULL, " ");
    posStr = strtok(NULL, "\n");
    
    int idVal = atoi(idStr);
    int posVal = atoi(posStr);
  
    tracef("got servo:%d pos:%d\r\n", idVal, posVal);
  
    servo[idVal].writeMicroseconds(posVal);
  }
}

// 
void setup() 
{
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  servo[SHOULDER_SERVO].attach(SHOULDER_PIN);
  servo[ELBOW_SERVO].attach(ELBOW_PIN);
  servo[FINGER_SERVO].attach(FINGER_PIN);
  
  servo[FINGER_SERVO].writeMicroseconds(FINGER_UP_POS);
  delay(500);
  
  servo[SHOULDER_SERVO].writeMicroseconds(SHOULDER_HOME_POS);
  servo[ELBOW_SERVO].writeMicroseconds(ELBOW_HOME_POS);  
  delay(500);
    
  Serial.begin(115200);
  Serial.println("ready");

/*  
  for (int i = 0; i < 6; i++)
  {
    pressButton(i);
    delay(500);
    releaseButton();
  }
*/

  //main loop
  int fingerState = 0;
  int nextButton = 0;
  while (1)
  {
    int switchState = digitalRead(SWITCH_PIN);
    
    if (fingerState == 0 and switchState == LOW)
    {
      pressButton(nextButton);
      fingerState = 1;
    }
    else if (fingerState == 1 and switchState == HIGH)
    {
      releaseButton();
      fingerState = 0;
      nextButton = (nextButton + 1) % 2;
    }
    
    if (Serial.available())
    {    
      char c = Serial.read();
      cmd[cmdIdx] = c;
      if (cmdIdx < MAX_CMD_LEN)
        cmdIdx++;
        
      if (c == '\n')
      {
        processCmd();
        cmdIdx = 0;
      }
    }
  }
} 

//
void loop() {}

