#include "Button.h"

#include <Arduino.h>

Button::Button (int inputPin, int ledPin)
    : inputPin (inputPin), ledPin (ledPin), lastReading (0), state (0),
      lastButtonChange (0), justPressed (false), justReleased (false),
      pressed (false)
{
  pinMode (inputPin, INPUT_PULLUP);
  pinMode (ledPin, OUTPUT);
}

void
Button::loop ()
{
  justPressed = justReleased = false;

  int latestReading = digitalRead (inputPin);

  if (latestReading != lastReading)
    {
      this->lastButtonChange = millis ();
    }

  if (millis () - lastButtonChange >= DEBOUNCE_INTERVAL)
    {
      if (latestReading != state)
        {
          state = latestReading;
          pressed = state == 0;
          justPressed = state == 0;
          justReleased = state == 1;

          if (ledPin != -1)
            {
              digitalWrite (ledPin, pressed);
            }
        }
    }

  lastReading = latestReading;
}
