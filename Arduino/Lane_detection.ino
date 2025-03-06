// Motor pins
#define PIN_EN_M_LEFT 22
#define PIN_1_M_LEFT 17
#define PIN_2_M_LEFT 5
#define PIN_EN_M_RIGHT 23
#define PIN_1_M_RIGHT 18
#define PIN_2_M_RIGHT 19

// Sensor pins
#define PIN_LINE_LEFT 4
#define PIN_LINE_RIGHT 2

// PI controller parameters
float Kp = 0.059;
float Ki = 10.7;

float Kp_right = 0.0646;
float Ki_left = 2.93;

// Variables
int BASE_SPEED = 150;
float integral = 0;
unsigned long previousTime = 0;

void setup() {
  Serial.begin(9600);

  // Motor pin setup
  pinMode(PIN_EN_M_LEFT, OUTPUT);
  pinMode(PIN_1_M_LEFT, OUTPUT);
  pinMode(PIN_2_M_LEFT, OUTPUT);
  pinMode(PIN_EN_M_RIGHT, OUTPUT);
  pinMode(PIN_1_M_RIGHT, OUTPUT);
  pinMode(PIN_2_M_RIGHT, OUTPUT);

  // Sensor pin setup
  pinMode(PIN_LINE_LEFT, INPUT);
  pinMode(PIN_LINE_RIGHT, INPUT);
}

void loop() {
  int sensorLeft = digitalRead(PIN_LINE_LEFT);
  int sensorRight = digitalRead(PIN_LINE_RIGHT);

  int error = sensorRight - sensorLeft;

  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  integral += error * elapsedTime;
  float controlSignal = Kp * error + Ki * integral;

  int leftSpeed = BASE_SPEED - controlSignal;
  int rightSpeed = BASE_SPEED + controlSignal;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Control logic for different sensor states
  if (sensorLeft == 0 && sensorRight == 0) {
    // Both sensors detect the line: move forward
    moveForward(leftSpeed, rightSpeed);
  } else if (sensorLeft == 0 && sensorRight == 1) {
    // Left sensor detects line, right does not: turn right
    moveRight(leftSpeed, rightSpeed);
  } else if (sensorLeft == 1 && sensorRight == 0) {
    // Right sensor detects line, left does not: turn left
    moveLeft(leftSpeed, rightSpeed);
  } else {
    // No line detected: stop or search
    stopMotors();
  }

  // Debugging output
  Serial.print("Left Sensor: ");
  Serial.print(sensorLeft);
  Serial.print(" | Right Sensor: ");
  Serial.print(sensorRight);
  Serial.print(" | Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);

  delay(50); 
}

void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(PIN_1_M_LEFT, LOW);
  digitalWrite(PIN_2_M_LEFT, HIGH);
  analogWrite(PIN_EN_M_LEFT, leftSpeed);

  digitalWrite(PIN_1_M_RIGHT, LOW);
  digitalWrite(PIN_2_M_RIGHT, HIGH);
  analogWrite(PIN_EN_M_RIGHT, rightSpeed);
}

void moveRight(int leftSpeed, int rightSpeed) {
  digitalWrite(PIN_1_M_LEFT, LOW);
  digitalWrite(PIN_2_M_LEFT, HIGH);
  analogWrite(PIN_EN_M_LEFT, BASE_SPEED);

  digitalWrite(PIN_1_M_RIGHT, LOW);
  digitalWrite(PIN_2_M_RIGHT, LOW);
  analogWrite(PIN_EN_M_RIGHT, 0);
}

void moveLeft(int leftSpeed, int rightSpeed) {
  digitalWrite(PIN_1_M_LEFT, LOW);
  digitalWrite(PIN_2_M_LEFT, LOW);
  analogWrite(PIN_EN_M_LEFT, 0);

  digitalWrite(PIN_1_M_RIGHT, LOW);
  digitalWrite(PIN_2_M_RIGHT, HIGH);
  analogWrite(PIN_EN_M_RIGHT, BASE_SPEED);
}

void stopMotors() {
  digitalWrite(PIN_1_M_LEFT, LOW);
  digitalWrite(PIN_2_M_LEFT, LOW);
  analogWrite(PIN_EN_M_LEFT, 0);

  digitalWrite(PIN_1_M_RIGHT, LOW);
  digitalWrite(PIN_2_M_RIGHT, LOW);
  analogWrite(PIN_EN_M_RIGHT, 0);
}