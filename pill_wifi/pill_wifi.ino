#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h> 



#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

//#define FIREBASE_HOST "pill-dispenser-9af61-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "X3nvG1lXfMezZeK5RT7HHjunhfnAGYTyT31Kg0tX"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Define the NTP server and time offset
const char* ntpServer = "asia.pool.ntp.org";
const long timeZoneOffset = 8 * 3600;  // Philippine time offset (in seconds)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, timeZoneOffset);
int ledPin = D4;
int wireSDAPin = D1;  
int wireSCLPin = D2;  
 
int irpin = D3;
int count = 0;

struct Data {
  String containerName; 
  String timeStr;
  int day;
  int hour;
  int minute;
  int month;
  int pillsToBeTaken;
  int year;
};

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600); // Hardware Serial interface (UART) on NodeMCU (TX: GPIO1, RX: GPIO3)

  setTime(0);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Initialize NTPClient
  timeClient.begin();
  timeClient.setTimeOffset(0); // Optionally, set the time offset in seconds if required.
  timeClient.update();

  pinMode(irpin, INPUT);
}

void loop() {
  // Update current time
  timeClient.update();

  String counter1 = Firebase.getString("container1_pill_number");
  count = counter1.toInt();  

  if (digitalRead(irpin) == LOW) {
    count++;
    Firebase.setString("container1_pill_number", String(count++));
    delay(200); // Adjust this delay based on your requirements
  }
    Serial.print("Container 1 Pill Number:");
    Serial.println(count++);

  // Retrieve data from Firebase
  String result = Firebase.getString("data");

  Serial.println(result);
  
  String result_con2 = Firebase.getString("data2");

  Serial.println(result_con2);

  // Parse the data into variables
  Data data = parseData(result);
  Data data_con2 = parseData(result_con2);

  // Get the current date and time
  //unsigned long epochTime = timeClient.getEpochTime() + 24 * 3600; // Add 1 day (24 hours) in seconds

  unsigned long epochTime = timeClient.getEpochTime();
  int currentYear = year(epochTime);
  int currentMonth = month(epochTime);
  int currentDay = day(epochTime);
  int currentHour = (hour(epochTime) + 8) % 24;
  int currentMinute = minute(epochTime);
  int currentSecond = second(epochTime);

// Format the hour with leading zero if necessary
  String New_currentDay;
  if (currentDay < 10) {
    New_currentDay = "0" + String(currentDay);
   } else {
    New_currentDay = String(currentDay);
  }

  // Format the hour with leading zero if necessary
  String New_currentMonth;
  if (currentMonth < 10) {
    New_currentMonth = "0" + String(currentMonth);
   } else {
    New_currentMonth = String(currentMonth);
  }
  
  // Format the hour with leading zero if necessary
  String New_hourString;
  if (currentHour < 10) {
    New_hourString = "0" + String(currentHour);
   } else {
    New_hourString = String(currentHour);
  }

  String New_currentTime;
  if (currentMinute < 10) {
    //currentTime1 = "0" + String(currentDay);
    New_currentTime = "0" + String(currentMinute);
  } else {
    New_currentTime = String(currentMinute);
  }

  String New_dataday;
  if (data.day < 10) {
    //currentTime1 = "0" + String(currentDay);
    New_dataday = "0" + String(data.day);
  } else {
    New_dataday = String(data.day);
  }

  String New_dataday2;
  if (data_con2.day < 10) {
    //currentTime1 = "0" + String(currentDay);
    New_dataday2 = "0" + String(data_con2.day);
  } else {
    New_dataday2 = String(data_con2.day);
  }

  
  String New_datamonth;
  if (data.month < 10) {
    //currentTime1 = "0" + String(currentDay);
    New_datamonth = "0" + String(data.month);
  } else {
    New_datamonth = String(data.month);
  }

   String New_datamonth2;
  if (data_con2.month < 10) {
    //currentTime1 = "0" + String(currentDay);
    New_datamonth2 = "0" + String(data_con2.month);
  } else {
    New_datamonth2 = String(data_con2.month);
  }

  String currentTime1 = String(New_hourString) + ":" + String(New_currentTime);
  int currentDate1 = int(currentDay);

  //Serial.print(currentTime1);

  String schedDates = String(New_dataday) + String(New_datamonth) + String(data.year);
  String currentDates = String(New_currentDay) + String(New_currentMonth) + String(currentYear);

  //Serial.println(String(schedDates));
  Serial.println(String(currentDates));
  //Serial.println(data.timeStr);
  //Serial.println(currentTime1);
  
  String schedDates_con2 = String(New_dataday2) + String(New_datamonth2) + String(data_con2.year);

  //Serial.println(schedDates_con2);
  //Serial.println(currentDates);
  //Serial.println(data_con2.timeStr);
  //Serial.println(currentTime1);
  

  // Compare the retrieved date and time with the current date and time (adjusted for Philippine time)
      bool container1Match = (String(schedDates) == String(currentDates) &&
                              String(data.timeStr) == String(currentTime1));

      bool container2Match = (String(schedDates_con2) == String(currentDates) && 
                              String(data_con2.timeStr) == String(currentTime1));

      // Check if both containers have a match
      if (container1Match && container2Match) {
          Serial.println(String(result) + "," + "True" + "," + "Container1");
          Serial.println(String(result_con2) + "," + "True" + "," + "Container2");

          // Turn on the LED
          digitalWrite(ledPin, HIGH);

          // Delay for 1 minute
          delay(60000);
      } else if (container1Match) {
          Serial.println(String(result) + "," + "True" + "," + "Container1");

          // Turn on the LED
          digitalWrite(ledPin, HIGH);

          // Delay for 1 minute
          delay(60000);
      } else if (container2Match) {
          Serial.println(String(result_con2) + "," + "True" + "," + "Container2");

          // Turn on the LED
          digitalWrite(ledPin, HIGH);

          // Delay for 1 minute
          delay(60000);
      } else {
          Serial.println("False");
          
          // Turn off the LED
          digitalWrite(ledPin, LOW);
      }

      delay(5000);
}


Data parseData(const String& dataString) {
  Data data;

  // Split the dataString into individual values
  String containerName = getValue(dataString, ',', 0);
  data.containerName = containerName;

  String pillsToBeTakenStr = getValue(dataString, ',', 1);
  data.pillsToBeTaken = pillsToBeTakenStr.toInt();

  String dateStr = getValue(dataString, ',', 2);
  sscanf(dateStr.c_str(), "%d/%d/%d", &(data.month), &(data.day), &(data.year));

  String timeStr = getValue(dataString, ',', 3);
  timeStr.trim();
  data.timeStr = timeStr;
  

  return data;
}

String getValue(const String& data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";

}
