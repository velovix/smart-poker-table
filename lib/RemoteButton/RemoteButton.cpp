#include "RemoteButton.h"

RemoteButton::RemoteButton (uint8_t sensorID)
    : sensorID (sensorID), justPressed (false), justReleased (false),
      pressed (false)
{
}

void
RemoteButton::loop ()
{
  justPressed = justReleased = false;
}

void
RemoteButton::press ()
{
  justPressed = true;
  justReleased = false;
  pressed = true;
}

void
RemoteButton::release ()
{
  justPressed = false;
  justReleased = true;
  pressed = false;
}
