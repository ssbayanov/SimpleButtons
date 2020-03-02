#include <sbuttons.h>

SButtons buttons; // Holder for buttons

// Pins of buttons
const int UP_PIN = 3;
const int DOWN_PIN = 4;
const int SELECT_PIN = 5;

// ID's of buttons
uint8_t UP;
uint8_t DOWN;
uint8_t SELECT;

void setup() {

  Serial.begin(9600);

  UP = buttons.addButton(UP_PIN);
  DOWN = buttons.addButton(DOWN_PIN);
  SELECT = buttons.addButton(SELECT_PIN);

  buttons.setLongClickDelay(SELECT, 3000); // By default 2000ms

}

void loop() {
  buttons.update();

  if(buttons.isPressed(UP))
    Serial.println("Button UP is pressed");

  if(buttons.isClicked(UP))
    Serial.println("Button UP is clicked");

  if(buttons.isClicked(DOWN))
    Serial.println("Button DOWN is clicked");

  if(buttons.isClicked(SELECT_PIN))
    Serial.println("Button UP is clicked");

  if(buttons.isLongClicked(SELECT_PIN))
    Serial.println("Button SELECT_PIN is long clicked");
}
