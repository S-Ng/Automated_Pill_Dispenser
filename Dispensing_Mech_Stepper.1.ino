#include <Stepper.h>

#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 4, 5, 6, 7); // different pins 8,9,10,12

bool dispenseMech = true;
bool dispensed = false;
bool IR = false;

void setup()
{
  Serial.begin(9600); // baud rate
  // set the speed of the motor in RPMs
  stepper.setSpeed(5);
}

void loop(){
  
  if (dispenseMech == true){ // run code to capture pill
      
      float Dist = 20.6; // distance in mm to extend pill elevator into compartment
      
      while (dispensed == false){ // rerun code to capture pill, extending pill compartment slighly further
        Move_Dispense_Mech(Dist);
        //Dist = Dist + 1; // go 1mm further each repetition
        
          if (IR == true){ // stop repeating dispensing mechanism
              dispensed = true;
          }
     }
  }
} 

void Move_Dispense_Mech(float Dist){ 

    float tot_Dist = Dist + 8.4; // 8.4 is distance until inner front edge of elevator is in line w inner wall of cylinder so any further movement exposes open elevator
    int Move = tot_Dist / 0.2585834333; // 2.154mm/tooth turn / 8.33 steps / tooth turn = 0.2586 mm traveled / step. Need to check how this rounds
    
    Serial.println(Move); // print movement
    stepper.step(Move); // move pill elevator into compartment

    Serial.println("Wait for pill to enter");
    delay(2000); // wait seconds. Pill falls in during this time
    
    Serial.println(-Move); // print movement
    stepper.step(-Move); // move pill elevator out of compartment

    Serial.println("Wait for before attempting again");
    delay(3000); // wait 3 seconds. Pill falls in during this time
}
