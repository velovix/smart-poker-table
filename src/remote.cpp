#ifdef REMOTE_MODE

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
// Loads the nRF24L01 driver to RF24
#include <nRF24L01.h>

#include <Button.h>
#include <Message.h>
#include <RotaryEncoder.h>

Button dimmerButton(4, 9);
Button dealerButton(5, A3);
Button setPlayerModeButton(7, A4);
Button togglePlayerButton(8, A0);
Button pingButton(A5, -1);
RotaryEncoder turnSelector(2, 3);
RF24 radio(A1, A2);

MessageQueue messageQueue;

void setup()
{
  Serial.begin(9600);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3, 5);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(PIPE_ADDRESS);
  radio.stopListening();
}

void loop()
{
  dimmerButton.loop();
  pingButton.loop();
  dealerButton.loop();
  setPlayerModeButton.loop();
  togglePlayerButton.loop();
  turnSelector.loop();

  messageQueue.addButtonMessages(dimmerButton, sensor::DIMMER_BUTTON);
  messageQueue.addButtonMessages(pingButton, sensor::PING_BUTTON);
  messageQueue.addButtonMessages(dealerButton, sensor::DEALER_BUTTON);
  messageQueue.addButtonMessages(setPlayerModeButton,
                                 sensor::SET_PLAYER_MODE_BUTTON);
  messageQueue.addButtonMessages(togglePlayerButton,
                                 sensor::TOGGLE_PLAYER_BUTTON);
  messageQueue.addRotaryEncoderMessages(turnSelector, sensor::TURN_SELECTOR);

  messageQueue.flush(radio);
}

#endif // REMOTE_MODE
