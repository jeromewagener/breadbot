#include <LiquidCrystal.h>
#include <Servo.h>

// Give some names to pins...
// This will vary depending on your stepper. Dont forget the motor drivers...
#define MOTOR_1_COIL_A 13
#define MOTOR_1_COIL_B 10
#define MOTOR_1_COIL_C 11
#define MOTOR_1_COIL_D 12
#define MOTOR_2_COIL_A 2
#define MOTOR_2_COIL_B 1
#define MOTOR_2_COIL_C 0
#define MOTOR_2_COIL_D 3

// Delay between stepper motor steps
// If too high, then there is no torque and the robot wont move
// If too low, it is boring to watch the robot
#define STEPPER_STEP_DELAY 30

// I am using a Hitachi-compatible LCD in 4-bit mode 
// This requires 7 pins, while the 8-bit mode would need 11...
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

// Servo to move platform with webcam / optoelectronic distance sensor
Servo servoMotor;

// Default message sent by the robot over the serial interface
String statusMessage = "Ready";

// Counter how many error messages have already been written to the serial port
// before defaulting back to the default status message.
// Bad idea to do this... There are better ways! :-)
int errorCounter = 0;

void setup() {
  // Defining all stepper motor pins as outputs
  pinMode(MOTOR_2_COIL_C, OUTPUT);
  pinMode(MOTOR_2_COIL_B, OUTPUT); 
  pinMode(MOTOR_2_COIL_A, OUTPUT); 
  pinMode(MOTOR_2_COIL_D, OUTPUT);
  pinMode(MOTOR_1_COIL_B, OUTPUT);
  pinMode(MOTOR_1_COIL_C, OUTPUT); 
  pinMode(MOTOR_1_COIL_D, OUTPUT); 
  pinMode(MOTOR_1_COIL_A, OUTPUT);
  
  // Initialize the serial communication as we wait for commands from the PC
  Serial.begin(9600);

  // Initialize the LCD
  lcd.begin(16, 2);

  // Connect the servo motor and use a horizontal viewing angle to start with
  servoMotor.attach(A4);
  moveServo("70");  
}

void loop() {
  // Here be Dragons.. Please dont copy this ever even if it works for a quick test.. ;-)
  if (errorCounter < 10) {
    // Keeping the error message on the output
    errorCounter++;
  } else {
    // Resetting the error message
    errorCounter = 0;
    statusMessage = "Ready";
  }

  // Writing whatever status message is currently set to the output
  Serial.println(statusMessage);

  // Read instructions from the PC and write them to the LCD for easy debugging
  String input = Serial.readString();
  input.trim();
  lcd.setCursor(0, 0);
  lcd.print(input);

  // Decide what to do with the PC instruction. Ignore anything we cannot understand
  if (input.startsWith("moveServo=")){
    moveServo(input.substring(10, input.length()));
  } else if (input.startsWith("moveForward=")){
    moveForward(input.substring(12, input.length()));
  } else if (input.startsWith("moveBackward=")){
    moveBackward(input.substring(13, input.length()));
  } else if (input.startsWith("turnLeft=")){
    turnLeft(input.substring(9, input.length()));
  } else if (input.startsWith("turnRight=")){
    turnRight(input.substring(10, input.length()));
  }
}

void moveServo(String servoPosition) {
   servoMotor.write(servoPosition.toInt());
}

void moveForward(String units) {
  // Serial interferes with stepper, so while we move I am deactivating
  // the serial communication... What could possibly go wrong... :-D
  Serial.end();
  
  // Powering the stepper motor coils through a motor driver
  // in the right order and waiting a reasonable amount of time between steps
  // results in movemnt
  for (int i=0; i<units.toInt(); i++) {
    digitalWrite(MOTOR_1_COIL_A, HIGH);
    digitalWrite(MOTOR_1_COIL_D, LOW);
    digitalWrite(MOTOR_2_COIL_A, HIGH);
    digitalWrite(MOTOR_2_COIL_D, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_B, HIGH);
    digitalWrite(MOTOR_1_COIL_A, LOW);
    digitalWrite(MOTOR_2_COIL_B, HIGH);
    digitalWrite(MOTOR_2_COIL_A, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_C, HIGH);
    digitalWrite(MOTOR_1_COIL_B, LOW);
    digitalWrite(MOTOR_2_COIL_C, HIGH);
    digitalWrite(MOTOR_2_COIL_B, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_D, HIGH);
    digitalWrite(MOTOR_1_COIL_C, LOW);
    digitalWrite(MOTOR_2_COIL_D, HIGH);
    digitalWrite(MOTOR_2_COIL_C, LOW);
    delay(STEPPER_STEP_DELAY);

    // In forward drive mode, we check after each four steps if something is 
    // getting close to the optoelectronic distance measuring sensor in which
    // case the sensor value goes up. If this is the case we abort the move
    int sensorValue = analogRead(A5);
    if (sensorValue > 300) {
      statusMessage = "MoveAborted";
      break;
    }
  }

  Serial.begin(9600);
}

