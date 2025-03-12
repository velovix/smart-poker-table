#ifndef POKER_TABLE_BUTTON_H
#define POKER_TABLE_BUTTON_H

const static unsigned int DEBOUNCE_INTERVAL = 50;

class Button
{
private:
  int inputPin;
  int ledPin;

  int lastReading;
  int state;
  unsigned long lastButtonChange;

public:
  bool justPressed;
  bool justReleased;
  bool pressed;

  Button (int inputPin, int ledPin);

  void loop ();
};

#endif // POKER_TABLE_BUTTON_H
