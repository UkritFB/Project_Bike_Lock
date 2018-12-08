#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
SoftwareSerial ArduinoSerial(3, 2); // RX, TX
//----------------------------------------------------------------------------- KEYPAD
LiquidCrystal_I2C lcd(0x27, 16, 2);
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
int pass_key[4] = {0, 0, 0, 0};
int pass_bit = 0;
int key[4] = {9, 9, 9, 9};
int i = 0;
char aa;
int state = 0;
int state_lock = 1;
int randNumber;
//-----------------------------------------------------------------------------
void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(115200);
  ArduinoSerial.begin(4800);
  pinMode(13, OUTPUT);
  Serial.println("START");
}
//-----------------------------------------------------------------------------
void loop() {
  //      ArduinoSerial.print("9999");
  //      ArduinoSerial.print("\n");
  if (state_lock == 1) {
    while (i != '#') {
      i = ReadKey1();
      DP2();
    }
    key[0] = random(1, 10);
    key[1] = random(1, 10);
    key[2] = random(1, 10);
    key[3] = random(1, 10);
    ArduinoSerial.print(String(key[0]) + String(key[1]) + String(key[2]) + String(key[3]));
    ArduinoSerial.print("\n");
    delay(10000);
    DP1();
    state_lock = 0;
  }
  else {
    while (pass_bit <= 3) {
      aa = ReadKey();
      i = int(aa);
      pass_key[pass_bit - 1] = aa;
      print_serial_port();
      DP1();
    }
    if (key[0] == pass_key[0] && key[1] == pass_key[1] && key[2] == pass_key[2] && key[3] == pass_key[3]) {

      i = 0;
      pass_bit = 0;
      pass_key[0] = {0};
      pass_key[1] = {0};
      pass_key[2] = {0};
      pass_key[3] = {0};
      digitalWrite(13, 1);
      state_lock = 1;
      Serial.println("START");

    }
    else {
      state_lock = 0;
      pass_bit = 0;
      pass_key[0] = {0};
      pass_key[1] = {0};
      pass_key[2] = {0};
      pass_key[3] = {0};
    }

  }





}
//-----------------------------------------------------------------------------
int ReadKey1() {
  char key = keypad.getKey();
  if (key) {
    return int(key);
  }
}
int ReadKey() {
  char key = keypad.getKey();
  if (key) {
    if (pass_bit >= 4) {
      pass_bit = 0;
    }
    else {
      pass_bit++;
    }
    return int(key);
  }
}
//-----------------------------------------------------------------------------
void Chack_pass(char a , char b, char c , char d) {
  if (a == pass_key[1] && b == pass_key[2] && c == pass_key[3] && d == pass_key[4]) {
    DP2();
    digitalWrite(13, 1);
    ArduinoSerial.print("5555");
    ArduinoSerial.print("\n");
    clear_key('D');
  }
  else {
    DP1();
    digitalWrite(13, 0);
  }
}
//-----------------------------------------------------------------------------
void DP1() {
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print("     L O C K !!!");
  lcd.setCursor(1, 2);
  lcd.print("Insert Pass : ");
  lcd.setCursor(15, 2);
  lcd.print(pass_key[0]);
  lcd.setCursor(16, 2);
  lcd.print(pass_key[1]);
  lcd.setCursor(17, 2);
  lcd.print(pass_key[2]);
  lcd.setCursor(18, 2);
  lcd.print(pass_key[3]);
}
void DP2() {
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("   U N L O C K !!!");

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
  Serial.print(key[0]);
  Serial.print(key[1]);
  Serial.print(key[2]);
  Serial.print(key[3]);
  Serial.print(" ");
  Serial.print(pass_key[0]);
  Serial.print(pass_key[1]);
  Serial.print(pass_key[2]);
  Serial.println(pass_key[3]);
}
