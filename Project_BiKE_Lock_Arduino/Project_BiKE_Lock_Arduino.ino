#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
SoftwareSerial ArduinoSerial(3, 2); // RX, TX
//-----------------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x3f, 16, 2);
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {11, 10, 9, 8}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//-----------------------------------------------------------------------------
char pass_key[4] = {'*', '*', '*', '*'};
char temp_key[4] = {'0', '0', '0', '0'};
int  pass_bit = 0;
int  key[4] = {9, 9, 9, 9};
int  i = 0;
char aa = ' ';
int  state_lock = 1;
int  randNumber;
int  Reset_MCU = A0;
//-----------------------------------------------------------------------------
void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  ArduinoSerial.begin(4800);
  pinMode(13, OUTPUT);
  pinMode(Reset_MCU, OUTPUT);
  Serial.println("START");
}
//-----------------------------------------------------------------------------
void loop() {
  if (state_lock == 1) {
    while (i != '#') {
      i = keypad.getKey();
      digitalWrite(13, 1);
      DP2();
    }
    Serial.println("LOCK !!!!!");
    key[0] = random(1, 10);
    key[1] = random(1, 10);
    key[2] = random(1, 10);
    key[3] = random(1, 10);
    ArduinoSerial.print(String(key[0]) + String(key[1]) + String(key[2]) + String(key[3]));
    ArduinoSerial.print("\n");
    state_lock = 0;
    digitalWrite(13, 0);
  }
  else {
    while (pass_bit <= 3) {
      aa = ReadKey();
      pass_key[pass_bit - 1] = aa;
      Serial.print(state_lock);
      Serial.print(" ");
      Serial.print(pass_key[0]);
      Serial.print(pass_key[1]);
      Serial.print(pass_key[2]);
      Serial.println(pass_key[3]);
      DP1();
    }
    //    temp_key[0] = char(key[0]);
    //    temp_key[1] = char(key[1]);
    //    temp_key[2] = char(key[2]);
    //    temp_key[3] = char(key[3]);
    if (pass_key[0] == pass_key[0] && pass_key[1] == pass_key[1] && pass_key[2] == pass_key[2] && pass_key[3] == pass_key[3]) {
      pass_bit = 0;
      i = 0;
      pass_key[0] = {'*'};
      pass_key[1] = {'*'};
      pass_key[2] = {'*'};
      pass_key[3] = {'*'};
      digitalWrite(13, 1);
      state_lock = 1;
      //  DP2();
    }
    else {
      DP3();
      digitalWrite(13, 0);
      state_lock = 0;
      pass_bit = 0;
      i = 0;
      pass_key[0] = {'*'};
      pass_key[1] = {'*'};
      pass_key[2] = {'*'};
      pass_key[3] = {'*'};
      //  DP1();
    }
  }
}
//-----------------------------------------------------------------------------
int ReadKey1() {
  int key = keypad.getKey();
  if (key) {
    return key;
  }
}
int ReadKey() {
  int key = keypad.getKey();
  if (key) {
    if (pass_bit >= 4) {
      pass_bit = 0;
    }
    else {
      pass_bit++;
    }
    return key;
  }
}
//-----------------------------------------------------------------------------
void DP1() {

  lcd.setCursor(0, 1);
  lcd.print("     L O C K ");
  lcd.setCursor(1, 0);
  lcd.print(" Pass : ");
  lcd.setCursor(9, 0);
  lcd.print(pass_key[0]);
  lcd.setCursor(10, 0);
  lcd.print(" ");
  lcd.setCursor(11, 0);
  lcd.print(pass_key[1]);
  lcd.setCursor(12, 0);
  lcd.print(" ");
  lcd.setCursor(13, 0);
  lcd.print(pass_key[2]);
  lcd.setCursor(14, 0);
  lcd.print(" ");
  lcd.setCursor(15, 0);
  lcd.print(pass_key[3]);
}
void DP2() {
  lcd.setCursor(0, 1);
  lcd.print(" Press Enter  # ");
  lcd.setCursor(0, 0);
  lcd.print("  U N L O C K   ");

}
void DP3() {
  lcd.setCursor(0, 0);
  lcd.print("                      ");
  lcd.setCursor(0, 1);
  lcd.print("fffffffffffffff");
}
//-----------------------------------------------------------------------------
void clear_key(char kk) {
  if (kk == 'D') {
    for (int j = 0 ; j <= 4; j++) {
      pass_key[j] = 0;
    }
    pass_bit = 0;
  }
}
//-----------------------------------------------------------------------------
void print_serial_port() {
  //  Serial.print("State : ");
  //  Serial.print(state_lock);
  //  Serial.print("  ");
  //  Serial.print("Key Now : ");
  //  Serial.print(i);
  //  Serial.print("  ");
  //  Serial.print("Bit Now : ");
  //  Serial.print(pass_bit);
  //  Serial.print("  ");
  //  Serial.print(key[0]);
  //  Serial.print(key[1]);
  //  Serial.print(key[2]);
  //  Serial.print(key[3]);
  //  Serial.print(" ");
  Serial.print(pass_key[0]);
  Serial.print(pass_key[1]);
  Serial.print(pass_key[2]);
  Serial.println(pass_key[3]);
}
