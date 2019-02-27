
int num_array[10][8] = {  { 1, 1, 1, 0, 1, 1, 1, 1 }, // 0
  { 0, 1, 0, 0, 0, 0, 1 , 1}, // 1
  { 1, 1, 0, 1, 1, 1, 0 , 1}, // 2
  { 1, 1, 0, 1, 0, 1, 1 , 1}, // 3
  { 0, 1, 1, 1, 0, 0, 1 , 1}, // 4
  { 1, 0, 1, 1, 0, 1, 1 , 1}, // 5
  { 1, 0, 1, 1, 1, 1, 1 , 1}, // 6
  { 1, 1, 1, 0, 0, 0, 1 , 1}, // 7
  { 1, 1, 1, 1, 1, 1, 1 , 1}, // 8
  { 1, 1, 1, 1, 0, 1, 1 , 1} // 9
};




//Digital variable (LED's,
int ledTop = 3;
int ledTopRight = 4;
int ledTopLeft = 5;
int ledMiddle = 6;
int ledBotLeft = 7;
int ledBot = 8;
int ledBotRight = 9;
int ledPeriod = 10;





void setup() {
  pinMode(ledTop, OUTPUT);
  pinMode(ledTopLeft, OUTPUT);
  pinMode(ledTopRight, OUTPUT);
  pinMode(ledMiddle, OUTPUT);
  pinMode(ledBotLeft, OUTPUT);
  pinMode(ledBotRight, OUTPUT);
  pinMode(ledBot, OUTPUT);
  pinMode(ledPeriod, OUTPUT);

  
}

void loop() {


  int input = x;

  int pin = 3;
  for (int j = 0; j < 8; j++)
  {
    digitalWrite(pin, num_array[input][j]);
    pin++;
  }
}


