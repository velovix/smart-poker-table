#ifdef TABLE_MODE

#ifndef POKER_TABLE_TURN_INDICATOR_H
#define POKER_TABLE_TURN_INDICATOR_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

const int LED_COUNT = 120 * 2 + 34;
static const int SECTION_COUNT = 10;
static const int SECTION_SIZE = LED_COUNT / SECTION_COUNT;

static const int PLAYER_ENABLED_SIZE = 3;
static const int MIN_PLAYERS = 3;

static const int BLINK_COUNT = 2;
static const int BLINK_INTERVAL = 200;
static const int BLINK_BRIGHTNESS = 30;

static const unsigned long TIME_TIL_IDLE = 30000;
static const unsigned long IDLE_ANIMATION_TIME = 3000;

static const int DEALER_SET_TIME = 500;

class TurnIndicator
{
private:
  Adafruit_NeoPixel &strip;
  int currentSection = 0;
  long dealerSettingStartTime = -1;
  long settingPlayersStartTime = -1;
  int dealerSection = -1;
  bool sectionEnabled[SECTION_COUNT];
  bool sectionTemporarilyEnabled[SECTION_COUNT];

  void setSection(int sectionID, bool clear, uint32_t customColor,
                  bool useCustomColor);

  void setPixelColorByLocation(int pixelIndex);

  void moveSection(int direction);

  void idleMarquee(unsigned int step);
  void idlePingPong(unsigned int step);
  void idleGrow(unsigned int step);
  void idleRandom(unsigned int step);

  uint32_t selectedColor(int pixelIndex);
  uint32_t deselectedColor(int pixelIndex);

  bool pixelInSection(int sectionID, int pixelIndex);

  int getSmallBlindSection();
  int getBigBlindSection();

  int enabledPlayerCount();

public:
  bool settingPlayers;
  int brightness;

  TurnIndicator(Adafruit_NeoPixel &strip, int brightness);

  void counterclockwise();
  void clockwise();

  void blink();

  void idle(int animationID, unsigned int step);

  void showTurn();

  void showSetPlayers();
  void toggleSelectedPlayer();
  void temporarilyToggleSelectedPlayer();
  void startSettingPlayers();
  void continueSettingPlayers();
  void stopSettingPlayers();
  void finishSettingPlayers();

  void startSettingDealer();
  void continueSettingDealer();
  void stopSettingDealer();
};

#endif // POKER_TABLE_TURN_INDICATOR_H
#endif // TABLE_MODE
