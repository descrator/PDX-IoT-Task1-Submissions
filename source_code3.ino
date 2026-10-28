#include <string.h>

// Define the key-value structure
typedef struct {
    char key;
    const char* value;
} MorseCode;

// Create the dictionary mapping based on the provided chart
MorseCode morseDictionary[] = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},  {'D', "-.."}, 
    {'E', "."},     {'F', "..-."},  {'G', "--."},   {'H', "...."}, 
    {'I', ".."},    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."}, 
    {'M', "--"},    {'N', "-."},    {'O', "---"},   {'P', ".--."}, 
    {'Q', "--.-"},  {'R', ".-."},   {'S', "..."},   {'T', "-"}, 
    {'U', "..-"},   {'V', "...-"},  {'W', ".--"},   {'X', "-..-"}, 
    {'Y', "-.--"},  {'Z', "--.."},  
    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, 
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."}, 
    {'8', "---.."}, {'9', "----."} 
};

const int irPin = 2;
const int buzzPin = 3;
const int ledPin = 4;
int lastIrState = LOW;
unsigned long startTime = 0;
unsigned long endTime = 0;
char language[6];
short int count = 0;

void setup()
{
  pinMode(irPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void playMorse(const char* pattern) {
  for (int i = 0; i < strlen(pattern); i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzPin, HIGH);
    
    if (pattern[i] == '.') {
      delay(200); // Short flash/beep for dot
    } else if (pattern[i] == '-') {
      delay(600); // Long flash/beep for dash
    }
    
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzPin, LOW);
    delay(200); // Gap between individual signals
  }
}

void loop()
{
  int irState = digitalRead(irPin);
  unsigned long currentTime = millis();
  if(irState == HIGH && lastIrState == LOW) { 
    startTime = currentTime;
  }
  else if(irState == LOW && lastIrState == HIGH) {
    endTime = currentTime;
    unsigned long duration = endTime - startTime;
    if(duration > 50 && duration < 500) {
      //Serial.print(".");
      language[count++] = '.';
    } else{
      //Serial.print("_"); 
      language[count++] = '-';
    }
  }
  else if(irState == LOW && lastIrState == LOW) {
    if(currentTime - startTime > 3000 && endTime != 0) {
      //Serial.print(" ");
      language[count] = '\0';
      count = 0;
      endTime = 0;
      bool printed = false;
      
      //Letter identification logic:
      for(int i=0; i<36; i++){
        if(strcmp(language, morseDictionary[i].value) == 0){
          Serial.print(morseDictionary[i].key);
          playMorse(morseDictionary[i].value);
          printed = true;
          break;
        }
      }
      if(!printed) Serial.print("?");
    }
  }
  lastIrState = irState;
}