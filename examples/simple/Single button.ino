#include <sbuttons.h>

SButton button(3, LOW, INPUT_PULLUP); // 1-pin, 2-press state, 3-init mode

void setup() {

  Serial.begin(9600);
}

void loop() {
  button.check();

  if(button.isPressed())
    Serial.println("Button is pressed");

  if(button.isClicked())
    Serial.println("Button is clicked");

  if(button.isLongClicked())
    Serial.println("Button is long clicked");
}