void moveBackward(String units) {
  // Serial interferes with stepper, so while we move I am deactivating
  // the serial communication... What could possibly go wrong... :-D
  Serial.end();
  
  // Powering the stepper motor coils through a motor driver
  // in the right order and waiting a reasonable amount of time between steps
  // results in movemnt
  for (int i=0; i<units.toInt(); i++) {
    digitalWrite(MOTOR_1_COIL_A, HIGH);
    digitalWrite(MOTOR_1_COIL_B, LOW);
    digitalWrite(MOTOR_2_COIL_A, HIGH);
    digitalWrite(MOTOR_2_COIL_B, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_D, HIGH);
    digitalWrite(MOTOR_1_COIL_A, LOW);
    digitalWrite(MOTOR_2_COIL_D, HIGH);
    digitalWrite(MOTOR_2_COIL_A, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_C, HIGH);
    digitalWrite(MOTOR_1_COIL_D, LOW);
    digitalWrite(MOTOR_2_COIL_C, HIGH);
    digitalWrite(MOTOR_2_COIL_D, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_B, HIGH);
    digitalWrite(MOTOR_1_COIL_C, LOW);
    digitalWrite(MOTOR_2_COIL_B, HIGH);
    digitalWrite(MOTOR_2_COIL_C, LOW);
    delay(STEPPER_STEP_DELAY);
  }

  Serial.begin(9600);
}

void turnRight(String units) {
  // Serial interferes with stepper, so while we move I am deactivating
  // the serial communication... What could possibly go wrong... :-D
  Serial.end();
  
  // Powering the stepper motor coils through a motor driver
  // in the right order and waiting a reasonable amount of time between steps
  // results in movemnt
  for (int i=0; i<units.toInt(); i++) {
    digitalWrite(MOTOR_1_COIL_A, HIGH);
    digitalWrite(MOTOR_1_COIL_B, LOW);
    digitalWrite(MOTOR_2_COIL_A, HIGH);
    digitalWrite(MOTOR_2_COIL_D, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_D, HIGH);
    digitalWrite(MOTOR_1_COIL_A, LOW);
    digitalWrite(MOTOR_2_COIL_B, HIGH);
    digitalWrite(MOTOR_2_COIL_A, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_C, HIGH);
    digitalWrite(MOTOR_1_COIL_D, LOW);
    digitalWrite(MOTOR_2_COIL_C, HIGH);
    digitalWrite(MOTOR_2_COIL_B, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_B, HIGH);
    digitalWrite(MOTOR_1_COIL_C, LOW);
    digitalWrite(MOTOR_2_COIL_D, HIGH);
    digitalWrite(MOTOR_2_COIL_C, LOW);
    delay(STEPPER_STEP_DELAY);
  }

  Serial.begin(9600);
}

void turnLeft(String units) {
  // Serial interferes with stepper, so while we move I am deactivating
  // the serial communication... What could possibly go wrong... :-D
  Serial.end();
  
  // Powering the stepper motor coils through a motor driver
  // in the right order and waiting a reasonable amount of time between steps
  // results in movemnt
  for (int i=0; i<units.toInt(); i++) {
    digitalWrite(MOTOR_1_COIL_A, HIGH);
    digitalWrite(MOTOR_1_COIL_D, LOW);
    digitalWrite(MOTOR_2_COIL_A, HIGH);
    digitalWrite(MOTOR_2_COIL_B, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_B, HIGH);
    digitalWrite(MOTOR_1_COIL_A, LOW);
    digitalWrite(MOTOR_2_COIL_D, HIGH);
    digitalWrite(MOTOR_2_COIL_A, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_C, HIGH);
    digitalWrite(MOTOR_1_COIL_B, LOW);
    digitalWrite(MOTOR_2_COIL_C, HIGH);
    digitalWrite(MOTOR_2_COIL_D, LOW);
    delay(STEPPER_STEP_DELAY);
    
    digitalWrite(MOTOR_1_COIL_D, HIGH);
    digitalWrite(MOTOR_1_COIL_C, LOW);
    digitalWrite(MOTOR_2_COIL_B, HIGH);
    digitalWrite(MOTOR_2_COIL_C, LOW);
    delay(STEPPER_STEP_DELAY);
  }

  Serial.begin(9600);
}
