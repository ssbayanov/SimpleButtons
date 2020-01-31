#include <sbuttons.h>

SButtons buttons;

SButton b1, b2, b3;

void setup() {

  Serial.begin(9600);
  
  b1 = buttons.addButton(3);

  b2 = buttons.addButton(4, HIGH);
  b2.setClickDelay(250); // By default 100ms

  b3 = buttons.addButton(5);
  b3.setLongClickDelay(3000); // By default 2000ms

}

void loop() {
  buttons.check();

  if(b1.isPressed())
    Serial.println("Button 1 is pressed");

  if(b2.isClicked())
    Serial.println("Button 2 is clicked");

  if(b3.isClicked())
    Serial.println("Button 3 is clicked");

  if(b3.isLongClicked())
    Serial.println("Button 3 is long clicked");
}