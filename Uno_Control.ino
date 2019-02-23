#include <Stepper.h>

#define STEPS 200

// Initialize Stepper Pins
Stepper Compartment_stepper(STEPS, 4, 5, 6, 7);
Stepper Elevator_stepper(STEPS, 8, 9, 10, 12);

// Initialize Alarm
int ledPin = 2; // need to change these
int soundPin = 3; // need to change these
int frequency = 1000;
int time1;
boolean beep = true;


int CurrentLoc = 6;//value 1-8. Need a way to initialize this value. Could link to LCD screen for user calibration

void setup() { // put your setup code here, to run once:
  Serial.begin(9600); // baud rate
  
  // set the speed of the motor in RPMs
  Compartment_stepper.setSpeed(10);
  Elevator_stepper.setSpeed(5);
  bool Move_Elevator = true;
  bool dispensed = false;
  bool IR = false; // continue running elevator code

  // set LED and Buzzer Pins
  pinMode(ledPin, OUTPUT);
  pinMode(soundPin, OUTPUT);


}

void loop() {  // put your main code here, to run repeatedly:
  
  // Compartment_Stepper Code
  bool dispense = true;
  if (dispense == true){
    int NewLoc = 8; // need new location input from somewhere
    Move_Compartment_Stepper(NewLoc);

   // Elevator_Stepper Code
   if (Move_Elevator == true){ // run code to capture pill
      
      float Dist = 20.6; // distance in mm to extend pill elevator into compartment
      
      while (dispensed == false){ // rerun code to capture pill, extending pill compartment slighly further
        Move_Elevator_Stepper(Dist);
        //Dist = Dist + 1; // go 1mm further each repetition
        
          if (IR == true){ // stop repeating dispensing mechanism
              dispensed = true;
          }
     }
  }
}
// alarm(); // LED and Buzzer
}

// Rotate to Correct Pill Compartment
void Move_Compartment_Stepper(int NewLoc){ 

    int NewLocSteps = (NewLoc - 1) * 25; // convert current location into steps
    int CurrentLocSteps = (CurrentLoc - 1) * 25; // convert current location into steps. 25 steps for 45 degrees

    int NumSteps = CurrentLocSteps - NewLocSteps; // find num steps between current container and new container
    
    int Direction = 1; // default direction is clockwise (+1)

    // Test whether counter clockwise is faster
    if ((NumSteps >= 25 && NumSteps <= 100) || (NumSteps >= -175 && NumSteps < -100)){ // if 25 <= NumSteps <= 100 or if -175 <= NumSteps < -100
      Direction = -1; // change direction to CCW (-1)
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
    
    Serial.println(Move); // print movement
    Elevator_stepper.step(Move); // move pill elevator into compartment

    Serial.println("Wait for pill to enter");
    delay(2000); // wait seconds. Pill falls in during this time
    
    Serial.println(-Move); // print movement
    Elevator_stepper.step(-Move); // move pill elevator out of compartment

    Serial.println("Wait for before attempting again");
    delay(3000); // wait 3 seconds. Pill enters chute during this time
}

// Set off Alarm
void alarm(){
  time1 = millis();
  while ((millis()-time1) < 10000 && beep){
  digitalWrite(ledPin, HIGH);
  tone(soundPin, frequency);
  delay(1000);
  digitalWrite(ledPin, LOW);
  noTone(soundPin);
  delay(1000);
  }
  beep = false;
}
