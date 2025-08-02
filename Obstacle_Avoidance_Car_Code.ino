#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// Pin Definitions
#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 190
#define CENTER_POSITION 115

// Objects
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

// Variables
boolean isMovingForward = false;
int obstacleDistance = 0;

void setup() {
  Serial.begin(9600);               // Initialize Serial communication
  myservo.attach(10);               // Attach servo motor to pin 10
  myservo.write(CENTER_POSITION);   // Set servo to default center position
  delay(2000);                      // Allow time for servo initialization
}

void loop() {
  obstacleDistance = readDistance();
  Serial.print("Distance: ");
  Serial.println(obstacleDistance);

  if (obstacleDistance <= 15) {
    handleObstacle();
  } else {
    moveForward();
  }
}

// Function to Read Distance from Ultrasonic Sensor
int readDistance() {
  delay(70);                        // Stabilize sensor readings
  int distance = sonar.ping_cm();
  return (distance == 0) ? MAX_DISTANCE : distance;
}

// Obstacle Handling Function
void handleObstacle() {
  moveStop();
  delay(200);

  moveBackward();
  delay(300);
  moveStop();
  delay(200);

  int distanceRight = checkDirection(50);  // Look Right
  int distanceLeft = checkDirection(170);  // Look Left

  if (distanceRight > distanceLeft) {
    turnRight();
  } else {
    turnLeft();
  }

  moveForward();
}

// Check Distance in a Specific Direction
int checkDirection(int angle) {
  myservo.write(angle);  // Move servo to specified angle
  delay(500);
  int distance = readDistance();
  myservo.write(CENTER_POSITION);  // Reset servo to center
  delay(100);
  return distance;
}

// Motor Control Functions
void moveForward() {
  if (!isMovingForward) {
    isMovingForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    gradualSpeedUp(MAX_SPEED);
  }
}

void moveBackward() {
  isMovingForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  gradualSpeedUp(MAX_SPEED);
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

// Gradual Speed Increase
void gradualSpeedUp(int targetSpeed) {
  for (int speed = 0; speed <= targetSpeed; speed += 10) {
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
    delay(5);
  }
}