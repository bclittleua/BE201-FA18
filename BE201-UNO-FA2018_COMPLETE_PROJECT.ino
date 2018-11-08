//INCLs and DEFs
#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
#define STEPS_PER_ROTATION 4096
#include <dht.h>
#define DHT11_PIN 2
dht DHT;

//COMMON VALUES
int ldrThresh = 100; //SET __LDR__ THRESHOLD HERE
int mThreshVal = 800; //SET __SOIL MOISTURE__ THRESHOLD HERE
int hThreshVal = 20; //SET __HUMIDITY__ THRESHOLD HERE
int tThreshVal = 26; //SET |__TEMP__ THRESHOLD HERE
int tempVal = DHT.temperature;
int humVal = DHT.humidity;
int ldrVal = analogRead(A0);
int moistVal = analogRead(A1);
int loopDelay = 30000; //delay of loop(), in miliseconds
int counter = 0;
bool flipmode = false; //false for closed and true for opened

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(IN1, OUTPUT); //pin4 (as defined above)
  pinMode(IN2, OUTPUT); //pin5
  pinMode(IN3, OUTPUT); //pin6
  pinMode(IN4, OUTPUT); //pin7
  pinMode(8, OUTPUT);//connect to IN1 on relay, LED
  pinMode(9, OUTPUT);//connect to IN2 on relay, PUMP
  pinMode(10, OUTPUT);//connect to IN3 on relay, FOGGER
  pinMode(11, OUTPUT);//connect to IN4 on relay
  digitalWrite(8, LOW); //LED
  digitalWrite(9, LOW); //WATER PUMP
  digitalWrite(10, LOW); //FOGGER
  digitalWrite(11, LOW); //unused for this project, but defined anyway
}

void loop(){
  //This will loop over and over again, delay defined above
  //SERIAL MONITOR REPORTING
  counter = counter +1;
  Serial.print("Report#");
  Serial.println(counter);
  Serial.print("Temperature C = ");
  Serial.println(tempVal);
  Serial.print("Humidity %= ");
  Serial.println(humVal);
  Serial.print("Light Value % = ");
  Serial.println(ldrVal);  
  Serial.print("Moisture Value % = ");
  Serial.println(moistVal);
  Serial.print("Flipmode = ");
  Serial.println(flipmode);
//LED FUNCTION
  if (ldrVal <= ldrThresh){
    digitalWrite(8, HIGH);
    Serial.println("LED Triggered");
  }
  else{
    digitalWrite(8, LOW);
    Serial.println("LED Off");
  }
//WATER PUMP FUNCTION
  if(moistVal >= mThreshVal){
    digitalWrite(9, HIGH);
    Serial.println("H20Pump Triggered");
  }
  else{
    digitalWrite(9, LOW);
    Serial.println("H20Pump Off");
  }
//FOGGER FUNCTION
  if (humVal < hThreshVal){
    digitalWrite(10, HIGH);
    Serial.println("Fogger Triggered");
  }
  else{
    digitalWrite(10, LOW);
    Serial.println("Fog Off");
  }
//STEPPER FUNCTION
  if (tempVal >= tThreshVal){
    opentop();
  }
  else{
    closetop();
  }
  Serial.println("");
  delay(loopDelay);     
}

//FLIP LID WITH STEPPER MOTOR BASED ON TEMP
void opentop(){
  if(flipmode == true){
    Serial.println("Tank Open");
    }
  else {
   flipmode = !flipmode;
   rotate(.75);
   Serial.println("Opening Tank");
   } 
}

void closetop(){
  if(flipmode == false){
    Serial.println("Tank Closed");
    }
  else {
    flipmode = !flipmode;
    rotate(-.75);
    Serial.println("Closing Tank");
    }
}

//MAIN STEPPER FUNCTION BELOW 
void rotate(float rotations) {
  rotate_steps(rotations * STEPS_PER_ROTATION);
}

int phase = 0;
byte phases[] = { 1, 3, 2, 6, 4, 12, 8, 9 };

void rotate_steps(int steps)
{
  int dir = (steps > 0) - (steps < 0);
  steps *= dir;
  long laststep;
  for (int i = 0; i < steps;) {
    long now = micros();
    if (now - laststep < 1000) continue;
    laststep = now;
    stepper_writepins(phases[phase]);
    phase = (8 + phase + dir) % 8;
    i++;
  }
  stepper_writepins(0);
}

void stepper_writepins(int bitmap) {
    digitalWrite(IN1, bitmap & 8 ? HIGH : LOW);
    digitalWrite(IN2, bitmap & 4 ? HIGH : LOW);
    digitalWrite(IN3, bitmap & 2 ? HIGH : LOW);
    digitalWrite(IN4, bitmap & 1 ? HIGH : LOW);
}

//RELAY TEST, add to loop() to run (but comment out other functions first!)
void relaytestloop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH); //turn relay IN1 on
  delay(5000); //delay for 3 seconds
  digitalWrite(8, LOW); //turn relay IN1 off
  delay(5000);
  digitalWrite(9, HIGH); //turn relay IN2 on
  delay(5000);
  digitalWrite(9, LOW); //Turn relay IN2 off
  delay(5000);
  digitalWrite(10, HIGH); //turn relay IN2 on
  delay(5000);
  digitalWrite(10, LOW); //Turn relay IN2 off
  delay(5000);
  digitalWrite(11, HIGH); //turn relay IN2 on
  delay(5000);
  digitalWrite(11, LOW); //Turn relay IN2 off
  delay(5000);  
}
