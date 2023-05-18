#include <DS3231.h>
#include <Wire.h>
#include <time.h>
#include <avr/io.h>

// Parameters needed for setting time and alarms for DS3231
DS3231 rtc;
bool h12Flag;
bool pmFlag;

// Connected pins of the alarm and alarm button
int alarmButtonPin = 2;
int alarmPin = 7;
byte currentTime[3];

/* checkTime: Checks the current time of the DS3231 and stores the Day, Hour and Minute
  into a supplied byte array.

  Params: an array of size 3 and type byte. 
  Returns: void
*/
byte *checkTime(byte array[3]){
  array[0] = rtc.getDoW();
  array[1] = rtc.getHour(h12Flag, pmFlag);
  array[2] = rtc.getMinute();
}

/* clearAlarm: Checks to see if the button for clearing the alarm is pressed. 
  If it is pressed then the pin that the alarm is connected to is cleared (turned off).
  If the button is not pressed nothing happens.

  Params: void
  returns: void

  Note: The data register will need to be changed depending on what pin the alarm and button is connected to.
*/
void clearAlarm(void) {
  if (PIND & (1 << alarmButtonPin)) {
    PORTD &= ~(1 << alarmPin);
  }
}

/* setTime: sets the Day, Hour and Minute of the DS3231 based on the supplied values
  Params: 
  int setDay between 1-7, where 1 is the first day of the week and 7 the last. 
  Whatever day is considered the start of the week is upto the programmer.
  int setHour between 0 - 23 (24-hour time is used for this project)
  int setMinute between 0 - 59

  returns: void
*/
void setTime(int setDay, int setHour, int setMinute){
  rtc.setDoW(setDay);
  rtc.setHour(setHour);
  rtc.setMinute(setMinute);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  // printDate();
  checkTime(currentTime);
  Serial.print(currentTime[1]);
  Serial.print(":");
  Serial.println(currentTime[2]);
  clearAlarm();
  delay(1000);
}
