// --- IR SENSOR PINS (S8 fixed on Pin 10) ---
const int S1 = A0;  // Far Left
const int S2 = A1;
const int S3 = A2;
const int S4 = A3;  // Center Left
const int S5 = A4;  // Center Right
const int S6 = A5;
const int S7 = 12;
const int S8 = 10;  // Far Right (Moved off Pin 13)
const int IR_EN = 11;

// --- L298N MOTOR DRIVER PINS (Hardware PWM) ---
const int IN1 = 2;   // Left Motor Dir 1
const int IN2 = 4;   // Left Motor Dir 2
const int ENA = 5;   // Left Motor Speed
const int IN3 = 7;   // Right Motor Dir 1
const int IN4 = 8;   // Right Motor Dir 2
const int ENB = 6;   // Right Motor Speed

// --- HIGH-SPEED PID TUNING PARAMETERS ---
float Kp = 35.0;   // Boosted for sharper steering
float Ki = 0.0;    
float Kd = 180.0;  // Boosted for aggressive braking on curves

float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;

// --- SPEED SETTINGS ---
int MAX_SPEED = 180;         // Cranked up base speed
const int SEARCH_SPEED = 75; // Slower speed when completely lost hunting for the line

unsigned long lastPIDTime = 0;
const unsigned long PID_INTERVAL = 10; // 100Hz loop

char lastTurn = 'F'; 

void setup() {
  Serial.begin(9600);
  
  pinMode(S1, INPUT); pinMode(S2, INPUT); pinMode(S3, INPUT); pinMode(S4, INPUT);
  pinMode(S5, INPUT); pinMode(S6, INPUT); pinMode(S7, INPUT); pinMode(S8, INPUT);
  
  pinMode(IR_EN, OUTPUT);
  digitalWrite(IR_EN, HIGH); 
  
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  
  stopMotors();
  Serial.println("Line Follower Starting in 3 Seconds...");
  delay(3000); 
}

void loop() {
  unsigned long now = millis();

  if (now - lastPIDTime >= PID_INTERVAL) {
    lastPIDTime = now;
    
    int v1 = digitalRead(S1); int v2 = digitalRead(S2);
    int v3 = digitalRead(S3); int v4 = digitalRead(S4);
    int v5 = digitalRead(S5); int v6 = digitalRead(S6);
    int v7 = digitalRead(S7); int v8 = digitalRead(S8);

    float sum = 0;
    int activeSensors = 0;

    if (v1 == 1) { sum += -4.0; activeSensors++; }
    if (v2 == 1) { sum += -3.0; activeSensors++; }
    if (v3 == 1) { sum += -1.5; activeSensors++; }
    if (v4 == 1) { sum += -0.5; activeSensors++; }
    if (v5 == 1) { sum += 0.5;  activeSensors++; }
    if (v6 == 1) { sum += 1.5;  activeSensors++; }
    if (v7 == 1) { sum += 3.0;  activeSensors++; }
    if (v8 == 1) { sum += 4.0;  activeSensors++; }

    if (activeSensors > 0) {
      error = sum / activeSensors;
    } else {
      error = 0;
    }

    integral += error;
    integral = constrain(integral, -100, 100); 
    derivative = error - lastError;
    float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
    lastError = error;

    // --- AGGRESSIVE AUTO SPEED REGULATION ---
    // Multiplied by 30 so the bot heavily slams the brakes on corners
    int baseSpeed = MAX_SPEED - (abs(error) * 30);
    if (baseSpeed < 45) baseSpeed = 45; // Minimum stall threshold

    int leftSpeed = baseSpeed + correction; 
    int rightSpeed = baseSpeed - correction;

    // --- DECISION LOGIC ---
    if (v4 == 1 || v5 == 1) {
      driveForward(leftSpeed, rightSpeed);
      lastTurn = 'F';
    } 
    else if (v1 == 1 || v2 == 1 || v3 == 1) {
      spinLeft(leftSpeed, rightSpeed);  
      lastTurn = 'L';
    } 
    else if (v6 == 1 || v7 == 1 || v8 == 1) {
      spinRight(leftSpeed, rightSpeed); 
      lastTurn = 'R';
    } 
    else {
      // Memory state: lost the line, use last known direction to find it
      if (lastTurn == 'L') {
        spinLeft(-30, SEARCH_SPEED);
      } else if (lastTurn == 'R') {
        spinRight(SEARCH_SPEED, -30);
      } else {
        stopMotors(); 
      }
    }
  }
}

// --- NATIVE HARDWARE PWM MOVEMENT FUNCTIONS ---

void driveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, constrain(leftSpeed, 0, 255)); 
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}

void spinLeft(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); 
  analogWrite(ENA, constrain(abs(leftSpeed), 60, 255));
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}

void spinRight(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); 
  analogWrite(ENB, constrain(abs(rightSpeed), 60, 255));
}

void stopMotors() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}