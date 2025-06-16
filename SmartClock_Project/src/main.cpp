#include <Arduino.h>
#include <Wire.h>
#include <SerLCD.h>

#define SER_LCD_ENABLED
#define SER_LCD_I2C_ADDRESS 0x72
#define DISPLAY_ADDRESS1 0x72

SerLCD lcd; // Declare the lcd object globally

// put function declarations here:

void setup()
{
  Wire.begin();

  lcd.begin(Wire); // Set up the LCD for I2C communication

  lcd.setBacklight(255, 255, 255); // Set backlight to bright white
  lcd.setContrast(5);              // Set contrast. Lower to 0 for higher contrast.

  lcd.clear(); // Clear the display - this moves the cursor to home position as well
  lcd.print("Hello, World!");
}

void loop()
{
  // put your main code here, to run repeatedly:
}

// put function definitions here:
