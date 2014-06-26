//THREE MOTOR DRIVER (BIDIRECTIONAL)

//LOGIC TABLE
//  A    B   EFFECT
// LOW  LOW   SLOW
// HIGH LOW   CW
// LOW  HIGH  CCW
// HIGH HIGH  !SHORT CIRCUIT! DON'T LET THIS HAPPEN EVER 

//define PWM pins -- these are the 6 PWM pins on the arduino uno
int pinA1 = 10; 
int pinB1 = 11;
int pinA2 = 6;
int pinB2 = 9;
int pinA3 = 3;
int pinB3 = 5;
int motors[][2] = { //M1, CW AKA 0 AND 1
{pinA1, pinB1},
{pinA2, pinB2},
{pinA3, pinB3}
};
//DIR
int CW = 1;
int ACW = 0;
//MOTORS
int M1 = 0;
int M2 = 1;
int M3 = 2;

void setup() {
  Serial.begin(9600);
  //define all PWM pins as OUTPUT
  pinMode(pinB1,OUTPUT); 
  pinMode(pinA1,OUTPUT);
  pinMode(pinB2,OUTPUT);
  pinMode(pinA2,OUTPUT);
  pinMode(pinB3,OUTPUT);
  pinMode(pinA3,OUTPUT);
  //set all PWM pins to LOW
  for(int i = 0; i < 3; i++) {
    slow(i, 1);  
  }
}

void loop() {    
  
  ramp(M1, 2000, CW, 255, 100);
  ramp(M1, 2000, CW, 100, 255);

  //ramp(M1, 2000, CW, 0, 255);
  //delay(3000);
//  slow(M1, 1000);
}

void slow(int motor, int time) {
  digitalWrite(motors[motor][0], LOW); 
  digitalWrite(motors[motor][1], LOW);
  delay(time);
}



void ramp(int motor, int time, int dir, int startSpeed, int finalSpeed) {
  Serial.println(startSpeed);//255
  Serial.println(finalSpeed);//200
  startSpeed = validateSpeed(startSpeed);//255
  finalSpeed = validateSpeed(finalSpeed);//200
  Serial.println(startSpeed);//
  Serial.println(finalSpeed);//
  if (time < 255) { //time is 2000
    time = 255;  
  } //time is still 2000
  int ramp = getRampType(startSpeed, finalSpeed); //input 255, 200, RAMP is 1
  //great shit works ^^^^^^
  
  if (ramp == 0) { //NOPE
    //Serial.println("UP");
    for (int i = startSpeed; i <= finalSpeed; i++) {
      analogWrite(motors[motor][dir], i);
      delay(floor(time/255)); //2000/255 floor dat shit
    }
  } else { //YEH THIS ONE
    //Serial.println("DOWN");
    for (int i = startSpeed; i >= finalSpeed; i--) { //from 255 to 200, i--
      analogWrite(motors[motor][dir], i); //M1, CW, i ----- ON PIN B1 WRITE PWM "i"
      delay(floor(time/255));
    }
  }
}

int validateSpeed(int val) {
  if (val < 0) {
    return 0;
  } else if (val > 255) {
    return 255; 
  } else {
    return val;  
  }
}

int getRampType(int startSpeed, int finalSpeed) { //we input 255, 200
  if (startSpeed < finalSpeed) { //255 < 200 NOPE
    Serial.println("0");
    return 0;  
  } else { //YEH THIS ONE
    Serial.println("1");
    return 1;  
  }
}





