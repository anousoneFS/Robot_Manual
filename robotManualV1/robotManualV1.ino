#include <Arduino.h>
#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>

int Time = 40;
int n = Time;
int Time2 = 50;
int n2 = Time2;

#define SERVO 9
#define SERVO2 14

// left wheel
#define OUT1 2
#define OUT2 3
#define ENA 5

// right wheel
#define OUT3 4
#define OUT4 7
#define ENB 6

#define ACT_OUT1 22
#define ACT_OUT2 24
#define ACT_ENA 26

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13
#define PS2_CMD        11
#define PS2_SEL        10
#define PS2_CLK        12

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/

#define pressures   false
#define rumble      false

Servo myservo;  // create servo object to control a servo
Servo myservo2;

int pos;// variable to rotate the servo
int add;

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
  
  Serial.begin(57600);

  myservo.attach(SERVO, 500, 2500);
  myservo.write(90);

  myservo2.attach(SERVO2, 500, 2500);
  myservo2.write(90);
  
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(ACT_OUT1, OUTPUT);
  pinMode(ACT_OUT2, OUTPUT);
  pinMode(ACT_ENA, OUTPUT);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  analogWrite(ACT_ENA, 180);
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
//   type = ps2x.readType(); 
//   switch(type) {
//     case 0:
//       Serial.print("Unknown Controller type found ");
//       break;
//     case 1:
//       Serial.print("DualShock Controller found ");
//       break;
//     case 2:
//       Serial.print("GuitarHero Controller found ");
//       break;
//   case 3:
//       Serial.print("Wireless Sony DualShock Controller found ");
//       break;
//    }
  
}

void loop() {

  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return;

  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

// Digital Reading
// ps2x.Button(PSB_BUTTON)

// Anaglog Reading
// ps2x.Analog(PSAB_BUTTON)
// convert analog signal to Decimal number when print, eg. Serial.println(ps2x.Analog(PSAB_BUTTON), DEC);

// Using ButtonPressed, NewButtonState, ButonReleased

//    if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
//      Serial.println("Circle just pressed");
//    if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
//      Serial.println("X just changed");
//    if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released

// Buttons reading

// ps2x.Button(PSB_START)       ps2x.Button(PSB_SELECT)
// ps2x.Button(PSB_PAD_UP)      ps2x.Button(PSB_PAD_LEFT)     ps2x.Button(PSB_PAD_DOWN)     ps2x.Button(PSB_PAD_RIGHT) 
// ps2x.Button(PSB_TRIANGLE)    ps2x.Button(PSB_SQUARE)       ps2x.Button(PSB_CROSS)        ps2x.Button(PSB_CIRCLE)
// ps2x.Button(PSB_L1)          ps2x.Button(PSB_L2)           ps2x.Button(PSB_L3)           ps2x.Button(PSB_R1)           ps2x.Button(PSB_R2)         ps2x.Button(PSB_R3)

// Sticcks reading (Analog only)
// ps2x.Analog(PSS_LY)    Left stick, Y axis
// ps2x.Analog(PSS_LX)    Left stick, X axis
// ps2x.Analog(PSS_RY)    Right stick, Y axis
// ps2x.Analog(PSS_RX)    Right stick, X axis

// to print value, convert to DEC first eg. Serial.println(ps2x.Analog(PSS_LY), DEC);

    // default: stop

    if(ps2x.Button(PSB_START)) {
        Serial.println("START");
    }

    if(ps2x.Button(PSB_SELECT)) {
        Serial.println("SELECT");
    }

    if(ps2x.Button(PSB_PAD_UP)) {
        Serial.println("UP");
        analogWrite(ENA, 150);
        analogWrite(ENB, 150);
        moveForward();
    }else if(ps2x.Button(PSB_PAD_DOWN)) {
        Serial.println("DOWN");
        analogWrite(ENA, 150);
        analogWrite(ENB, 150);
        moveBackward();
    }else if(ps2x.Button(PSB_PAD_LEFT)) {
        Serial.println("LEFT");
        turnLeft();
    }else if(ps2x.Button(PSB_PAD_RIGHT) ) {
        Serial.println("RIGHT");
        turnRight();
    }else{
      moveStop();
    }

//============================

    if(ps2x.Button(PSB_SQUARE)) {
        Serial.println("[]");
    }
    
    if(ps2x.Button(PSB_CIRCLE)) {
        Serial.println("O");
    }

    if(ps2x.Button(PSB_TRIANGLE)) {
        Serial.println("/_\\");
        actuatorForward();
    }else if(ps2x.Button(PSB_CROSS)) {
        Serial.println("X");
        actuatorBackward();
    }else{
        actuatorStop();
    }

//===========================

    Serial.print("n,n2 = ");
    Serial.print(n);
    Serial.print(",");
    Serial.println(n2);
    
    if(ps2x.Button(PSB_L2)) {
        Serial.println("L2");
        if(n != Time){
           servoRelease();
           n++;  
        }else{
          servoStop();
        }
    }else if(ps2x.Button(PSB_L1)) {
        Serial.println("L1");
        if(n != -Time){
          servoCatch();
          n--;
        }else{
          servoStop();
        }   
    }else{
      servoStop();
    }

    if(ps2x.Button(PSB_L3)) {
        Serial.println("L3");
    }

//============================

    if(ps2x.Button(PSB_R1)) {
        Serial.println("R1");
        if(n2 != -Time2){
          servoUp();
          n2--;
        }else{
          servoStop2();
        }   
    }else if(ps2x.Button(PSB_R2)) {
        Serial.println("R2");
        if(n2 != Time2){
           servoDown();
           n2++;  
        }else{
          servoStop2();
        }
    }else{
      servoStop2();
    }

    if(ps2x.Button(PSB_R3)) {
        Serial.println("R3");
    }

  delay(15);

}


