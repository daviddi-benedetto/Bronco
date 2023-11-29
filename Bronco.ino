Tank control code // Battle Bot Control Code
// Authors: David Di-Benedetto, William Marsh, Mehul Gupta, Brycen Miller

// Pins
int receiverPin2 = 2; // left joystick
int receiverPin4 = 4; // right joystick
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
  pinMode(receiverPin2, INPUT); // right joystick
  pinMode(receiverPin3, INPUT); // left joystick
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
  digitalWrite(LeftMotorDirectionPin, LOW); // Clockwise
  digitalWrite(RightMotorDirectionPin, LOW); // Counter Clockwise
  
  digitalWrite(BeaterMotorInput1, LOW); // Enable Beater Bar Motor 
  analogWrite(BeaterMotorENA, 0); // Set Beater Bar to maximum speed
}

void loop() {
  int pwmValue2 = pulseIn(receiverPin2, HIGH); // Read PWM value for left joystick
  int pwmValue4 = pulseIn(receiverPin4, HIGH); // Read PWM value for right joystick
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

   // Tank Controls
  int leftSpeed = map(pwmValue2, minPWM, maxPWM, -255, 255); // Map left joystick to left motor speed
  int rightSpeed = map(pwmValue4, minPWM, maxPWM, -255, 255); // Map right joystick to right motor speed

  // Apply the speed to left and right motors
  analogWrite(LeftMotorPWMPin, abs(leftSpeed));
  analogWrite(RightMotorPWMPin, abs(rightSpeed));

  // Set motor directions based on the sign of the speed
  digitalWrite(LeftMotorDirectionPin, leftSpeed >= 0 ? HIGH : LOW);
  digitalWrite(RightMotorDirectionPin, rightSpeed >= 0 ? HIGH : LOW);


  // Additional delay for stability
  delay(10);
  }
  
  }





