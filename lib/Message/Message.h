#ifndef POKER_TABLE_MESSAGE_H
#define POKER_TABLE_MESSAGE_H

#include <Arduino.h>
#include <RF24.h>
#include <cppQueue.h>

#include <Button.h>
#include <RemoteButton.h>
#include <RemoteRotaryEncoder.h>
#include <RotaryEncoder.h>

const uint8_t PIPE_NUMBER = 0;
const byte PIPE_ADDRESS[6] = "00001";

namespace sensor
{
  const uint8_t DIMMER_BUTTON = 1;
  const uint8_t DEALER_BUTTON = 2;
  const uint8_t SET_PLAYER_MODE_BUTTON = 3;
  const uint8_t TOGGLE_PLAYER_BUTTON = 4;
  const uint8_t PING_BUTTON = 5;
  const uint8_t TURN_SELECTOR = 6;
}

namespace value
{
  const uint8_t PRESSED = 1;
  const uint8_t RELEASED = 2;
  const uint8_t ROTATED_CLOCKWISE = 3;
  const uint8_t ROTATED_COUNTERCLOCKWISE = 4;
}

class Message
{
public:
  uint8_t sensorID;
  uint8_t value;
};

const int QUEUE_SIZE = 20;
const int MAX_REGISTERED_BUTTONS = 10;
const int MAX_REGISTERED_ROTARY_ENCODERS = 3;

class MessageQueue
{
private:
  cppQueue queue;

  RemoteButton *buttons[MAX_REGISTERED_BUTTONS];
  int buttonCount;
  RemoteRotaryEncoder *rotaryEncoders[MAX_REGISTERED_ROTARY_ENCODERS];
  int rotaryEncoderCount;

public:
  MessageQueue();

  void addButtonMessages(Button &button, uint8_t sensorID);
  void addRotaryEncoderMessages(RotaryEncoder &encoder, uint8_t sensorID);

  void flush(RF24 &radio);
  void read(RF24 &radio);

  void registerRemoteButton(RemoteButton *button);
  void registerRemoteRotaryEncoder(RemoteRotaryEncoder *rotaryEncoder);
};

#endif // POKER_TABLE_MESSAGE_H