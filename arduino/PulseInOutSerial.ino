/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 Modified 4 Sep 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 */
// constants won't change. Used here to 
// set pin numbers:
const int ledPin =  13;      // the number of the LED pin

// Variables will change:
int blinkActive = HIGH;
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

const int pulsePin0 = 2; // I/O pulse in pin
const int pulsePin1 = 3; // I/O pulse in pin

int delayMil = 100;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int run = LOW;

int inChar = 0;         // incoming serial byte
String inString = "";    // string to hold input
String inCmd = "";    // string to hold command input

//COMMAND LIST
#define RESET 0
#define BLINK_ON  1
#define BLINK_OFF 2
#define BLINK_MIL 3
#define SET 4
#define NOP 5
#define STATUS 6
#define GET 7
#define START 8
#define STOP 9
#define DELAY_MIL 10

int lastCommand = NOP;
int lastValue = 0;
int offset=0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT);      
  pinMode(pulsePin0, INPUT);
  pinMode(pulsePin1, INPUT);
}

void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char 
      // and add it to the string:
      inString += (char)inChar; 
    }
    else{
      if(inChar != '\n')
        inCmd += (char)inChar; 
    }
    // if you get a newline, print the string,
    // then the string's value:
    if (inChar == '\n') {
      // parse the command
      if(inCmd=="RESET"){
        lastCommand = RESET;
        offset = sensorValue;
        lastValue = 0;  
        outputValue = lastValue;
        Serial.println("RESET");
      }
      else if(inCmd=="SET"){
        lastCommand = SET;
        lastValue = inString.toInt();  
        // map it to the range of the analog out:
        //outputValue = map(lastValue, 0, 1023, 0, 255);  
        if(lastValue>255)
          outputValue = 255;
        else
          outputValue = lastValue;
        Serial.print("SET ");
        Serial.println(lastValue);
      }
      else if(inCmd=="BLINK_MIL"){
        lastCommand = BLINK_MIL;
        lastValue = inString.toInt();  
        interval = lastValue;
        Serial.print("BLINK_MIL ");
        Serial.println(interval);
      }      
      else if(inCmd=="BLINK_OFF"){
        lastCommand = BLINK_OFF;
        blinkActive = LOW;
        Serial.println("BLINK_OFF");
      }
      else if(inCmd=="BLINK_ON"){
        lastCommand = BLINK_ON;
        blinkActive = HIGH;
        Serial.println("BLINK_ON");
      }
      else if(inCmd=="NOP"){
        lastCommand = NOP;
        Serial.println("NOP");
      }
      else if(inCmd=="STATUS"){
        lastCommand = STATUS;
        Serial.print("STATUS ");
        Serial.print(millis());        
        Serial.print(" interval:");
        Serial.print(interval);        
        Serial.print(" sensor:");
        Serial.print(sensorValue);        
        Serial.print(" output:");
        Serial.println(outputValue);        
      }
      else if(inCmd=="GET"){
        lastCommand = GET;
        Serial.print("sensor: ");
        Serial.print(sensorValue);    
        Serial.print(" " );                       
        Serial.println(millis()); 
      }
      else if(inCmd=="START"){
        lastCommand = START;
        run = HIGH;
        Serial.println("START");
      }
      else if(inCmd=="STOP"){
        lastCommand = STOP;
        run = LOW;
        Serial.println("STOP");
      }
      else if(inCmd=="DELAY_MIL"){
        lastCommand = DELAY_MIL;
        lastValue = inString.toInt();  
        delayMil = lastValue;
        Serial.print("DELAY_MIL ");
        Serial.println(interval);
      }
      else {
        Serial.print("ERROR ");
        Serial.println(inCmd);
      }
      // acknowledge the command
/*        Serial.print("lastCommand: ");
        Serial.print(lastCommand);
        Serial.print(" lastValue:");
        Serial.print(lastValue);
*/      
      // clear the string for new input:
      inString = ""; 
      inCmd = "";
    }
  }
  // read the analog in value:
  //sensorValue = analogRead(analogInPin);            
  sensorValue = pulseIn(pulsePin1, HIGH);
  // change the analog out value:
  // analogWrite(analogOutPin, outputValue);           

  // print the results to the serial monitor:
  if(run){
    Serial.print("sensor = " );                       
    Serial.print(sensorValue-offset); 
    Serial.print(" " );                       
    Serial.println(millis()); 
    
    // wait 10 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:
    delay(delayMil);          

  }
  
   // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if(blinkActive)
      if (ledState == LOW)
        ledState = HIGH;
      else
        ledState = LOW;
    else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }  
}
