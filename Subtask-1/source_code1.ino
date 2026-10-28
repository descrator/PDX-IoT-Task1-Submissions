#include <Servo.h>
#define VEL 0.025 
//cm per ms
#define THRESHOLD 300
//in cm


const int servoPin = 4;
const int ussPin = 3;
int prevState;
int ussState;
unsigned long long duration;
unsigned long long startTime;
Servo myServo;
int short_cnt = 0;
int med_cnt = 0;
int long_cnt = 0;

void setup()
{
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(ussPin, INPUT);
  prevState = LOW;
  duration = 0;
}

int durationToBinary(unsigned long long duration){
  int distance = 0.03430*duration/2;
  if(distance <= THRESHOLD) return HIGH;
  else return LOW;
}

void loop()
{
  //Getting input from USS:
  //Give a short LOW Pulse beforehand to ensure a CLEAN high pulse:
  pinMode(ussPin, OUTPUT);
  digitalWrite(ussPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ussPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ussPin, LOW);
  
  //Input:
  pinMode(ussPin, INPUT);
  duration = pulseIn(ussPin, HIGH);
  ussState = durationToBinary(duration);
 
  /* Definitions-
    Short object - 0 to 10 cm
    Medium object - 10 to 20 cm
    Long object 20+ cm
    
    Servo Directions-
    60 for Short
    90 for Medium
    120 for Long
  */
  
  //Conditionals
  unsigned long long currentTime = millis();
  if(prevState == LOW && ussState == HIGH) startTime = currentTime;
  else if(prevState == HIGH && ussState == LOW){
    long long int timeDiff = currentTime - startTime;
    int len = timeDiff*VEL;
    if(len<10){
    	short_cnt++;
        myServo.write(60);
    } else if(len >= 10 && len<20){
      med_cnt++;
      myServo.write(90);
    } else if(len>20){
      long_cnt++;
      myServo.write(120);
    }
    Serial.print("\n\n");
    Serial.print("Short - ");
    Serial.print(short_cnt);
    Serial.print("\nMedium - ");
    Serial.print(med_cnt);
    Serial.print("\nLong - ");
    Serial.print(long_cnt);
    Serial.print("\n");
  }
  
  prevState = ussState;
}
