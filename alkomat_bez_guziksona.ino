//Written by Marcin Stecewicz & Kamil Stachowiak

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DFRobot_LCD.h"

#define OLED_RESET 10000
#include <Keypad.h>                       //dodaj bibliotekę obsługującą klawiaturę

const byte wiersze = 4;                      //ilość wierszy klawiatury
const byte kolumny = 4;                      //ilość kolumn klawiatury


char klawiatura[wiersze][kolumny] = {                 //ustaw rozkład klawiszy
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte wierszePiny[wiersze] = {9,8,7,6};            //wiersze podłączone do pinów (numeracja od lewej strony do prawej)
byte kolumnyPiny[kolumny] = {5,4,3,2};            //kolumny podłączone do pinów (numeracja od lewej strony do prawej)


//utwórz klawiaturę na postawienie ustawienia klawiszy, pinów Arduino oraz ilości kolumn i wierszy
Keypad klaw1 = Keypad( makeKeymap(klawiatura), wierszePiny, kolumnyPiny, wiersze, kolumny );

int TIME_UNTIL_WARMUP = 10;
unsigned long time;
int analogPin = 0;
float val = 0;
DFRobot_LCD lcd(16,2);

void setup()   {                
  lcd.init();
  lcd.clear();
  Serial.begin(9600);
}


void loop() {  
  char przycisk = klaw1.getKey();  
  if (przycisk=='3') {
    delay(100);
    val = readAlcohol();
    printTitle();
    printWarming();
    lcd.clear();
    time = millis()/1000;
    if(time<=TIME_UNTIL_WARMUP)
    {
      lcd.setCursor(0, 0);
      lcd.print("Czekaj 10s.");

      delay(2000);
      lcd.clear();

    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pomiar");
      delay(1200);
      lcd.setCursor(0, 1);
      lcd.print("Start");    
      delay(2000);
      lcd.clear();
      Serial.println(val);
      printAlcohol(val);
      lcd.clear();
      printAlcoholLevel(val);
      lcd.clear();
    
    }
    lcd.display();
    delay(100);
    lcd.clear();

  }
  
}


void printTitle()
{
  lcd.setCursor(0, 1);
  lcd.print("Alkomat");
  delay(2000);
  lcd.clear();
}

void printWarming()
{
  lcd.setCursor(0, 1);
  lcd.print("Rozgrzewanie");
  delay(2000);
}

void printAlcohol(float value)
{
  lcd.setCursor(0, 0);
  lcd.print("Pomiar:");
  lcd.setCursor(0, 1);
  lcd.print(value);
  delay(2000);
}

void printAlcoholLevel(float value)
{
  if(value<0.095) {
    lcd.setCursor(0, 1);
    lcd.print("Trzezwy");
    delay(2000);
    lcd.clear();
  }
  if (value>=0.095)
  {
    lcd.setCursor(0, 1);
    lcd.print("Nietrzezwy");
    delay(2000);
    lcd.clear();  }
 }
 
 /*float float_readAlcohol() {
   float wart=0;
   wart=float(readAlcohol);
   wart=wart/1000;
   return wart;
 }*/

 float readAlcohol()
 {
  float val = 0;
  float val1;
  float val2;
  float val3; 
  float val4;
  float val5;    

  lcd.clear();
  val1 = analogRead(analogPin); 
  delay(100);
  val2 = analogRead(analogPin); 
  delay(100);
  val3 = analogRead(analogPin);
  delay(100);
  val4 = analogRead(analogPin);
  delay(100);
  val5 = analogRead(analogPin);    
  val = (((val1+val2+val3+val4+val5)/5)-230)/2500;///1000;
  if (val<=0) {
    val=0;
  }
  //Serial.println(val);
  return val;
  
 }


