#include "RotaryEncoder.h"

#include <Encoder.h>

RotaryEncoder::RotaryEncoder (int pinA, int pinB)
    : pinA (pinA), pinB (pinB), encoder (pinA, pinB), lastReading (-999),
      clockwiseRotateCount (0), counterclockwiseRotateCount (0)
{
}

void
RotaryEncoder::loop ()
{
  justRotatedClockwise = justRotatedCounterclockwise = false;

  int reading = encoder.read ();

  if (reading >= lastReading + SINGLE_ROTATION)
    {
      clockwiseRotateCount++;
      counterclockwiseRotateCount = 0;
      lastReading = reading;
    }
  else if (reading < lastReading - SINGLE_ROTATION)
    {
      counterclockwiseRotateCount++;
      clockwiseRotateCount = 0;
      lastReading = reading;
    }

  if (clockwiseRotateCount >= ROTATION_DEADZONE)
    {
      justRotatedClockwise = true;
      clockwiseRotateCount--;
    }
  else if (counterclockwiseRotateCount >= ROTATION_DEADZONE)
    {
      justRotatedCounterclockwise = true;
      counterclockwiseRotateCount--;
    }
}