#include <Stepper.h>

#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 4, 5, 6, 7);


void setup()
{
  Serial.begin(9600); // baud rate(?)
  Serial.println("Stepper test!");
  // set the speed of the motor in RPMs
  stepper.setSpeed(20);
}

void loop()
{
  bool dispense = true;
  if (dispense == true){
    
    int NewLoc = 2;// take number of pill compartment
    int CurrentLoc = 6;//value 1-8

    int NewLocSteps = (NewLoc - 1) * 25; // convert current location into steps
    // need to store current location
    int CurrentLocSteps = (CurrentLoc - 1) * 25; // convert current location into steps. 25 steps for 45 degrees

    int NumSteps = CurrentLocSteps - NewLocSteps; // find num steps between current container and new container
    
    int Direction = 1; // default direction is clockwise (+1)

    // Test whether counter clockwise is faster
    if (NumSteps >= 25 && NumSteps <= 100){ // if 25 <= NumSteps <= 100
      Direction = -1; // change direction to CCW (-1)
    }
    if (NumSteps >= -175 && NumSteps < -100){ // if -175 <= NumSteps < -100
      Direction = -1; // change direction to CCW (-1)
    }

    int Move = abs(NumSteps)*Direction; // move number of steps either CW or CCW
    Serial.println(Move); // print movement
    stepper.step(Move); // turn stepper
    delay(5000); // wait 5 seconds
  }
} 
