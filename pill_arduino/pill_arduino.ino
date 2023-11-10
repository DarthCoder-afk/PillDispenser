#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



// Define the stepper motor parameters
const int stepsPerRevolution = 400;  // Change according to your stepper motor
const int stepperPin1 = 2;           // Change to the desired stepper motor pin
const int stepperPin2 = 3;           // Change to the desired stepper motor pin
const int stepperPin3 = 4;           // Change to the desired stepper motor pin
const int stepperPin4 = 5;           // Change to the desired stepper motor pin
const int motorpin = 10;

const int stepperPin5 = 6;           // Change to the desired stepper motor pin
const int stepperPin6 = 7;           // Change to the desired stepper motor pin
const int stepperPin7 = 8;           // Change to the desired stepper motor pin
const int stepperPin8 = 9;           // Change to the desired stepper motor pin

Stepper stepper(stepsPerRevolution, stepperPin1, stepperPin2, stepperPin3, stepperPin4);


Stepper stepper2(stepsPerRevolution, stepperPin5, stepperPin6, stepperPin7, stepperPin8);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  // Initialize the serial communication
  Serial.begin(9600); 

  Wire.begin();

  // Set the speed of the stepper motor
  stepper.setSpeed(60);  // Adjust the speed as required
  stepper2.setSpeed(60);

  pinMode(motorpin, OUTPUT);

  // initialize the LCD, 
	lcd.begin();
 
	// Turn on the blacklight and print a message.
	lcd.backlight();
  // Robojax code for LCD with I2C

}

void loop() {
  digitalWrite(motorpin,LOW);

  if (Serial.available()) {
    // Read the received data from the NodeMCU


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

    //start of loop Robojax code for LCD with I2C
    lcd.clear();
    lcd.print(" Pill Dispenser  ");
    lcd.setCursor (0,1); // go to start of 2nd line 
    lcd.print(String("    WAXJACS   "));  
          


    if (String(identifier) == "True") {
      // Move the stepper motor based on the number of pills to be taken
       
      //Serial.println(pillsToBeTaken); 
      
        lcd.clear();
        lcd.print("Pill Name: ");
        lcd.setCursor (0,1); // go to start of 2nd line 
        lcd.print(containerName);

      
      if (String(container) == "Container1" && String(container) == "Container2"){

        moveStepperMotor(pillsToBeTaken);
        moveStepperMotor2(pillsToBeTaken);
        digitalWrite(motorpin, HIGH);
        delay(500);
    
      } else if (String(container) == "Container1"){

        moveStepperMotor(pillsToBeTaken);
        digitalWrite(motorpin, HIGH);
        delay(500);
    
      } else if (String(container) == "Container2") {
        moveStepperMotor2(pillsToBeTaken);
        digitalWrite(motorpin, HIGH);
        delay(500);
        

      }

      //// Print the container name, number of pills to be taken, and identifier
      Serial.println("----------------------------------------------");
      Serial.print("Container Name: ");
      Serial.println(containerName);
      Serial.print("Pills to be Taken: "); 
      Serial.println(pillsToBeTaken); 
      Serial.println("----------------------------------------------");
      // Delay for 1 minute
      delay(10000);
      
    } else {
      digitalWrite(motorpin, LOW);
      Serial.println("None");
      // Delay for 1 minute 
    }
    
  }
}


void moveStepperMotor(int numPillsToBeTaken) {
  //int numPillsToBeTaken = 1;
  // Calculate the total number of steps to move the desired number of pills
  int totalSteps = numPillsToBeTaken * stepsPerRevolution;

  // Move the stepper motor
  stepper.step(totalSteps);
}

void moveStepperMotor2(int numPillsToBeTaken) {
  //int numPillsToBeTaken = 1;
  // Calculate the total number of steps to move the desired number of pills
  int totalSteps = numPillsToBeTaken * stepsPerRevolution;

  // Move the stepper motor
  stepper2.step(totalSteps);
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
