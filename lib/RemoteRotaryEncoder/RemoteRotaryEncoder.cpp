#include "RemoteRotaryEncoder.h"

RemoteRotaryEncoder::RemoteRotaryEncoder (uint8_t sensorID)
    : sensorID (sensorID), justRotatedClockwise (false),
      justRotatedCounterclockwise (false)
{
}

void
RemoteRotaryEncoder::loop ()
{
  justRotatedClockwise = justRotatedCounterclockwise = false;
}

void
RemoteRotaryEncoder::rotateClockwise ()
{
  justRotatedClockwise = true;
  justRotatedCounterclockwise = false;
}

void
RemoteRotaryEncoder::rotateCounterclockwise ()
{
  justRotatedClockwise = false;
  justRotatedCounterclockwise = true;
}