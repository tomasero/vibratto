const int motorCount = 4;
int motors[motorCount] = {9, 6, 5, 3};
//comment

void setup() {
  Serial.begin(115200); //opens serial comm at specified baudrate 
  initMotors();
}

void loop() {
  //on(motors[0],150,2000); //on
  //brake(motors[0], 1000);
  //saw(motors[0], 17, 255, 20000, 1); //ramp up
  //saw(motors[0], 255, 17, 20000, 1); //ramp down
  //square(motors[0], 255, 800, 400, 10); //square wave
  triangle(motors[0],17,255,10000,1); //
  brake(motors[0],1000);
}

void initMotors() {
  for(int i = 0; i < motorCount; i++) {
    pinMode(motors[i],OUTPUT); //sets PWM to OUTPUT
    digitalWrite(motors[i],LOW); //initializes PWM pins to LOW
  }
}

//turns motor on at specified amplitude for specified time
void on(int motor, int amp, int time) {
    analogWrite(motor, amp);
    Serial.println("ON");
    delay(time);
}

//stops motor for specified amount of time
void brake(int motor, int time) {
  Serial.println("brake");
  digitalWrite(motor,LOW);
  delay(time);
}

//provides enough current to kickstart the motor
//power is an int between 1-3. 1 is a gentle kickstart, 3 is a powerful kickstart
void kickstart(int motor, int power) {
  analogWrite(motor,255);
  switch(power) {
    case 3: 
      delay(30);
      break;
    case 2: 
      delay(20);
      break;
    case 1:
      delay(10);
      break;
  }
  Serial.println("kickstart");
}

//square wave, control over amplitude, time ON, time OFF, and number of iterations
//hiTime has to be at least 100ms (lag time is 15.5ms, rise time is 85.5ms)
//loTime has to be at least 200ms for a complete stop (passive brake time is 200ms)
void square(int motor, int amp, int hiTime, int loTime, int iter) {
  for(int i = 0; i < iter; i++) {
    analogWrite(motor, amp);
    Serial.println("HIGH");
    delay(hiTime);
    digitalWrite(motor, LOW);
    delay(loTime);
    Serial.println("LOW");
  }
}

//saw wave, contol over direction, time, and number of iterations
void saw(int motor, int amp0, int amp, int time, int iter) {
  if (amp > amp0) {
    kickstart(motor,3); //during ramp up, we need an initial boost of current
    if (amp0 < 17) { //duty cycle 17/255 is the minimum freq. needed to power motors
      amp0 = 17; //~0.22V
    }
    for(int i = 0; i < iter; i++) {
      for(int j = amp0; j <= amp; j++) {
       analogWrite(motor,j);
       delay(floor(time/(amp-amp0)));
       Serial.println(j);
      }
    }
  } else {
    for(int i = 0; i < iter; i++) {
      for(int j = amp0; j >= amp; j--) {
       analogWrite(motor,j);
       delay(floor(time/(amp0-amp)));
       Serial.println(j);
      }
    }
  } 
}

//triangle wave, takes in low and high bounds for amplitude, time, and # of iterations
void triangle(int motor, int ampLo, int ampHi, int time, int iter) {
  for(int i = 0; i < iter; i++) {
    saw(motor,ampLo,ampHi,time/2,1);
    saw(motor,ampHi,ampLo,time/2,1); 
  }
}





