#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Servo servo;

Servo servo2;

Servo gateservo;

int buzzer = 5;
const int buttonpin= 4;
const int motor = 3;

int buttonstate = 0;
int gatebuttonstate = 0;
const int gatebutton = 6;



LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600); 

  Wire.begin();
  servo.attach(10);
  servo2.attach(9);
  gateservo.attach(7);  
 
  lcd.init();
  lcd.backlight();
  pinMode(buzzer,OUTPUT);
  pinMode(buttonpin, INPUT_PULLUP);
  pinMode(motor, OUTPUT);
  pinMode(gatebutton, INPUT_PULLUP);

}

void loop() {
  noTone(buzzer);


  if (Serial.available()) {
 
    String data = Serial.readStringUntil('\n');
    data.trim();
    Serial.println(data);

    // Parse the received data
    String containerName = getValue(data, ',', 0);
    int pillsToBeTaken = getValue(data, ',', 1).toInt();
    String date = getValue(data, ',', 2);
    String time = getValue(data, ',', 3);
    String identifier = getValue(data, ',', 4);
    String container = getValue(data, ',', 5);


    lcd.clear();
    lcd.print(" Pill-Spenser  ");
    lcd.setCursor (0,1); 
    lcd.print(String("    WAXJACS   "));  

    if (String(identifier) == "True") {

      lcd.clear();
      lcd.print("Pill Name: ");
      lcd.setCursor (0,1); 
      lcd.print(containerName); 

      if (String(container) == "Container1" && String(container) == "Container2") {
        tone(buzzer, 392, 5000);
        moveServoMotor(pillsToBeTaken, servo);   
        moveServoMotor(pillsToBeTaken, servo2); 
      } else if (String(container) == "Container1") {
        tone(buzzer, 392, 5000);
        moveServoMotor(pillsToBeTaken, servo);   
      } else if (String(container) == "Container2") {
        tone(buzzer, 392, 5000);
        moveServoMotor(pillsToBeTaken, servo2);  // Move Servo2 for Container2
      }
    } else {
      Serial.println("None");
      noTone(buzzer);
    }
  }

  int buttonstate = digitalRead(buttonpin);
  if (buttonstate == LOW){
    digitalWrite(motor, HIGH);
  } else {
    digitalWrite(motor, LOW);
  }

  int gatebuttonstate = digitalRead(gatebutton);
   if (gatebuttonstate == LOW){
    Serial.write("Button pressed");
    gateservo.write(180);
    delay(2000);
    gateservo.write(0);
    delay(1000);
  } else {
    gateservo.write(0);
  }



servo.write(0);
servo2.write(0);

}


void moveServoMotor(int numPillsToBeTaken, Servo &servoMotor) {
  for (int i = 0; i < numPillsToBeTaken; ++i) {
    int angle = 60;
    servoMotor.write(angle);
    delay(1000);
    servoMotor.write(0);
    delay(1000);
  }
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
