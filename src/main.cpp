#include <Arduino.h>

#define PIR_PIN 8
#define BUZZER_PIN 4
#define RED_PIN 7
#define GREEN_PIN 6
#define BLUE_PIN 5

enum AlarmState
{
  IDLE,
  ALARM_ON,
  ALARM_OFF
};
AlarmState state = IDLE;

unsigned long lastTriggerTime = 0;
unsigned long lastStepTime = 0;
short pulseCount = 0;

void setup()
{
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Початковий стан — вимкнено
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 255);

  randomSeed(analogRead(A0));
}

void loop()
{
  unsigned long curTime = millis();

  switch (state)
  {
  case IDLE:
    if (digitalRead(PIR_PIN) == HIGH && (curTime - lastTriggerTime >= 1000))
    {
      lastTriggerTime = curTime;
      pulseCount = 0;
      state = ALARM_ON;
      lastStepTime = curTime;

      tone(BUZZER_PIN, 4000);
      analogWrite(RED_PIN, 255 - random(50, 256));
      analogWrite(GREEN_PIN, 255 - random(50, 256));
      analogWrite(BLUE_PIN, 255 - random(50, 256));
    }
    break;

  case ALARM_ON:
    if (curTime - lastStepTime >= 150)
    {
      lastStepTime = curTime;
      noTone(BUZZER_PIN);
      // Вимикаємо світло (подаємо 255)
      analogWrite(RED_PIN, 255);
      analogWrite(GREEN_PIN, 255);
      analogWrite(BLUE_PIN, 255);
      state = ALARM_OFF;
    }
    break;

  case ALARM_OFF:
    if (curTime - lastStepTime >= 150)
    {
      lastStepTime = curTime;
      pulseCount++;

      if (pulseCount < 7)
      {
        tone(BUZZER_PIN, 4000);
        analogWrite(RED_PIN, 255 - random(50, 256));
        analogWrite(GREEN_PIN, 255 - random(50, 256));
        analogWrite(BLUE_PIN, 255 - random(50, 256));
        state = ALARM_ON;
      }
      else
      {
        state = IDLE;
      }
    }
    break;
  }
}