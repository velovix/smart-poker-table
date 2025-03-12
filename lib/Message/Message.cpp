#include "Message.h"

MessageQueue::MessageQueue()
    : queue(sizeof(Message), QUEUE_SIZE), buttonCount(0),
      rotaryEncoderCount(0)
{
  for (int i = 0; i < MAX_REGISTERED_BUTTONS; i++)
  {
    buttons[i] = nullptr;
  }
  for (int i = 0; i < MAX_REGISTERED_ROTARY_ENCODERS; i++)
  {
    rotaryEncoders[i] = nullptr;
  }
}

void MessageQueue::addButtonMessages(Button &button, uint8_t sensorID)
{
  Message message;

  if (button.justPressed)
  {
    Serial.println("Sending button with ID:");
    Serial.println(sensorID);
    message = {sensorID : sensorID, value : value::PRESSED};
    queue.push(&message);
  }
  else if (button.justReleased)
  {
    message = {sensorID : sensorID, value : value::RELEASED};
    queue.push(&message);
  }
}

void MessageQueue::addRotaryEncoderMessages(RotaryEncoder &encoder,
                                            uint8_t sensorID)
{
  Message message;

  if (encoder.justRotatedClockwise)
  {
    Serial.println("Sending clockwise rotation");
    message = {sensorID : sensorID, value : value::ROTATED_CLOCKWISE};
    queue.push(&message);
  }
  else if (encoder.justRotatedCounterclockwise)
  {
    Serial.println("Sending counterclockwise rotation");
    message = {sensorID : sensorID, value : value::ROTATED_COUNTERCLOCKWISE};
    queue.push(&message);
  }
}

void MessageQueue::flush(RF24 &radio)
{
  while (!queue.isEmpty())
  {
    Message message;
    queue.pop(&message);
    bool result = radio.write(&message, sizeof(message));
    if (result)
    {
      Serial.println("Sent the message!");
    }
    else
    {
      Serial.println("Failed to send the message...");
    }
  }
}

void MessageQueue::read(RF24 &radio)
{
  while (radio.available())
  {
    Serial.println("There's a message on the radio");
    Message message;
    radio.read(&message, sizeof(Message));

    for (int i = 0; i < buttonCount; i++)
    {
      if (buttons[i]->sensorID == message.sensorID)
      {
        switch (message.value)
        {
        case value::PRESSED:
          buttons[i]->press();
          break;
        case value::RELEASED:
          buttons[i]->release();
          break;
        default:
          Serial.println("Unexpected button value in message");
          break;
        }
      }
    }

    for (int i = 0; i < rotaryEncoderCount; i++)
    {
      if (rotaryEncoders[i]->sensorID == message.sensorID)
      {
        switch (message.value)
        {
        case value::ROTATED_CLOCKWISE:
          rotaryEncoders[i]->rotateClockwise();
          break;
        case value::ROTATED_COUNTERCLOCKWISE:
          rotaryEncoders[i]->rotateCounterclockwise();
          break;
        default:
          Serial.println(
              "Unexpected rotary encoder value in message");
          break;
        }
      }
    }
  }
}

void MessageQueue::registerRemoteButton(RemoteButton *button)
{
  if (buttonCount >= MAX_REGISTERED_BUTTONS)
  {
    Serial.println("MessageQueue::registerButton: There is no more room "
                   "for registered buttons! Ignoring request.");
    return;
  }

  buttons[buttonCount] = button;
  buttonCount++;
}

void MessageQueue::registerRemoteRotaryEncoder(RemoteRotaryEncoder *rotaryEncoder)
{
  if (rotaryEncoderCount >= MAX_REGISTERED_ROTARY_ENCODERS)
  {
    Serial.println(
        "MessageQueue::registerRotaryEncoder: There is no more room "
        "for registered rotary encoders! Ignoring request.");
    return;
  }

  rotaryEncoders[rotaryEncoderCount] = rotaryEncoder;
  rotaryEncoderCount++;
}