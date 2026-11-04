int RightMotor_E_pin = 5;
int LeftMotor_E_pin = 6;
int RightMotor_1_pin = 8;
int RightMotor_2_pin = 9;
int LeftMotor_3_pin = 10;
int LeftMotor_4_pin = 11;

int L_Line = A5;
int C_Line = A4;
int R_Line = A3;

// 성능: 20.XX 초
// 기본 베이스 속도: 205, 185 / 210, 190 / 220, 200 / 230, 210
int L_MotorSpeed = 230; 
int R_MotorSpeed = 210; 

// 라인을 이탈했을 때 직전 위치를 기억하기 위한 변수 (0: 중앙, 1: 왼쪽 탈출, 2: 오른쪽 탈출)
int last_position = 0; 

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

  int currentL_Speed = L_MotorSpeed;
  int currentR_Speed = R_MotorSpeed;
  
  // 1. 직진 (중앙 센서만 감지)
  if (L == LOW && C == HIGH && R == LOW) {
    motor_control(1, 1, currentR_Speed, currentL_Speed); 
    last_position = 0; // 중앙에 있음
  }
  
  // 2. 우회전 (오른쪽 센서가 선을 밟음), 40 / 50
  else if (R == HIGH && L == LOW) {
    // 안쪽 바퀴(우측) 속도를 낮추거나 뒤로 돌리고, 바깥쪽(좌측)을 올려서 날카롭게 회전
    motor_control(1, 1, currentR_Speed - 50, currentL_Speed + 50);  
    last_position = 2; // 우측으로 치우침 (라인이 오른쪽에 있음)
  }
  
  // 3. 좌회전 (왼쪽 센서가 선을 밟음)
  else if (L == HIGH && R == LOW) {
    // 안쪽 바퀴(좌측) 속도를 낮추고, 바깥쪽(우측)을 올려서 날카롭게 회전
    motor_control(1, 1, currentR_Speed + 50, currentL_Speed - 50);   
    last_position = 1; // 좌측으로 치우침 (라인이 왼쪽에 있음)
  }
  
  // 4. 교차로 또는 T자 (세 센서 모두 감지) -> 일단 직진 혹은 감속 직진 20/30
  else if (L == HIGH && C == HIGH && R == HIGH) {
    motor_control(1, 1, currentR_Speed - 30, currentL_Speed - 30);
  }
  
  // 5. 라인 완전 이탈 예외 처리 (모든 센서가 LOW)
  else if (L == LOW && C == LOW && R == LOW) {
    if (last_position == 1) {
      // 직전에 왼쪽으로 이탈했으므로, 왼쪽으로 제자리 회전(Pivot Turn)하여 라인 탐색
      motor_control(1, -1, currentR_Speed + 30, currentL_Speed); 
    } 
    else if (last_position == 2) {
      // 직전에 오른쪽으로 이탈했으므로, 오른쪽으로 제자리 회전(Pivot Turn)하여 라인 탐색
      motor_control(-1, 1, currentR_Speed, currentL_Speed + 30); 
    }
    else {
      // 정방향 이탈인 경우 천천히 전진하며 탐색 50
      motor_control(1, 1, currentR_Speed - 50, currentL_Speed - 50);
    }
  }
}

void motor_control(int R_dir, int L_dir, int R_speed, int L_speed) {
  // 우측 모터 방향 제어
  if (R_dir == 1) {
    digitalWrite(RightMotor_1_pin, HIGH);
    digitalWrite(RightMotor_2_pin, LOW);
  } else if (R_dir == -1) {
    digitalWrite(RightMotor_1_pin, LOW);
    digitalWrite(RightMotor_2_pin, HIGH);
  } else { // 정지
    digitalWrite(RightMotor_1_pin, LOW);
    digitalWrite(RightMotor_2_pin, LOW);
  }

  // 좌측 모터 방향 제어
  if (L_dir == 1) {
    digitalWrite(LeftMotor_3_pin, HIGH);
    digitalWrite(LeftMotor_4_pin, LOW);
  } else if (L_dir == -1) {
    digitalWrite(LeftMotor_3_pin, LOW);
    digitalWrite(LeftMotor_4_pin, HIGH);
  } else { // 정지
    digitalWrite(LeftMotor_3_pin, LOW);
    digitalWrite(LeftMotor_4_pin, LOW);
  }

  // 속도 제어 (0~255 제한)
  analogWrite(RightMotor_E_pin, constrain(R_speed, 0, 255));
  analogWrite(LeftMotor_E_pin, constrain(L_speed, 0, 255));
}
