#include <arduino-timer.h>

// using digital pins 0,1, 2, 3, 4
#define REL 2
#define FAN 3
#define BUZ 4

#define REL_ON 100
#define REL_OFF 101

#define FAN_ON 200
#define FAN_OFF 201

#define BUZ_ON 300
#define BUZ_OFF 301
#define BUZ_DELAY 1000 // ms

#define ALL_ON 400
#define ALL_OFF 401

auto timer = timer_create_default();
bool isBuzzerOn = false;
String ip;

void setup() {
  Serial.begin(9600);
  
  pinMode(REL,OUTPUT);
  pinMode(FAN,OUTPUT);
  pinMode(BUZ,OUTPUT);
  
  digitalWrite(REL, LOW);
  digitalWrite(FAN, LOW);
  digitalWrite(BUZ, LOW);
}
void loop() {
 
  //check data serial from bluetooth android App
  ip = "";
  while (Serial.available() > 0)
    ip += (char)Serial.read();
    
  if(ip == "" && isBuzzerOn)
    timer.tick<void>(); // void -> dont calc ticks
  //Serial.println(ip);
  
  if(ip != "") 
    switch(ip.toInt()){
      case REL_ON:
        relay("on");
        break;
      case REL_OFF:
        relay("off");
        break;
      case FAN_ON:
        fan("on");
        break;
      case FAN_OFF:
        fan("off");
        break;
      case BUZ_ON:
        keep_buzzer("on");
        break;
      case BUZ_OFF:
        keep_buzzer("off");
        break;
      case ALL_ON:
        relay("on");
        fan("on");
        keep_buzzer("on");
        break;
      case ALL_OFF:
        relay("off");
        fan("off");
        keep_buzzer("off");
        break;
    }
  delay(10);
}

bool buzzer(void *){
  digitalWrite(BUZ, !digitalRead(BUZ));
  return true; // true to keep timer repeating, else false
}

void relay(String w){
  // npn transistor
  if(w == "on")
    digitalWrite(REL, HIGH);
  else if(w == "off")
    digitalWrite(REL, LOW);
}

void fan(String w){
  if(w == "on")
    digitalWrite(FAN, HIGH);
  else if(w == "off")
    digitalWrite(FAN, LOW);
}

void keep_buzzer(String w){
  // pnp transistor
//  Serial.println("Buz: " + w);
  if(w == "on"){
    if(timer.empty())
      timer.every(BUZ_DELAY, buzzer);
//    Serial.println("ON DONE");
    isBuzzerOn = true;
  }
  else if(w == "off" && !timer.empty()){
//    Serial.println("OFF DONE");
    timer.cancel(); // cancel all tasks
    isBuzzerOn = false;
    delay(10);
    digitalWrite(BUZ, LOW);
  }
}
