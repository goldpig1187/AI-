int RightMotor_E_pin = 5;                                        // 오른쪽 모터의 Enable & PWM
int LeftMotor_E_pin = 6;                                         // 왼쪽 모터의 Enable & PWM
int RightMotor_1_pin = 8;                                        // 오른쪽 모터 제어선 IN1
int RightMotor_2_pin = 9;                                        // 오른쪽 모터 제어선 IN2
int LeftMotor_3_pin = 10;                                        // 왼쪽 모터 제어선 IN3
int LeftMotor_4_pin = 11;                                        // 왼쪽 모터 제어선 IN4

int L_Line = A5;                                                 // 왼쪽 라인트레이서 센서는 A5 핀에 연결
int C_Line = A4;                                                 // 가운데 라인트레이서 센서는 A4 핀에 연결
int R_Line = A3;                                                 // 오른쪽 라인트레이서 센서는 A3 핀에 연결

int L_MotorSpeed = 170;                                          // 왼쪽 모터 속도
int R_MotorSpeed = 150;                                          // 오른쪽 모터 속도

int SL = 1;
int SC = 1;
int SR = 1;

void setup() {
  pinMode(RightMotor_E_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);
}

void loop() {
  int L = digitalRead(L_Line);
  int C = digitalRead(C_Line);
  int R = digitalRead(R_Line);

  // 1. 좌회전 우선 판단 (왼쪽 센서가 닿으면 무조건 좌회전)
  if ( L == HIGH ) { 
    digitalWrite(RightMotor_1_pin, HIGH);
    digitalWrite(RightMotor_2_pin, LOW);
    digitalWrite(LeftMotor_3_pin, LOW);
    digitalWrite(LeftMotor_4_pin, LOW);
    analogWrite(RightMotor_E_pin, R_MotorSpeed);
    analogWrite(LeftMotor_E_pin, 0);
    SL = 1; SR = 0; // 마지막 위치 기억
  }
  
  // 2. 우회전 우선 판단 (오른쪽 센서가 닿으면 무조건 우회전)
  else if ( R == HIGH ) { 
    digitalWrite(RightMotor_1_pin, LOW);
    digitalWrite(RightMotor_2_pin, LOW); 
    digitalWrite(LeftMotor_3_pin, HIGH);
    digitalWrite(LeftMotor_4_pin, LOW);
    analogWrite(RightMotor_E_pin, 0); 
    analogWrite(LeftMotor_E_pin, L_MotorSpeed);
    SL = 0; SR = 1; // 마지막 위치 기억
  }

  // 3. 직진 (가운데만 닿았을 때)
  else if ( C == HIGH ) {
    digitalWrite(RightMotor_1_pin, HIGH);
    digitalWrite(RightMotor_2_pin, LOW);
    digitalWrite(LeftMotor_3_pin, HIGH);
    digitalWrite(LeftMotor_4_pin, LOW);
    analogWrite(RightMotor_E_pin, R_MotorSpeed);
    analogWrite(LeftMotor_E_pin, L_MotorSpeed);
  }

  // 4. 선을 완전히 이탈했을 때 (0 0 0)
  else {
    // 마지막에 기억된 방향(SL, SR)에 따라 선을 찾을 때까지 회전 유지
    if (SL == 1) { /* 좌회전 유지 */ } 
    else if (SR == 1) { /* 우회전 유지 */ }
  }
}