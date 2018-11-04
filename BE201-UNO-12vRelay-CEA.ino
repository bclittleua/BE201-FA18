#include <dht.h>
dht DHT;
#define DHT11_PIN 2
int counter = 0;
bool flipmode = false; //false for closed and true for opened

#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
#define STEPS_PER_ROTATION 4096

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(8, OUTPUT);//connect to IN1 on relay
  pinMode(9, OUTPUT);//connect to IN2 on relay
  pinMode(10, OUTPUT);//connect to IN2 on relay
  pinMode(11, OUTPUT);//connect to IN2 on relay
  digitalWrite(8, LOW); //LED
  digitalWrite(9, LOW); //WATER PUMP
  digitalWrite(10, LOW); //FOGGER
  digitalWrite(11, LOW); //STEPPER??
}


void loop(){
  //COMMON VALUES
  int ldrValue = analogRead(A0);
  int ldrStatus = analogRead(A0);
  int mthresholdValue = 800;
  int moistValue = analogRead(A1);
  int chk = DHT.read11(DHT11_PIN);

  //SERIAL MONITOR REPORTING
  counter = counter +1;
  Serial.print("Report#");
  Serial.println(counter);
  Serial.print("Temperature C = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity %= ");
  Serial.println(DHT.humidity);
  Serial.print("Light Value % = ");
  Serial.println(ldrValue);  
  Serial.print("Moisture Value % = ");
  Serial.println(moistValue);
  Serial.print("Flipmode = ");
  Serial.println(flipmode);
  Serial.println("");
  //delay(5000);
  
//LED FUNCTION
  if (ldrStatus <= 100){
    digitalWrite(8, HIGH);
    Serial.println("LED Triggered");
  }
  else{
    digitalWrite(8, LOW);
  }

//WATER PUMP FUNCTION
  if(moistValue < mthresholdValue){
    digitalWrite(9, HIGH);
    Serial.println("H20Pump Triggered");
  }
  else{
    digitalWrite(9, LOW);
  }

//FOGGER FUNCTION
  if (DHT.humidity < 20){
    digitalWrite(10, HIGH);
    Serial.println("Fogger Triggered");
  }
  else{
    digitalWrite(10, LOW);
  }
  
//STEPPER FUNCTION
  if (DHT.temperature < 25){
    opentop();
  }
  else{
    closetop();
  }  
  
  delay(30000);     
}

//FLIP LID WITH STEPPER MOTOR BASED ON TEMP
void opentop(){
  if(flipmode == true){
    Serial.println("open");
    }
  else {
   flipmode = !flipmode;
   rotate(1);
   Serial.println("opening");
   }
}


void closetop(){
  if(flipmode == false){
    Serial.println("closed");
    }
  else {
    flipmode = !flipmode;
    rotate(-1);
    Serial.println("closing");
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
