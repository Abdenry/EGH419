#include <DS3231.h>
#include <Wire.h>
#include <time.h>
DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void printDate(void){
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

void loop() {
  printDate();

  delay(1000);
}