void turnLeft() {
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, LOW);

  digitalWrite(OUT3, LOW);
  digitalWrite(OUT4, HIGH);
}

void turnRight() {
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, HIGH);

  digitalWrite(OUT3, HIGH);
  digitalWrite(OUT4, LOW);
}

void moveForward() {
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, HIGH);

  digitalWrite(OUT3, LOW);
  digitalWrite(OUT4, HIGH);
}

void moveBackward() {
  digitalWrite(OUT1, HIGH);
  digitalWrite(OUT2, LOW);

  digitalWrite(OUT3, HIGH);
  digitalWrite(OUT4, LOW);
}

void moveStop() {
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, LOW);
  
  digitalWrite(OUT3, LOW);
  digitalWrite(OUT4, LOW);
}

void actuatorForward() {
  digitalWrite(ACT_OUT1, HIGH);
  digitalWrite(ACT_OUT2, LOW);
}

void actuatorBackward() {
  digitalWrite(ACT_OUT1, LOW);
  digitalWrite(ACT_OUT2, HIGH);
}

void actuatorStop() {
  digitalWrite(ACT_OUT1, LOW);
  digitalWrite(ACT_OUT2, LOW);
}

void servoStop(){
  pos = 1500;
  myservo.writeMicroseconds(pos);
  delay(15);
}

void servoCatch() {
    pos = 2000;
    myservo.writeMicroseconds(pos);              // if pos > 1500 is forward
    delay(15);                                   // waits 15ms for the servo to reach the position
}

void servoRelease() {
    pos = 1000;
    myservo.writeMicroseconds(pos);              // if pos < 1300 is backward 
    delay(15);                                   // waits 15ms for the servo to reach the position
}

//==============================

void servoUp(){
    pos = 1000;
    myservo2.writeMicroseconds(pos);              // if pos < 1300 is backward 
    delay(15);     
}

void servoDown(){
    pos = 2000;
    myservo2.writeMicroseconds(pos);              // if pos > 1500 is forward
    delay(15);   
}

void servoStop2(){
  pos = 1500;
  myservo2.writeMicroseconds(pos);
  delay(15);
}
