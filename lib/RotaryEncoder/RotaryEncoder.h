#ifndef POKER_TABLE_ROTARY_ENCODER_H
#define POKER_TABLE_ROTARY_ENCODER_H

#include <Arduino.h>
#include <Encoder.h>

static const int SINGLE_ROTATION = 4;
static const int ROTATION_DEADZONE = 1;

class RotaryEncoder
{
private:
  int pinA;
  int pinB;

  Encoder encoder;

  int lastReading;
  int clockwiseRotateCount;
  int counterclockwiseRotateCount;

public:
  bool justRotatedClockwise = false;
  bool justRotatedCounterclockwise = false;

  RotaryEncoder (int pinA, int pinB);

  void loop ();
};

#endif // POKER_TABLE_ROTARY_ENCODER_H
