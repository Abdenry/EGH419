#include <DS3231.h>
#include <Wire.h>
#include <time.h>
DS3231 rtc;
bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
  // // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  // rtc.setClockMode(false);
  // rtc.setHour(13);
  // rtc.setMinute(32);
  // rtc.setSecond(0);
  // rtc.setDoW(3);
  // rtc.setDate(26);
  // rtc.setMonth(4);
  // rtc.setYear(23);
}

void loop() {
  // put your main code here, to run repeatedly:
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
  Serial.println();

  delay(1000);

}
