#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "arduinoFFT.h"
#include <Stepper.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Menu definition
const char *menuItems[] = { "Mode Auto", "Senar E", "Senar A", "Senar D", "Senar G", "Senar B", "Senar e" };
int selectedItem = 0;
int itemCount = sizeof(menuItems) / sizeof(menuItems[0]);
int firstVisibleItem = 0;  // Index of the first visible item

// Define the number of steps per revolution
const int stepsPerRevolution = 256;
// Define the motor control pins
const int motorPin1 = 12;
const int motorPin2 = 13;
const int motorPin3 = 14;
const int motorPin4 = 27;

// Button pin definitions
#define CHANGE_BUTTON 13  // Change with the appropriate pin
#define OK_BUTTON 12      // Change with the appropriate pin
#define BACK_BUTTON 14    // Change with the appropriate pin

void setup() {
  Serial.begin(115200);

  pinMode(CHANGE_BUTTON, INPUT_PULLUP);
  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(BACK_BUTTON, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Hello, world!");
  display.display();
}

void loop() {
  while (digitalRead(CHANGE_BUTTON) == HIGH && digitalRead(OK_BUTTON) == HIGH && digitalRead(BACK_BUTTON) == HIGH) {
    // Wait until a button is pressed
  }

  // Handle input to select a menu item
  if (digitalRead(CHANGE_BUTTON) == LOW) {
    delay(50);  // Debouncing delay

    while (digitalRead(CHANGE_BUTTON) == LOW) {
      // Wait for the button to be released
    }

    // Button CHANGE is pressed, move to the next item
    selectedItem = (selectedItem + 1) % itemCount;

    // Update the display immediately
    updateDisplay();
  }

  // Handle input to perform an action on the selected item
  if (digitalRead(OK_BUTTON) == LOW) {
    delay(50);  // Debouncing delay

    while (digitalRead(OK_BUTTON) == LOW) {
      // Wait for the button to be released
    }

    // Button OK is pressed, perform an action on the selected item
    handleMenuAction(selectedItem);
    delay(200);  // Optional delay for visual feedback
  }

  // Handle input to go back to the previous item
  if (digitalRead(BACK_BUTTON) == LOW) {
    delay(50);  // Debouncing delay

    while (digitalRead(BACK_BUTTON) == LOW) {
      // Wait for the button to be released
    }

    // Button BACK is pressed, go back to the previous item
    selectedItem = (selectedItem - 1 + itemCount) % itemCount;

    // Update the display immediately
    updateDisplay();
  }
}

void updateDisplay() {
  // Clear the display before drawing the new menu
  display.clearDisplay();

  // Display title
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Pilih Mode/Senar"));

  // Handle scrolling
  if (selectedItem >= firstVisibleItem + 4) {
    firstVisibleItem = selectedItem - 3;  // Show 4 items at a time
  } else if (selectedItem < firstVisibleItem) {
    firstVisibleItem = selectedItem;
  }

  // Limit scrolling to avoid overflow
  if (firstVisibleItem < 0) {
    firstVisibleItem = 0;
  } else if (firstVisibleItem + 4 >= itemCount) {
    firstVisibleItem = itemCount - 4;
  }

  // Display menu items with cursor on the selected item
  for (int i = 0; i < 4; i++) {
    int menuItemIndex = firstVisibleItem + i;
    display.setCursor(0, 10 + i * 10);
    if (menuItemIndex >= 0 && menuItemIndex < itemCount) {
      if (menuItemIndex == selectedItem) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Selected item
        display.print(">");
      } else {
        display.setTextColor(SSD1306_WHITE);
        display.print(" ");
      }
      display.print(menuItems[menuItemIndex]);
    }
  }

  display.display();
}

void handleMenuAction(int selectedItem) {
  // Implement actions for each menu item here
  Serial.print("Selected item: ");
  Serial.println(menuItems[selectedItem]);
  // Add your actions based on the selected item
}
