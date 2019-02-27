//Importing libraries into project namespace
#include <time.h>

//Declare global variables

//2 dimentional array for 7 segment display
int num_array[12][8] = {  { 1, 1, 1, 0, 1, 1, 1, 0 }, // 0
  { 0, 1, 0, 0, 0, 0, 1 , 0}, // 1
  { 1, 1, 0, 1, 1, 1, 0 , 0}, // 2
  { 1, 1, 0, 1, 0, 1, 1 , 0}, // 3
  { 0, 1, 1, 1, 0, 0, 1 , 0}, // 4
  { 1, 0, 1, 1, 0, 1, 1 , 0}, // 5
  { 1, 0, 1, 1, 1, 1, 1 , 0}, // 6
  { 1, 1, 1, 0, 0, 0, 1 , 0}, // 7
  { 1, 1, 1, 1, 1, 1, 1 , 0}, // 8
  { 1, 1, 1, 1, 0, 1, 1 , 0}, // 9
  { 0, 0, 0, 0, 0, 0, 0 , 1}, // .
  { 0, 0, 0, 1, 0, 0, 0 , 0}  // -
};


//Analogue variables (LDR's, Ultrasonic sensor, etc)
int ldr1 = A0;
int ldr2 = A1;
int Switch = A2;

//Digital variable (LED's,
int ldrLed1 = 2;
int ldrLed2 = 3;
int roomLight = 4;
int ledTop = 5;
int ledTopRight = 6;
int ledTopLeft = 7;
int ledMiddle = 8;
int ledBotLeft = 9;
int ledBot = 10;
int ledBotRight = 11;
int ledPeriod = 12;
int ledSwitch = 13;

//Non-pin variables
int Delay = 100;
int debug = 0;
int tolerance = 5;
int lastLDR1Avg = 0;
int lastLDR2Avg = 0;
unsigned long ldr1TriggerTime;
unsigned long ldr2TriggerTime;
bool ldr1Triggered = false;
bool ldr2Triggered = false;
int noOfPeopleInRoom = 0;
boolean ledBooted = false;
int resetDelay = 500;
int pinStartOf7SegmentDisplay = 5;


//Setup method. Ran when Arduino first boots up
void setup() {

  /*
     ALGORITHM
     1. Set the mode of the pins to input and output. This ensures there is no dim LED's.
     2. Set the default values for the LED's upon boot
     3. Begin the Serial communication at 9600 bits per second
     4. Set default value of the seven segment display to 0
  */

  //1. Set the mode of the pins to input and output. This ensures there is no dim LED's.
  pinMode(roomLight, OUTPUT);
  pinMode(ldrLed1, OUTPUT);
  pinMode(ldrLed2, OUTPUT);
  pinMode(ledTop, OUTPUT);
  pinMode(ledTopLeft, OUTPUT);
  pinMode(ledTopRight, OUTPUT);
  pinMode(ledMiddle, OUTPUT);
  pinMode(ledBotLeft, OUTPUT);
  pinMode(ledBotRight, OUTPUT);
  pinMode(ledBot, OUTPUT);
  pinMode(ledPeriod, OUTPUT);
  pinMode(ledSwitch, OUTPUT);


  //2. Set the default values for the LED's upon boot
  digitalWrite(roomLight, false);
  digitalWrite(ldrLed1, true);
  digitalWrite(ldrLed2, true);

  //3. Begin the Serial communication at 9600 bits per second
  Serial.begin(9600);

  //4. Set default value of the seven segment display to 0
  pinStartOf7SegmentDisplay = 5;
  for (int j = 0; j < 8; j++)
  {
    digitalWrite(pinStartOf7SegmentDisplay, num_array[0][j]);
    pinStartOf7SegmentDisplay++;
  }
}

void loop() {
  /*
     1. Variable declaration
     2. Collect the LDR values through iteration and total them over a specific period
     3. Average out the total value by dividing by the number of values we collected
     4. Adjust the range of the LDR values from 0 to (2^n)-1 range to a manageable scale such as 0 - 100
     5. Test if the Average LDR1 value has dropped significantly (outside the range of tolerance) since the last reading
     5.1 If outside the range of tolerance, record trigger time in milliseconds and record that LDR1 has been triggered
     6. Test if the Average LDR2 value has dropped significantly (outside the range of tolerance) since the last reading
     6.1 If outside the range of tolerance, record trigger time in milliseconds and record that LDR2 has been triggered
     7. Test if both of the LDR's have been triggered
     7.1 If both LDR's are triggered, test if LDR1's trigger time is less than LDR2's time
     7.1.1 If LDR1's trigger is less than LDR2's time, then LDR1 was triggered first therefore we increment the number of people in the room
     7.2 If LDR1's trigger is not less than LDR2's time, then LDR2 was triggered first therefore we decrement the number of people in the room
     7.2.1 Test if the number of people in the room is less than 0 (in case system was started with people inside)
     7.2.1.1 If the number of people is less than 0, set it to 0
     8. Test if number of people in the room is equal to 0
     8.1 If equal to 0, set the room light to LOW
     8.2 If not equal to 0, set the room light to HIGH
     8.3 set the 7 segment display to the value of people in the room
     9. Reset the trigger time and the flag for triggered for each of the LDR's
     10. Test if the trigger time for LDR1 added with the reset delay is more than the currently millisecond time and if LDR1 is marked as triggered
     10.1 Set the trigger time to 0 and triggered flag to false
     11. Test if the trigger time for LDR2 added with the reset delay is more than the currently millisecond time and if LDR2 is marked as triggered
     11.1 Set the trigger time to 0 and triggered flag to false
     12. Set the new average we gained from the LDR's into the previous variable in order to test upon the next iteration
  */

  //This allows us to on-the-fly debug when
  //we have found an issue
  //   if(Serial.available())
  //   {
  //    String input = (String)Serial.readString();
  //
  //    if(input == "Debug")
  //    {
  //      Serial.println("0 = Debug Off");
  //      Serial.println("1 = Basic Debug");
  //      Serial.println("2 = Advanced Debug");
  //
  //      debug++;
  //
  //      if(debug > 2)
  //      {
  //        debug = 0;
  //      }
  //
  //      Serial.println("Debug is now set at: " + (String)debug);
  //    }
  //   }





  //1. Variable declaration
  long totalLDR1 = 0;
  long totalLDR2 = 0;



  //2. Collect the LDR values through iteration and total them over a specific period
  for (int i = 0; i < Delay; i++)
  {

    //Use a compound assignment operator to total up the LDR values
    //We read the analogue signal from the Analogue input pins specified in the global variable
    totalLDR1 += analogRead(ldr1);
    totalLDR2 += analogRead(ldr2);

    //We delay each iteration in order to get values from a range of time as processing speed
    //would mean that the for loop would get it in a shorter time duration
    delay(1);
  }

  //3. Average out the total value by dividing by the number of values we collected
  int avgLDR1 = totalLDR1 / Delay;
  int avgLDR2 = totalLDR2 / Delay;

  //4. Adjust the range of the LDR values from 0 to (2^n)-1 range to a manageable scale such as 0 - 100
  int newAvgLDR1 = map(avgLDR1, 0, 1023, 0, 100);
  int newAvgLDR2 = map(avgLDR2, 0, 1023, 0, 100);

  if (debug == 2)
  {
    Serial.println("Current LDR1 value: " + (String)newAvgLDR1 + " Previous LDR 1 value: " + (String)lastLDR1Avg);
    Serial.println("Current LDR2 value: " + (String)newAvgLDR2 + " Previous LDR 2 value: " + (String)lastLDR2Avg);
  }

  //5. Test if the Average LDR1 value has dropped significantly (outside the range of tolerance) since the last reading
  if (newAvgLDR1 + tolerance < lastLDR1Avg)
  {
    if (debug > 0)
    {
      Serial.println("LDR 1 has been triggered! " + (String)newAvgLDR1 + " + " + (String)tolerance + " < " + (String)lastLDR1Avg);
    }
    //5.1 If outside the range of tolerance, record trigger time in milliseconds and record that LDR1 has been triggered
    ldr1TriggerTime = millis();
    ldr1Triggered = true;
  }

  delay(1);

  //6. Test if the Average LDR2 value has dropped significantly (outside the range of tolerance) since the last reading
  if (newAvgLDR2 + tolerance < lastLDR2Avg)
  {
    if (debug > 0)
    {
      Serial.println("LDR 2 has been triggered! " + (String)newAvgLDR2 + " + " + (String)tolerance + " < " + (String)lastLDR2Avg);
    }
    //6.1 If outside the range of tolerance, record trigger time in milliseconds and record that LDR2 has been triggered
    ldr2TriggerTime = millis();
    ldr2Triggered = true;
  }

  //7. Test if both of the LDR's have been triggered
  if (ldr1Triggered && ldr2Triggered)
  {
    if (debug > 0)
    {
      Serial.println("We recognised that both LDR 1 and LDR 2 have been triggered!");
    }

    //7.1 If both LDR's are triggered, test if LDR1's trigger time is less than LDR2's time
    if (ldr1TriggerTime < ldr2TriggerTime)
    {
      if (debug > 0)
      {
        Serial.println("The LDR 1 Trigger time is less than LDR 2 Trigger time!");
        Serial.println("Increasing number of people");
      }
      //7.1.1 If LDR1's trigger is less than LDR2's time, then LDR1 was triggered first therefore we increment the number of people in the room
      noOfPeopleInRoom++;
    }
    else
    {
      if (debug > 0)
      {
        Serial.println("The LDR 2 Trigger time is less than LDR 1 Trigger time!");
        Serial.println("Decreasing number of people");
      }
      //7.2 If LDR1's trigger is not less than LDR2's time, then LDR2 was triggered first therefore we decrement the number of people in the room
      noOfPeopleInRoom--;

      //7.2.1 Test if the number of people in the room is less than 0 (in case system was started with people inside)
      if (noOfPeopleInRoom < 0)
      {
        if (debug > 0)
        {
          Serial.println("We stopped the number of people going negative!");
        }
        //7.2.1.1 If the number of people is less than 0, set it to 0
        noOfPeopleInRoom = 0;
      }
    }

    //8. Test if number of people in the room is equal to 0
    if (noOfPeopleInRoom == 0)
    {
      if (debug > 0)
      {
        Serial.println("Number of people is 0. Turning off light");
      }
      //8.1 If equal to 0, set the room light to LOW
      digitalWrite(roomLight, LOW);
    }
    else
    {
      if (debug > 0)
      {
        Serial.println("Number of people is now " + (String)noOfPeopleInRoom + ". Turning on light");
      }
      //8.2 If not equal to 0, set the room light to HIGH
      digitalWrite(roomLight, HIGH);
    }

    //8.3 set the 7 segment display to the value of people in the room

    if (0 <= noOfPeopleInRoom && noOfPeopleInRoom <= 9)
    {
      pinStartOf7SegmentDisplay = 5;
      for (int j = 0; j < 8; j++)
      {
        digitalWrite(pinStartOf7SegmentDisplay, num_array[noOfPeopleInRoom][j]);
        pinStartOf7SegmentDisplay++;
      }
    }
    else
    {
      pinStartOf7SegmentDisplay = 5;
      for (int j = 0; j < 8; j++)
      {
        digitalWrite(pinStartOf7SegmentDisplay, num_array[10][j]);
        pinStartOf7SegmentDisplay++;

      }

    }


    //9. Reset the trigger time and the flag for triggered for each of the LDR's
    ldr1TriggerTime = 0;
    ldr2TriggerTime = 0;
    ldr1Triggered = false;
    ldr2Triggered = false;

  }

  //10. Test if the trigger time for LDR1 added with the reset delay is more than the currently millisecond time and if LDR1 is marked as triggered
  if (ldr1TriggerTime + resetDelay < millis() && ldr1Triggered)
  {
    if (debug > 0)
    {
      Serial.println("LDR 1 Trigger time expired!");
    }
    //10.1 Set the trigger time to 0 and triggered flag to false
    ldr1TriggerTime = 0;
    ldr1Triggered = false;
  }

  //11. Test if the trigger time for LDR2 added with the reset delay is more than the currently millisecond time and if LDR2 is marked as triggered
  if (ldr2TriggerTime + resetDelay < millis() && ldr2Triggered)
  {
    if (debug > 0)
    {
      Serial.println("LDR 2 Trigger time expired!");
    }
    //11.1 Set the trigger time to 0 and triggered flag to false
    ldr2TriggerTime = 0;
    ldr2Triggered = false;
  }

  //12. Set the new average we gained from the LDR's into the previous variable in order to test upon the next iteration
  lastLDR2Avg = newAvgLDR2;
  lastLDR1Avg = newAvgLDR1;

  //13. Freeze the program if the switch is in an opened state
  bool first = true;
  while (!digitalRead(Switch))
  {
    if (first)
    {
      pinStartOf7SegmentDisplay = 5;
      for (int j = 0; j < 8; j++)
      {
        digitalWrite(pinStartOf7SegmentDisplay, num_array[11][j]);
        pinStartOf7SegmentDisplay++;


      }
      digitalWrite(ledSwitch, false);
      digitalWrite(roomLight, false);
      noOfPeopleInRoom = 1;
      first = !first;


    }


  }

  if (!first)
  {

    digitalWrite(ledSwitch, true);
    digitalWrite(roomLight, true);
    
    pinStartOf7SegmentDisplay = 5;
    for (int j = 0; j < 8; j++)
    {
      digitalWrite(pinStartOf7SegmentDisplay, num_array[noOfPeopleInRoom][j]);
      pinStartOf7SegmentDisplay++;

    }


  }
}
