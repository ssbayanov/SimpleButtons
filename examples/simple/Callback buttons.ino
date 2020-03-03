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

// Variables
int i = 0;

void setup() {

    Serial.begin(9600);

    UP = buttons.addButton(UP_PIN);
    buttons.setClickCallback(UP, []() { Serial.print("Button UP is clicked. Variable i: "); Serial.println(++i) });
    buttons.setLongClickCallback(DOWN, []() { Serial.print("Button UP is long clicked. Variable i: "); Serial.println(i+=10) });

    DOWN = buttons.addButton(DOWN_PIN);
    buttons.setClickCallback(DOWN, []() { Serial.print("Button DOWN is clicked. Variable i: "); Serial.println(--i) });
    buttons.setLongClickCallback(DOWN, []() { Serial.print("Button DOWN is long clicked. Variable i: "); Serial.println(i-=10) });

    SELECT = buttons.addButton(SELECT_PIN);
    buttons.setClickCallback(SELECT_PIN, []() { Serial.print("Button SELECT is clicked. Variable i: "); Serial.println(i) });  

}

void loop() {
  buttons.update();
}