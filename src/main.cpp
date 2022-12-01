#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <HCSR04.h>
#include "Servo.h"
#include <pincontrol.hpp>
#define binfull 5.7

void setup(void);
void loop(void);
bool isEven(uint8_t);
bool isOdd(uint8_t);


HCSR04 binCheck (12, 10), personCheck (0, 1);
Servo lid;


LiquidCrystal_I2C lcd (0x27, 16, 2);


char keys [4] [3] =
{
    { '1', '2', '3'},
    { '4', '5', '6'},
    { '7', '8', '9'},
    { '*', '0', '#'}
};

char code [5] { }, key;

uint8_t rowPins [4] = {2, 3, 4, 5},
        colPins [3] = {6, 7, 9};

Keypad keypad (makeKeymap(keys), rowPins, colPins, 4, 3);

uint8_t binPercent;             //How full the bin is in %

void setup()
{
    lid.attach(11);
    lid.write(20);


    lcd.begin();               // initialize the lcd


    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("   Prod. By:    ");
    delay(400);
    lcd.setCursor(0, 1);
    lcd.print(" NorahLinks TM  ");
    delay(400);
    lcd.setCursor(0, 0);
    lcd.print("   Eichen (R)   ");
    lcd.cursor();
    lcd.blink();
    delay(2000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Smart Waste Bin");
    delay(2000);
    lcd.clear();
}

void loop()
{
    binPercent = (binfull / binCheck.dist()) * 100;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bin percentage: ");
    lcd.setCursor(0, 1);
    lcd.print(binPercent);
    lcd.setCursor(5, 1);
    lcd.print("% full");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press * to enter");
    lcd.setCursor(0, 1);
    lcd.print("code to open bin");
    delay(1000);

    key = keypad.waitForKey();

    if(key == '*')             //Wait until key is pressed
    {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Enter code: ");
        lcd.setCursor(12, 1);
        for(int i = 0; i < 4; ++i)
        {
            code[i] = keypad.waitForKey();
            lcd.print('*');
        }
        code[4] = '\0';


        lcd.clear();

        if (isEven(code[0] - '0') && isOdd(code[3] - '0'))
        {
            lcd.setCursor(0, 0);
            lcd.print("Correct Code!");
            lcd.setCursor(0, 1);
            lcd.print("Bin opening...");
            delay(50);                             //TODO: Change after testing.
            delay(500);
            lid.write(100);                             //TODO: Change after testing.
            delay(500);
            lcd.setCursor(0, 1);
            lcd.print("Bin Open!");
            delay(1000);
            lcd.clear();


            while(personCheck.dist() <= 37)
            {
                lcd.clear();
                lcd.home();
                lcd.print("You may dump");
                delay(500);
                lcd.setCursor(0, 1);
                lcd.print("your refuse...");
                delay(1000);
                lcd.clear();
                lcd.home();
                lcd.print("...in the bin.");
                delay(500);
                lcd.setCursor(0, 1);
                lcd.print("Thank you!");
                delay(1000);
            }

            lcd.clear();
            lcd.home();
            lcd.print("Bin closing...");
            lid.write(20);                           //TODO: Change after testing.
            delay(200);
            lcd.setCursor(0, 1);
            lcd.print("Bin closed!");
            delay(500);
        }

        else
        {
            lcd.clear();
            lcd.home();
            lcd.print("Incorrect code!");
            lcd.setCursor(0, 1);
            delay(500);
            lcd.print("Try Again.");
            delay(1500);
        }

        key = '0';
    }

    else
    {

    }
}

bool isEven(uint8_t num)
{
   return ( ((num%2) == 0) ? true : false );
}

bool isOdd(uint8_t num)
{
    return ( ((num%2) != 0) ? true : false);
}
