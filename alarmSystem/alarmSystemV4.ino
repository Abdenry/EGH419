#include <DS3231.h>
#include <Wire.h>
#include <time.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Parameters needed for setting time and alarms for DS3231
DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;

// Parameters for setting the alarm
bool alarmH12 = false;        // (true) if hour is 1 - 12, (false) if hour is 00 - 23
byte alarmHour = 15;          // 1-12 if 12-hour mode, 0-23 if 24-hour mode
bool alarmPM = true;          // (true) if 12-hour time is PM, (false) if AM
byte alarmMinute = 0;         // 00 - 59
byte alarmSecond = 0;         // 00 - 59
byte alarmDay = 5;            // 1-7 if day of week, 1-31 if date in month
bool alarmIsDay = true;       // (true) if alarmDay is a day of the week, (false) if date in month
byte alarmBits = 0b00001110;  // a bitfield

// Connected pins of the alarm and alarm button
int alarmButtonPin = 2;
int alarmPin = 7;
bool exitAlarmSettings = false;


void printDate(void) {

  Serial.print(rtc.getYear());
  Serial.print("-");
  Serial.print(rtc.getMonth(century));
  Serial.print("-");
  Serial.print(rtc.getDate());
  Serial.print("\t");
  Serial.print(rtc.getHour(h12Flag, pmFlag));
  Serial.print("-");
  Serial.print(rtc.getMinute());
  Serial.print("-");
  Serial.print(rtc.getSecond());
  Serial.print(" ");
  switch (rtc.getDoW()) {
    case 1:
      Serial.print("Monday");
      break;
    case 2:
      Serial.print("Tuesday");
      break;
    case 3:
      Serial.print("Wednesday");
      break;
    case 4:
      Serial.print("Thursday");
      break;
    case 5:
      Serial.print("Friday");
      break;
    case 6:
      Serial.print("Saturday");
      break;
    case 7:
      Serial.print("Sunday");
      break;
  }
  Serial.println();
}

void setAlarm(void) {
  rtc.setA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmIsDay, alarmH12, alarmPM);
  rtc.turnOnAlarm(1);
  rtc.checkIfAlarm(1);
  rtc.checkIfAlarm(2);
}

void checkAlarm(void) {
  if (rtc.checkIfAlarm(1, false)) {
    PORTD |= (1 << alarmPin);
    rtc.checkIfAlarm(1, true);
  }
}

void clearAlarm(void) {
  if (PIND & (1 << 2)) {
    PORTD &= ~(1 << alarmPin);
  }
}

// check to see if any buttons for the alarm are pressed
void checkButtons(void) {
  bool saveAndExit = false;
  byte hourTemp = 0;
  byte minTemp = 0;
  byte dayTemp = 1;
  byte secTemp = 0;
  // If buttons 1 and 3 are pressed enter alarm settings
  if ((PIND & (1 << 2)) && (PIND & (1 << 3))) {
    Serial.println("Changing the alarm settings...");
    exitAlarmSettings = false;
    delay(1000);
    while (!exitAlarmSettings) {

      // If buttons 1 and 3 are pressed exit alarm settings
      if ((PIND & (1 << 2)) && (PIND & (1 << 4))) {
        Serial.print("Alarm settings are: ");
        Serial.print(dayTemp);
        Serial.print("-");
        Serial.print(hourTemp);
        Serial.print("-");
        Serial.print(minTemp);
        Serial.print("-");
        Serial.print(secTemp);
        Serial.println(" (DD-HH-MM-SS)");
        Serial.println("Do you want to save and exit?");
        // if button 1 is pressed save the new alarm settings
        // if button 2 is pressed discard the new alarm settings
        while (!exitAlarmSettings) {
          delay(1000);
          if (PIND & (1 << 2) && ~(PIND & (1 << 4))) {
            Serial.println("Alarm settings saved...");
            alarmDay = dayTemp;
            alarmHour = hourTemp;
            alarmMinute = minTemp;
            alarmSecond = secTemp;
            setAlarm();
            exitAlarmSettings = true;
          }
          if (!(PIND & (1 << 2)) && (PIND & (1 << 4))) {
            Serial.println("Alarm settings discarded...");
            exitAlarmSettings = true;
          }
        }
        break;
      }


      // if only button 1 is pressed then increase the alarm hour
      if ((PIND & (1 << 2)) && !(PIND & (1 << 4))) {
        if (hourTemp == 23) {
          hourTemp = 0;
        } else {
          hourTemp = hourTemp + 1;
        }
        Serial.print("Alarm hour increased to: ");
        Serial.println(hourTemp);
      }


      // if only button 2 is pressed then increase the alarm minute
      if ((PIND & (1 << 3)) && !(PIND & (1<<4))) {
        if (minTemp == 59) {
          minTemp = 0;
        } else {
          minTemp = minTemp + 1;
        }
        Serial.print("Alarm minute increased to: ");
        Serial.println(minTemp);
      }


      // if only button 3 is pressed then increase the alarm day
      if ((PIND & (1 << 4)) && !(PIND & (1 << 2)) && !(PIND & (1<<3))) {
        if (dayTemp == 7) {
          dayTemp = 1;
        } else {
          dayTemp = dayTemp + 1;
        }
        Serial.print("Alarm day changed to: ");
        switch (dayTemp) {
          case 1:
            Serial.println("Monday");
            break;
          case 2:
            Serial.println("Tuesday");
            break;
          case 3:
            Serial.println("Wednesday");
            break;
          case 4:
            Serial.println("Thursday");
            break;
          case 5:
            Serial.println("Friday");
            break;
          case 6:
            Serial.println("Saturday");
            break;
          case 7:
            Serial.println("Sunday");
            break;
        }
      }

      // if buttons 2 and 3 are pressed increase the alarm seconds
      if ((PIND & (1 << 3)) && (PIND & (1 << 4))) {
        if (secTemp == 59) {
          secTemp = 0;
        } else {
          secTemp = secTemp + 1;
        }
        Serial.print("Alarm second increase to: ");
        Serial.println(secTemp);
      }

      delay(1000);
    }
  }
}


void setup() {
  DDRD |= (1 << 7);
  DDRD |= (1 << 6);
  Serial.begin(115200);
  Wire.begin();
  setAlarm();
}

void loop() {
  printDate();
  checkAlarm();
  clearAlarm();
  checkButtons();
  delay(1000);
}
