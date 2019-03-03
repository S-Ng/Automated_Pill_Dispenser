#include <Stepper.h>

#define STEPS 200

// create an instance of the stepper class, specifying the number of steps of the motor and the pins it's attached to
Stepper Compartment_stepper(STEPS, 4, 5, 6, 7);
Stepper Elevator_stepper(STEPS, 8, 9, 10, 12);
int code = 0;
int dispenseArray[8] = {};
int disNum = 0;
bool dispense = false;
bool dispensed = false;
int CurrentLoc = 1;//value 1-8. Need a way to initialize this value. Could link to LCD screen for user calibration
bool Move_Elevator = true;
bool IR = false; // continue running elevator code
bool first = true;

void setup() { // put your setup code here, to run once:
  Serial.begin(9600); // baud rate
  
  // set the speed of the motor in RPMs
  Compartment_stepper.setSpeed(10);
  Elevator_stepper.setSpeed(5);

}

void loop() {  // put your main code here, to run repeatedly:
 /* Serial.println("Got to void loop");
  if (Serial.available()){
  for (int i = 0; i<5; i++)
  { 
    if (Serial.available())
    { Serial.println("Got past first Serial.available");
      code = Serial.read();
      Serial.println(code);
      dispenseArray[i] = code;
      disNum++;    //counts how many times we are dispensing things
    }
   } dispense = true;
  }*/
  // Compartment_Stepper Code
  dispenseArray[0] = 21;
  dispenseArray[1] = 71;
  dispenseArray[2] = 11;
  dispenseArray[3] = 51;
  dispenseArray[4] = 32;
  dispenseArray[5] = 63;
  dispenseArray[6] = 83;
  dispenseArray[7] = 42;
  if(first){
    dispense = true;
  }
  disNum = 8;
  if (dispense == true){
    for (int j = 0; j < disNum; j++){ 
    int NewLoc = readDigit(dispenseArray[j], 1); // need new location input from somewhere, first digit of 'code' is pill number/pill compartment
    Serial.println(NewLoc);
    Move_Compartment_Stepper(NewLoc);

   // Elevator_Stepper Code
   // run code to capture pill
      for (int k = 0; k < readDigit(dispenseArray[j], 2); k++){
      float Dist = 1.6; // distance in mm to extend pill elevator into compartment
      
      while (dispensed == false){ // rerun code to capture pill, extending pill compartment slighly further
        Move_Elevator_Stepper(Dist);
        //Dist = Dist + 1; // go 1mm further each repetition
        //IR sensor code goes here for making dispensed true
        dispensed = true;
     }
     delay (1000);
     dispensed = false;
      }//this is int k loop, this is reading second digit of 'code' which is value of how many pills to dispense
    }//this is int j loop, this is number of times we are dispensing
  }
  //disNum = 0;
  
  first = false;
  dispense = false;
  dispensed = false;
  for (int l= 0; l++; l<5) {
  dispenseArray[l] = 0;
  }
}

// Rotate to Correct Pill Compartment

void Move_Compartment_Stepper(int NewLoc){ 

    int NewLocSteps = (NewLoc - 1) * 25; // convert current location into steps
    int CurrentLocSteps = (CurrentLoc - 1) * 25; // convert current location into steps. 25 steps for 45 degrees

    int NumSteps = CurrentLocSteps - NewLocSteps; // find num steps between current container and new container
    
    int Direction = -1; // default direction is clockwise (-1)

    // Test whether counter clockwise is faster
    if ((NumSteps >= 25 && NumSteps <= 100) || (NumSteps >= -175 && NumSteps < -100)){ // if 25 <= NumSteps <= 100 or if -175 <= NumSteps < -100
      Direction = 1; // change direction to CCW (+1)
    }

    if (abs(NumSteps) > 100) { // get correct num steps
      NumSteps = 200 - abs(NumSteps);
    }
  

    int Move = abs(NumSteps)*Direction; // move number of steps either CW or CCW
    Serial.println(Move); // print movement
    Compartment_stepper.step(Move); // turn stepper
    delay(1000); // wait seconds

    CurrentLoc = NewLoc; // set new location to current location once new location has been reached
}

// Move Pill Elevator
void Move_Elevator_Stepper(float Dist){ 

    float tot_Dist = Dist + 8.4; // 8.4 is distance until inner front edge of elevator is in line w inner wall of cylinder so any further movement exposes open elevator
    int Move = tot_Dist / 0.2585834333; // 2.154mm/tooth turn / 8.33 steps / tooth turn = 0.2586 mm traveled / step. Need to check how this rounds
    
    //Serial.println(Move); // print movement
    Elevator_stepper.step(Move); // move pill elevator into compartment

    //Serial.println("Wait for pill to enter");
    delay(2000); // wait seconds. Pill falls in during this time
    
    //Serial.println(-Move); // print movement
    Elevator_stepper.step(-Move); // move pill elevator out of compartment

    //Serial.println("Wait for before attempting again");
    delay(3000); // wait 3 seconds. Pill enters chute during this time
}

int readDigit(int doubledigit, int place){
  int code2;
  switch(place){
    case 1:
      code2 = (doubledigit - (doubledigit%10))/10;
      break;

    case 2:
      code2 = doubledigit%10;
      break;
  }
  return code2;
}
