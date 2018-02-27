#include "definitions.h"

#define MOTOR_1 16
#define MOTOR_2 5
#define MOTOR_3 4

#define MOTOR_4 14
#define MOTOR_5 12
#define MOTOR_6 13

void setup() {
  delay(100);
  Serial.begin(115200);
  Serial.println("Starting up");

  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);

  pinMode(MOTOR_4, OUTPUT);
  pinMode(MOTOR_5, OUTPUT);
  pinMode(MOTOR_6, OUTPUT);

  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);

  digitalWrite(MOTOR_4, LOW);
  digitalWrite(MOTOR_5, LOW);
  digitalWrite(MOTOR_6, LOW);

  setup_tables();
  setup_wifi();
}

void loop() {
  wifi_loop();
}

void pump(uint8_t motor_index) {
  digitalWrite(MOTOR_1, motor_index == 0 ? HIGH : LOW);
  digitalWrite(MOTOR_2, motor_index == 1 ? HIGH : LOW);
  digitalWrite(MOTOR_3, motor_index == 2 ? HIGH : LOW);
  digitalWrite(MOTOR_4, motor_index == 3 ? HIGH : LOW);
  digitalWrite(MOTOR_5, motor_index == 4 ? HIGH : LOW);
  digitalWrite(MOTOR_6, motor_index == 5 ? HIGH : LOW);
}

