#ifndef POKER_TABLE_REMOTE_BUTTON_H
#define POKER_TABLE_REMOTE_BUTTON_H

#include <Arduino.h>

class RemoteButton
{
public:
  uint8_t sensorID;
  bool justPressed;
  bool justReleased;
  bool pressed;

  RemoteButton (uint8_t sensorID);

  void loop ();
  void press ();
  void release ();
};

#endif // POKER_TABLE_REMOTE_BUTTON_H