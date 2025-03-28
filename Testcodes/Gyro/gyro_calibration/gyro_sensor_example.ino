#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// 캘리브레이션 값
float calX = 0;
float calY = 0;

// 캘리브레이션 샘플 수
const int numSamples = 100;

// 경고 상태 변수 (예: 자세가 나쁠 때 1, 아니면 0)
int warningStatus = 0;

// 사용자에게 캘리브레이션을 진행하라고 안내하는 시간 (예: 5초)
const unsigned long calibrationDelay = 5000;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  Serial.println("MPU6050 초기화 중...");
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 연결 성공!");
  } else {
    Serial.println("MPU6050 연결 실패!");
    while (1);
  }
  
  // 사용자가 바른 자세를 취할 시간을 제공
  Serial.println("바른 자세를 취해주세요. 캘리브레이션 진행까지 5초 대기...");
  delay(calibrationDelay);
  
  // 캘리브레이션: numSamples 개의 값을 읽어 평균을 구함
  float sumX = 0;
  float sumY = 0;
  Serial.println("캘리브레이션 진행 중...");
  for (int i = 0; i < numSamples; i++) {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // 여기서는 예시로 X축과 Y축의 가속도 값(또는 기울기 측정을 위한 값)을 사용
    sumX += ax;
    sumY += ay;
    
    delay(10); // 샘플 간 짧은 딜레이
  }
  calX = sumX / numSamples;
  calY = sumY / numSamples;
  
  Serial.print("캘리브레이션 완료. 기준값 -> X: ");
  Serial.print(calX);
  Serial.print(", Y: ");
  Serial.println(calY);
  
  // 초기 경고 상태 설정
  warningStatus = 0;
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // 예시로 임계치(threshold)를 적용: 캘리브레이션 값보다 5% 이상 낮을 경우 경고
  float thresholdX = calX * 0.95;
  float thresholdY = calY * 0.95;
  
  if (ax < thresholdX || ay < thresholdY) {
    if (warningStatus == 0) {  // 처음 경고 발생 시
      Serial.println("경고: 자세가 좋지 않습니다! 바른 자세로 돌아가세요.");
      warningStatus = 1;
      // 추가로 변수 변경이나 LED, 부저 작동 등을 여기에 추가
    }
  } else {
    // 정상 자세면 경고 상태 리셋
    if (warningStatus == 1) {
      Serial.println("자세가 회복되었습니다.");
      warningStatus = 0;
    }
  }
  
  delay(200);  // 루프 딜레이 (필요에 따라 조절)
}
