#include <AFMotor.h>
#include <Servo.h>
#include <NewPing.h>

#define TRIG_PIN A4 // Pin A4 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A5 // Pin A5 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 200 // sets maximum useable sensor measuring distance to 200cm
#define MAX_SPEED 180 // sets speed of DC traction motors to 180/256 or about 70% of full speed - to get power drain down.
#define MAX_SPEED_OFFSET 10 // this sets offset to allow for differences between the two DC traction motors
#define COLL_DIST 10 // sets distance at which robot stops and reverses to 10cm
#define TURN_DIST COLL_DIST+10 // sets distance at which robot veers away from object (not reverse) to 20cm (10+10)
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.

AF_DCMotor motor1(1, MOTOR12_1KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor motor2(4, MOTOR12_1KHZ); // create motor #2, using M2 output, set to 1kHz PWM frequency
int pos = 0; // this sets up variables for use in the sketch (code)
int maxDist = 0;
int maxAngle = 0;
int maxRight = 0;
int maxLeft = 0;
int maxFront = 0;
int course = 0;
int curDist = 0;
String motorSet = "";
int speedSet = 0;

//readPing
//-------------------------------------------------------------------------------------------------------------------------------------
int readPing() { // read the ultrasonic sensor distance
  delay(70);
  unsigned int uS = sonar.ping();
  int cm = uS / US_ROUNDTRIP_CM;
  return cm;
}


void moveForward() {
  motorSet = "FORWARD";
  motor1.run(FORWARD);      // turn it on going forward
  motor2.run(FORWARD);      // turn it on going forward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet + MAX_SPEED_OFFSET);
    motor2.setSpeed(speedSet);
    delay(5);
  }
}
void checkForward()

{
  if (motorSet == "FORWARD") {
    motor1.run(FORWARD);  // make sure motors are going forward
    motor2.run(FORWARD);
  }
}

//checkCourse
//-------------------------------------------------------------------------------------------------------------------------------------
void checkCourse() { // we're about to hit something so move backwards, stop, find where the empty path is.
  //-------------------------------------------------------------------------------------------------------------------------------------
}

void moveBackward() {
  motorSet = "BACKWARD";
  motor1.run(BACKWARD);      // turn it on going forward
  motor2.run(BACKWARD);     // turn it on going forward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet + MAX_SPEED_OFFSET);
    motor2.setSpeed(speedSet);
    delay(5);
  }

}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  motor1.run(FORWARD);      // turn motor 1 forward
  motor2.run(BACKWARD);     // turn motor 2 backward
  delay(400); // run motors this way for 400ms
  motorSet = "FORWARD";
  motor1.run(FORWARD);      // set both motors back to forward
  motor2.run(FORWARD);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "LEFT";
  motor1.run(BACKWARD);     // turn motor 1 backward
  motor2.run(FORWARD);      // turn motor 2 forward
  delay(400); // run motors this way for 400ms
  motorSet = "FORWARD";
  motor1.run(FORWARD);      // turn it on going forward
  motor2.run(FORWARD);      // turn it on going forward
}

//-------------------------------------------------------------------------------------------------------------------------------------
void setCourse() { // set direction for travel based on a very basic distance map, simply which direction has the greatest distance to and object - turning right or left?

  if (maxAngle < 90) {
    turnRight();
  }
  if (maxAngle > 90) {
    turnLeft();
  }
  maxLeft = 0;
  maxRight = 0;
  maxFront = 0;
}
void moveStop()
{ motor1.run(RELEASE); motor2.run(RELEASE); // stop the motors.
  moveBackward(); {
    delay(500);
    moveStop();
    setCourse();
  }

}
//-------------------------------------------------------------------------------------------------------------------------------------
void veerLeft() {
  motor1.run(BACKWARD);  // veering left? set left motor backwards for 400ms
  delay(400);
  motor1.run(FORWARD);
}
//-------------------------------------------------------------------------------------------------------------------------------------

void veerRight() {
  motor2.run(BACKWARD);  // veering right? set right motor backwards for 400ms
  delay(400);
  motor2.run(FORWARD);
}

//-------------------------------------------------------------------------------------------------------------------------------------
void changePath() {
  if (pos < 90) {
    veerLeft(); // if current pos of sensor is less than 90-degrees, it means the object is on the right hand side so veer left
  }
  if (pos > 90) {
    veerRight(); // if current pos of sensor is greater than 90-degrees, it means the object is on the left hand side so veer right
  }
}

void checkPath() {
  int curLeft = 0;
  int curFront = 0;
  int curRight = 0;
  int curDist = 0;

  {
    // wait 90ms for servo to get to position
    checkForward();                                  // check the robot is still moving forward
    curDist = readPing();                          // get the current distance to any object in front of sensor
  }

  if (curDist < COLL_DIST) {                               // if the current distance to object is less than the collision distance
    checkCourse();                                          // run the checkCourse function
    // jump out of this loop
  }

  if (curDist < TURN_DIST) {                                 // if current distance is less than the turn distance
    changePath();
  }


  {
    if (curDist > curDist) {
      {
        maxAngle = pos;
      }
      if (pos > 90 && curDist > curLeft) {
        curLeft = curDist;
      }
      if (pos == 90 && curDist > curFront) {
        curFront = curDist;
      }
      if (pos < 90 && curDist > curRight) {
        curRight = curDist;
      }
    }

    maxLeft = curLeft;
    maxRight = curRight;
    maxFront = curFront;
  }




  moveForward();
}











//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------

void setup() {

  checkPath(); // run the CheckPath routine to find the best path to begin travel
  motorSet = "FORWARD"; // set the director indicator variable to FORWARD
  moveForward(); // run function to make robot move forward
}



//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  moveForward();        // check that if the robot is supposed to be moving forward, that the drive motors          are set to move forward - this is needed to overcome some issues with only using 4 AA NiMH batteries

  checkPath(); // set ultrasonic sensor to scan for any possible obstacles
}




//-------------------------------------------------------------------------------------------------------------------------------------









