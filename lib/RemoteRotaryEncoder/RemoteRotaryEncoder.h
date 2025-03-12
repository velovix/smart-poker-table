#ifndef POKER_TABLE_REMOTE_ROTARY_ENCODER_H
#define POKER_TABLE_REMOTE_ROTARY_ENCODER_H

#include <Arduino.h>

class RemoteRotaryEncoder
{
public:
  uint8_t sensorID;
  bool justRotatedClockwise;
  bool justRotatedCounterclockwise;

  RemoteRotaryEncoder (uint8_t sensorID);

  void loop ();
  void rotateClockwise ();
  void rotateCounterclockwise ();
};

#endif // POKER_TABLE_REMOTE_ROTARY_ENCODER_H