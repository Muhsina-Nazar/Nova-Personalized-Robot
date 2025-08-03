#include <Servo.h>

// Servo objects
Servo armServo;    // Pin 9 - Arm/Wave
Servo headServo;   // Pin 10 - Head/Nod

// Current positions
int armPos = 0;    // 0-180 (0 = arm down)
int headPos = 90;  // 0-180 (90 = center)

void setup() {
  Serial.begin(9600);
  
  // Attach servos with pulse width range (500-2500 microseconds)
  armServo.attach(9, 500, 2500);
  headServo.attach(11, 500, 2500);
  
  // Initial positions
  armServo.write(armPos);
  headServo.write(headPos);
  
  delay(1000); // Allow servos to reach position
  Serial.println("NOVA: Ready");
}

void loop() {
  if (Serial.available() >= 4) { // Expecting 2 numbers
    // Read positions
    armPos = Serial.parseInt();
    headPos = Serial.parseInt();
    
    // Clear buffer
    while (Serial.available() > 0) Serial.read();
    
    // Constrain values and move servos
    armPos = constrain(armPos, 0, 180);
    headPos = constrain(headPos, 0, 180);
    
    armServo.write(armPos);
    headServo.write(headPos);
    
    // Optional: Send confirmation back
    Serial.print("Moving to Arm:");
    Serial.print(armPos);
    Serial.print(" Head:");
    Serial.println(headPos);
  }
  delay(20);
}