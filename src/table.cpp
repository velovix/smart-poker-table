#ifdef TABLE_MODE

#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <RF24.h>
// Loads the nRF24L01 driver to RF24
#include <nRF24L01.h>

#include <Message.h>
#include <TurnIndicator.h>
#include <RemoteButton.h>
#include <RemoteRotaryEncoder.h>

const int BRIGHTNESS_STEP = 10;
const int MAX_BRIGHTNESS = 255 - BRIGHTNESS_STEP * 2;
const int MIN_BRIGHTNESS = 1;
const int BOOT_ANIMATION_BRIGHTNESS = 50;

Adafruit_NeoPixel strip(LED_COUNT, /* LED pin */ 6, NEO_GRB + NEO_KHZ800);

RemoteButton dimmerButton(sensor::DIMMER_BUTTON);
RemoteButton dealerButton(sensor::DEALER_BUTTON);
RemoteButton setPlayerModeButton(sensor::SET_PLAYER_MODE_BUTTON);
RemoteButton togglePlayerButton(sensor::TOGGLE_PLAYER_BUTTON);
RemoteButton pingButton(sensor::PING_BUTTON);
RemoteRotaryEncoder turnSelector(sensor::TURN_SELECTOR);

RF24 radio(A1, A2);
MessageQueue messageQueue;

TurnIndicator turnIndicator(strip, MIN_BRIGHTNESS);

unsigned long idleStart = millis();
unsigned long lastIdleStep = 0;
unsigned int idleStep = 0;
int idleAnimationID = -1;

void resetIdle()
{
  idleStart = millis();
  idleAnimationID = -1;
}

void bootAnimation()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, BOOT_ANIMATION_BRIGHTNESS));
    strip.show();

    int delayPeriod = (pow(LED_COUNT, 2) - pow(i, 2)) / 3000;
    delay(delayPeriod);
  }

  delay(500);

  for (int blue = BOOT_ANIMATION_BRIGHTNESS; blue >= 0; blue--)
  {
    for (int i = 0; i < LED_COUNT; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, blue));
    }
    strip.show();
  }

  strip.clear();

  delay(500);
}

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3, 5);
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(PIPE_NUMBER, PIPE_ADDRESS);
  radio.startListening();

  messageQueue.registerRemoteButton(&dimmerButton);
  messageQueue.registerRemoteButton(&dealerButton);
  messageQueue.registerRemoteButton(&setPlayerModeButton);
  messageQueue.registerRemoteButton(&togglePlayerButton);
  messageQueue.registerRemoteButton(&pingButton);
  messageQueue.registerRemoteRotaryEncoder(&turnSelector);

  strip.begin();
  strip.show();
  // We control brightness on a per-pixel basis
  strip.setBrightness(255);

  bootAnimation();
  turnIndicator.showTurn();

  Serial.println("Table initialized");
}

void dimmer()
{
  if (turnSelector.justRotatedClockwise)
  {
    if (turnIndicator.brightness + BRIGHTNESS_STEP > MAX_BRIGHTNESS)
    {
      turnIndicator.brightness = MAX_BRIGHTNESS;
    }
    else
    {
      turnIndicator.brightness += BRIGHTNESS_STEP;
    }
    turnIndicator.showTurn();
  }
  else if (turnSelector.justRotatedCounterclockwise)
  {
    if (turnIndicator.brightness - BRIGHTNESS_STEP < MIN_BRIGHTNESS)
    {
      turnIndicator.brightness = MIN_BRIGHTNESS;
    }
    else
    {
      turnIndicator.brightness -= BRIGHTNESS_STEP;
    }
    turnIndicator.showTurn();
  }
}

void idle()
{
  if (idleAnimationID == -1)
  {
    idleAnimationID = random(0, 4);
  }
  if (millis() - lastIdleStep > 50)
  {
    turnIndicator.idle(idleAnimationID, idleStep++);
    lastIdleStep = millis();
  }

  if (millis() - idleStart > TIME_TIL_IDLE + IDLE_ANIMATION_TIME)
  {
    resetIdle();
    turnIndicator.showTurn();
  }
}

void loop()
{
  dimmerButton.loop();
  pingButton.loop();
  dealerButton.loop();
  setPlayerModeButton.loop();
  togglePlayerButton.loop();
  turnSelector.loop();

  messageQueue.read(radio);

  if (turnIndicator.settingPlayers)
  {
    if (setPlayerModeButton.justPressed)
    {
      turnIndicator.finishSettingPlayers();
    }
    if (togglePlayerButton.justPressed)
    {
      turnIndicator.toggleSelectedPlayer();
    }
    if (turnSelector.justRotatedClockwise)
    {
      turnIndicator.clockwise();
      turnIndicator.showSetPlayers();
    }
    else if (turnSelector.justRotatedCounterclockwise)
    {
      turnIndicator.counterclockwise();
      turnIndicator.showSetPlayers();
    }
  }
  else if (dimmerButton.pressed)
  {
    dimmer();
  }
  else if (dealerButton.justPressed)
  {
    turnIndicator.startSettingDealer();
    resetIdle();
  }
  else if (dealerButton.pressed)
  {
    turnIndicator.continueSettingDealer();
    resetIdle();
  }
  else if (dealerButton.justReleased)
  {
    turnIndicator.stopSettingDealer();
    resetIdle();
  }
  else if (setPlayerModeButton.justPressed)
  {
    turnIndicator.startSettingPlayers();
    resetIdle();
  }
  else if (setPlayerModeButton.pressed)
  {
    turnIndicator.continueSettingPlayers();
    resetIdle();
  }
  else if (setPlayerModeButton.justReleased)
  {
    turnIndicator.stopSettingPlayers();
    resetIdle();
  }
  else if (pingButton.justPressed)
  {
    turnIndicator.blink();
    resetIdle();
  }
  else if (turnSelector.justRotatedClockwise)
  {
    turnIndicator.clockwise();
    resetIdle();
  }
  else if (turnSelector.justRotatedCounterclockwise)
  {
    turnIndicator.counterclockwise();
    resetIdle();
  }
  else if (millis() - idleStart > TIME_TIL_IDLE)
  {
    idle();
  }
}

#endif // TABLE_MODE
