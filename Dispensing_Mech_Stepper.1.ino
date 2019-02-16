#include <Stepper.h>

#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 4, 5, 6, 7); // different pins


void setup()
{
  Serial.begin(9600); // baud rate
  Serial.println("Stepper test!");
  // set the speed of the motor in RPMs
  stepper.setSpeed(20);
}

void loop(){
  
  bool dispenseMech = true;
  bool dispensed = false;
  if (dispenseMech == true){ // run code to capture pill
      
      float Dist = 10; // distance in mm to extend pill elevator into compartment
      
      while (dispensed == false){ // rerun code to capture pill, extending pill compartment slighly further
        Move_Dispense_Mech(Dist);
        Dist = Dist + 1; // go 1mm further each repetition
        
          if (IR Sensor reads pill){ // stop repeating dispensing mechanism
              dispensed = true;
          }
     }
  }
} 

void Move_Dispense_Mech(float Dist){ 

    float tot_Dist = Dist + 40; // '40' should be distance until inner front edge of elevator is in line w inner wall of cylinder so any further movement exposes open elevator
    int Move = tot_Dist / 0.2585834333; // 2.154mm/tooth turn / 8.33 steps / tooth turn = 0.2586 mm traveled / step. Need to check how this rounds
    
    Serial.println(Move); // print movement
    stepper.step(Move); // move pill elevator into compartment
    
    delay(5000); // wait 5 seconds. Pill falls in during this time
    
    Serial.println(Move); // print movement
    stepper.step(-Move); // move pill elevator out of compartment

}
