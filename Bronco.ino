// Battle Bot Control Code
// Authors: David Di-Benedetto, William Marsh, Mehul Gupta, Brycen Miller

// Pins
int receiverPin2 = 2; // Forward/Backward
int receiverPin4 = 4; // Left/Right
int receiverPin6 = 10; // Channel 6 for Beater Motor control, connected to Arduino pin 10

int LeftMotorDirectionPin = 12;
int LeftMotorPWMPin = 3;
int LeftMotorBrakePin = 9; 

int RightMotorDirectionPin = 13;
int RightMotorPWMPin = 11;
int RightMotorBrakePin = 8; 

int BeaterMotorENA = 5;
int BeaterMotorInput1 = 6;
int BeaterMotorInput2 = 7;
 

// PWM
const int minPWM = 1360; // Minimum PWM value
const int maxPWM = 1630; // Maximum PWM value
const int midPWM = (minPWM + maxPWM) / 2; // Middle PWM value, used for neutral position

void setup() {
  // Receiver pins are inputs
  pinMode(receiverPin2, INPUT); // Forward/Backward
  pinMode(receiverPin4, INPUT); // Left/Right
  pinMode(receiverPin6, INPUT); // Setup for Channel 6

  // Motor pins are outputs
  pinMode(LeftMotorDirectionPin, OUTPUT); 
  pinMode(LeftMotorPWMPin, OUTPUT);     
  pinMode(LeftMotorBrakePin, OUTPUT);  

  pinMode(RightMotorDirectionPin, OUTPUT); 
  pinMode(RightMotorPWMPin, OUTPUT);       
  pinMode(RightMotorBrakePin, OUTPUT);

  pinMode(BeaterMotorENA, OUTPUT);                
  pinMode(BeaterMotorInput1, OUTPUT);       
  pinMode(BeaterMotorInput2, OUTPUT);       

  // Put motors to a default state
  digitalWrite(LeftMotorDirectionPin, HIGH); // Clockwise
  digitalWrite(RightMotorDirectionPin, LOW); // Counter Clockwise
  
  digitalWrite(BeaterMotorInput1, LOW); // Enable Beater Bar Motor 
  analogWrite(BeaterMotorENA, 0); // Set Beater Bar to maximum speed
}

void loop() {
  int pwmValue2 = pulseIn(receiverPin2, HIGH); // Read PWM value for forward/backward
  int pwmValue4 = pulseIn(receiverPin4, HIGH); // Read PWM value for left/right
  int pwmValue6 = pulseIn(receiverPin6, HIGH); // Read PWM value for Beater Motor control

  // Beater Bar Control
    // Assuming 1500 as the threshold. Adjust as needed based on your transmitter's behavior
  if (pwmValue6 > 1500) {
    // Switch activated - Full speed
    digitalWrite(BeaterMotorInput1, HIGH);
    analogWrite(BeaterMotorENA, 255);
  } else {
    // Switch not activated - Motor off
    digitalWrite(BeaterMotorInput1, LOW);
    analogWrite(BeaterMotorENA, 0);
  }

  // Disable brakes for motion
  digitalWrite(LeftMotorBrakePin, LOW);
  digitalWrite(RightMotorBrakePin, LOW);

  int forwardSpeed = 0;
  int turnAdjustment = 0;

  // Forward/Backward
  if (pwmValue2 > midPWM) {
    // Move forward
    digitalWrite(LeftMotorDirectionPin, HIGH); 
    digitalWrite(RightMotorDirectionPin, LOW);
    forwardSpeed = map(pwmValue2, midPWM, maxPWM, 0, 255);

  } else if (pwmValue2 < midPWM) {
    // Move backward
    digitalWrite(LeftMotorDirectionPin, LOW); 
    digitalWrite(RightMotorDirectionPin, HIGH);
    forwardSpeed = map(pwmValue2, minPWM, midPWM, 255, 0);
  }
  
  // Turning While Moving Forwards
  if (pwmValue4 > midPWM && pwmValue2 > midPWM) {
    // Turning right while moving forward - reduce speed of right wheel
    turnAdjustment = map(pwmValue4, midPWM, maxPWM, forwardSpeed, 0);
    analogWrite(LeftMotorPWMPin, forwardSpeed);
    analogWrite(RightMotorPWMPin, forwardSpeed - turnAdjustment);

  } else if (pwmValue4 < midPWM && pwmValue2 > midPWM) {
    // Turning left while moving forward - reduce speed of left wheel
    turnAdjustment = map(pwmValue4, minPWM, midPWM, forwardSpeed, 0);
    analogWrite(RightMotorPWMPin, forwardSpeed);
    analogWrite(LeftMotorPWMPin, forwardSpeed - turnAdjustment);

  // Turning While Moving Backwards
  } else if (pwmValue4 > midPWM && pwmValue2 < midPWM) {
    // Turning right while moving backward - reduce speed of left wheel
    turnAdjustment = map(pwmValue4, minPWM, midPWM, forwardSpeed, 0);
    analogWrite(RightMotorPWMPin, forwardSpeed);
    analogWrite(LeftMotorPWMPin, forwardSpeed - turnAdjustment);

  } else {
    // Straight forward or backward, or stop
    analogWrite(LeftMotorPWMPin, forwardSpeed);
    analogWrite(RightMotorPWMPin, forwardSpeed);

    if (pwmValue2 == midPWM) {
      // Stop
      digitalWrite(LeftMotorBrakePin, HIGH);
      digitalWrite(RightMotorBrakePin, HIGH);
    }

  // Turning While Moving Backwards
  if (pwmValue4 > midPWM) {
    // Turning right - reduce speed of right wheel
    turnAdjustment = map(pwmValue4, midPWM, maxPWM, forwardSpeed, 0);
    analogWrite(LeftMotorPWMPin, forwardSpeed);
    analogWrite(RightMotorPWMPin, forwardSpeed - turnAdjustment);

  } else if (pwmValue4 < midPWM) {
    // Turning left - reduce speed of left wheel
    turnAdjustment = map(pwmValue4, minPWM, midPWM, forwardSpeed, 0);
    analogWrite(RightMotorPWMPin, forwardSpeed);
    analogWrite(LeftMotorPWMPin, forwardSpeed - turnAdjustment);

  } else {
    // Straight forward or backward, or stop
    analogWrite(LeftMotorPWMPin, forwardSpeed);
    analogWrite(RightMotorPWMPin, forwardSpeed);

    if (pwmValue2 == midPWM) {
      // Stop
      digitalWrite(LeftMotorBrakePin, HIGH);
      digitalWrite(RightMotorBrakePin, HIGH);
    }
    
  }

  // Additional delay for stability
  delay(10);
  }
  
  }





