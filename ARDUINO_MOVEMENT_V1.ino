#include <Servo.h>
Servo myservo;

const int stepPin1 = 2;  // Step pin for Motor 1 (left = HIGH, right = LOW)
const int dirPin1 = 3;   // Direction pin for Motor 1
const int stepPin2 = 6;  // Step pin for Motor 2 (up = LOW, down = HIGH)
const int dirPin2 = 7;   // Direction pin for Motor 2

bool grab = true;
void setup() {
  // Set all pins as Outputs
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  myservo.attach(10);

  // Move to home position slowly at startup
  clawopen();
  clawclose();
  goHome();
  hometo_00();
  //hometo_01();
  hometo_02();
  //hometo_10();
  //hometo_11();
  //hometo_12();
  //hometo_20();
  //hometo_21();
  //hometo_22();
  //figureoutsteps();
}

void loop() {
  // Main code
  delay(1000);  // Add a delay or other motor actions after homing
}

void goHome() {
  // Set the direction towards home position for both motors
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, LOW);  // Adjust direction if needed
  myservo.write(100);

  // Move both motors 200 steps to reach the home position
  for (int x = 0; x < 1250; x++) {
    //begins 1.8cm from corner, finishes 16.5 cm 16.5-1.8 =~ 15cm
     //Assume 1 step == 0.17cm
     
    // Step Motor 1
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    
   
    // Step Motor 2
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    
    delayMicroseconds(1000);  // Small delay between each full step cycle
  }

delay(1000);
  // Motors are now at the home position
}


//each movement function will be of type boolean, true == pick up, false == drop
void hometo_00(){
   // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 6cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }
  
    //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //up first
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }
  //This for loop moves it left 4cm
  for(int x = 0; x < 200; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
delay(1000);
  
}

void hometo_01(){
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 285; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  ///This for loop moves it down 6cm
  for(int x = 0; x < 285; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }


  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  
delay(1000);
}

void hometo_02(){
  
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 40mm
  for(int x = 0; x < 222; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 53.748mm
  for(int x = 0; x < 270; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //
  if(!grab){
    clawopen();
    grab = true;
  }
  else if(grab == true){
    clawclose();
    grab = false;
  }

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down ~5.38cm
  for(int x = 0; x < 270; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 222; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
}

void hometo_10(){
  
}

void hometo_11(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, HIGH);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it down 6cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  
  delay(1000);
}

void hometo_12(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it down 6cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, LOW);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  
  delay(1000);
}

void hometo_20(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4.55cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home
  delay(2000);
  digitalWrite(dirPin1, HIGH);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
delay(1000);
  
}

void hometo_21(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4.55cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, HIGH);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 400; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
delay(1000);
  
}

void hometo_22(){
  // 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, HIGH); 
  //function needs to move right 4cm, up 6cm

  //This for loop moves it right 4.55cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it up 5.68cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //Here would be either the pickup or drop function
  //
  //
  //

  //Now we go back to home

  delay(2000);
  digitalWrite(dirPin1, HIGH);  
  digitalWrite(dirPin2, LOW); 

  //This for loop moves it down 6cm
  for(int x = 0; x < 500; x++){
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  //This for loop moves it left 4cm
  for(int x = 0; x < 300; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);
}

void clawopen(){
  myservo.write(100);
  delay(1000);
}


void clawclose(){
  myservo.write(35);
  delay(1000);
}


//math:
/*
whole board is 43 cm, Entire block holder portion is 21 cm, blocks
are entirely held within 15cm, divided evenly. 2.5cm dividers
middle part
43-21 = 22cm to the right to get to first column
To figure out steps:

void figureoutsteps(){
  //35 mm from left post
  //finished 18mm away
  //100 steps = 18mm -> 1 step = 0.18mm == 0.018cm
  digitalWrite(dirPin1, LOW);  // Adjust direction if needed
  digitalWrite(dirPin2, LOW); 
  for(int x = 0; x < 200; x++){
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(1000);  // Longer delay for slower speed
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(1000);
  }
}

//total height of 3 blocks is 93.8mm
31.26 distance between each one
base is 5mm from table -> total height is 98.8mm
150mm from center of robot to top block
150-98.8 = 51.2mm distance from center
center of robot is 
*/
