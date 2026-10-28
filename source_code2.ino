#include <Servo.h>
#define THRESHOLD 25

int tl = A0; //top-left
int tr = A1; //top-right
int bl = A2; //bottom-left
int br = A3; //bottom-right

Servo servoH;
Servo servoV;

int posH = 90;
int posV = 90;

void setup()
{
  servoH.attach(9);
  servoV.attach(10);
}

void loop()
{
  //Analog read
  int tl_val = analogRead(tl);
  int tr_val = analogRead(tr);
  int bl_val = analogRead(bl);
  int br_val = analogRead(br);
  
  //Averaging out
  int avgTop = (tl_val + tr_val) / 2;
  int avgBottom = (bl_val + br_val) / 2;
  int avgLeft = (tl_val + bl_val) / 2;
  int avgRight = (tr_val + br_val) / 2;
  
  //Conditional
  if(abs(avgTop - avgBottom) > THRESHOLD)
    (avgTop >avgBottom)?posV--:posV++;
  if(abs(avgLeft - avgRight) > THRESHOLD)
    (avgLeft >avgRight)?posH++:posH--;
  

  posV = constrain(posV, 0, 180);
  posH = constrain(posH, 0, 180);

  servoV.write(posV);
  servoH.write(posH);
  
  delay(15);
}