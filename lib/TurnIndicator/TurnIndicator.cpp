#ifdef TABLE_MODE

#include "TurnIndicator.h"

TurnIndicator::TurnIndicator(Adafruit_NeoPixel &strip, int brightness)
    : strip(strip), settingPlayers(false), brightness(brightness)
{
  for (int i = 0; i < SECTION_COUNT; i++)
  {
    sectionEnabled[i] = true;
    sectionTemporarilyEnabled[i] = true;
  }
}

void TurnIndicator::setSection(int sectionID, bool clear, uint32_t customColor,
                               bool useCustomColor)
{
  if (clear)
  {
    strip.clear();
  }
  int sectionStart = sectionID * SECTION_SIZE;

  for (int i = sectionStart; i < sectionStart + SECTION_SIZE; i++)
  {
    if (useCustomColor)
    {
      strip.setPixelColor(i, customColor);
    }
    else
    {
      setPixelColorByLocation(i);
    }
  }

  strip.show();
}

void TurnIndicator::setPixelColorByLocation(int pixelIndex)
{
  uint32_t color;
  if (pixelInSection(currentSection, pixelIndex))
  {
    color = selectedColor(pixelIndex);
  }
  else
  {
    color = deselectedColor(pixelIndex);
  }

  strip.setPixelColor(pixelIndex, color);
}

void TurnIndicator::moveSection(int direction)
{
  int nextSection = currentSection + direction;
  if (nextSection == SECTION_COUNT)
  {
    nextSection = 0;
  }
  else if (nextSection < 0)
  {
    nextSection = SECTION_COUNT - 1;
  }

  if (direction == 1)
  {
    for (int i = 0; i < SECTION_SIZE; i++)
    {
      int lastPixel = currentSection * SECTION_SIZE + i;
      int firstPixel = nextSection * SECTION_SIZE + i;
      strip.setPixelColor(lastPixel, deselectedColor(lastPixel));
      strip.setPixelColor(firstPixel, selectedColor(firstPixel));
      strip.show();
      delay(10);
    }
  }
  else if (direction == -1)
  {
    for (int i = SECTION_SIZE - 1; i >= 0; i--)
    {
      int lastPixel = currentSection * SECTION_SIZE + i;
      int firstPixel = nextSection * SECTION_SIZE + i;
      strip.setPixelColor(lastPixel, deselectedColor(lastPixel));
      strip.setPixelColor(firstPixel, selectedColor(firstPixel));
      strip.show();
      delay(10);
    }
  }
}

void TurnIndicator::idleMarquee(unsigned int step)
{
  if (step % 5 != 0)
  {
    return;
  }

  for (unsigned int i = 0; i < SECTION_SIZE; i++)
  {
    uint32_t color;
    int pixelID = currentSection * SECTION_SIZE + i;
    if (i % 2 == step % 2)
    {
      color = selectedColor(pixelID);
    }
    else
    {
      color = deselectedColor(pixelID);
    }
    strip.setPixelColor(pixelID, color);
  }
  strip.show();
}

void TurnIndicator::idlePingPong(unsigned int step)
{
  step %= SECTION_SIZE * 2;
  if (step >= SECTION_SIZE)
  {
    step = SECTION_SIZE - (step % SECTION_SIZE);
  }
  strip.clear();

  for (int i = -1; i < 2; i++)
  {
    int pixelID = currentSection * SECTION_SIZE + step + i;
    strip.setPixelColor(pixelID, selectedColor(pixelID));
  }

  strip.show();
}

void TurnIndicator::idleGrow(unsigned int step)
{
  unsigned int half = SECTION_SIZE / 2;
  step %= SECTION_SIZE;
  if (step >= half)
  {
    step = half - (step % half);
  }

  strip.clear();

  for (unsigned int i = 0; i < step; i++)
  {
    int pixelID = currentSection * SECTION_SIZE + (half + i);
    strip.setPixelColor(pixelID, selectedColor(pixelID));
    pixelID = currentSection * SECTION_SIZE + (half - i);
    strip.setPixelColor(pixelID, selectedColor(pixelID));
  }
  strip.show();
}

void TurnIndicator::idleRandom(unsigned int step)
{
  step %= 2;
  if (step != 0)
  {
    return;
  }

  int index = random(currentSection * SECTION_SIZE,
                     (currentSection + 1) * SECTION_SIZE);

  uint32_t currentColor = strip.getPixelColor(index);
  if (currentColor > 0)
  {
    strip.setPixelColor(index, strip.Color(0, 0, 0));
  }
  else
  {
    strip.setPixelColor(index, selectedColor(index));
  }

  strip.show();
}

uint32_t
TurnIndicator::selectedColor(int pixelIndex)
{
  if (pixelInSection(getSmallBlindSection(), pixelIndex))
  {
    return strip.Color(brightness, 0, brightness);
  }
  else if (pixelInSection(getBigBlindSection(), pixelIndex))
  {
    return strip.Color(brightness, brightness, 0);
  }
  else
  {
    return strip.Color(brightness, brightness, brightness);
  }
}

uint32_t
TurnIndicator::deselectedColor(int pixelIndex)
{
  /*if (pixelInSection (getSmallBlindSection (), pixelIndex))
    {
      return strip.Color (1, 0, 1);
    }
  else if (pixelInSection (getBigBlindSection (), pixelIndex))
    {
      return strip.Color (1, 1, 0);
    }
  else
    {
      return strip.Color (0, 0, 0);
    }*/
  return strip.Color(0, 0, 0);
}

bool TurnIndicator::pixelInSection(int sectionID, int pixelIndex)
{
  if (sectionID == -1)
  {
    return false;
  }
  return pixelIndex >= sectionID * SECTION_SIZE && pixelIndex < (sectionID + 1) * SECTION_SIZE;
}

int TurnIndicator::getSmallBlindSection()
{
  if (dealerSection == -1)
  {
    return -1;
  }

  int smallBlind = dealerSection - 1;
  while (true)
  {
    if (smallBlind < 0)
    {
      smallBlind = SECTION_COUNT + smallBlind;
    }
    if (sectionEnabled[smallBlind])
    {
      break;
    }
    smallBlind--;
  }
  return smallBlind;
}

int TurnIndicator::getBigBlindSection()
{
  if (dealerSection == -1)
  {
    return -1;
  }
  int bigBlind = getSmallBlindSection() - 1;
  while (true)
  {
    if (bigBlind < 0)
    {
      bigBlind = SECTION_COUNT + bigBlind;
    }
    if (sectionEnabled[bigBlind])
    {
      break;
    }
    bigBlind--;
  }
  return bigBlind;
}

int TurnIndicator::enabledPlayerCount()
{
  int count = 0;
  for (int i = 0; i < SECTION_COUNT; i++)
  {
    if (sectionEnabled[i])
    {
      count++;
    }
  }

  return count;
}

void TurnIndicator::counterclockwise()
{
  do
  {
    moveSection(1);
    currentSection = (currentSection + 1) % SECTION_COUNT;
  } while (!sectionEnabled[currentSection] && !settingPlayers);

  showTurn();
}

void TurnIndicator::clockwise()
{
  do
  {
    moveSection(-1);
    currentSection--;
    if (currentSection < 0)
    {
      currentSection = SECTION_COUNT - 1;
    }
  } while (!sectionEnabled[currentSection] && !settingPlayers);

  showTurn();
}

void TurnIndicator::blink()
{
  for (int i = 0; i < BLINK_COUNT; i++)
  {
    for (float x = 0; x < M_PI; x += 0.02)
    {
      int fade = abs(sin(x)) * BLINK_BRIGHTNESS;
      uint32_t normalColor = selectedColor(currentSection * SECTION_SIZE);
      uint8_t normalR = normalColor >> 16;
      uint8_t normalG = normalColor >> 8;
      uint8_t normalB = normalColor;

      long r = (long)normalR + (long)fade;
      if (r > 255)
      {
        r = 255;
      }
      long g = (long)normalG - (long)fade;
      if (g < 0)
      {
        g = 0;
      }
      long b = (long)normalB - (long)fade;
      if (b < 0)
      {
        b = 0;
      }

      uint32_t color = strip.Color(r, g, b);
      setSection(currentSection, false, color, true);
    }
  }
  showTurn();
}

void TurnIndicator::idle(int animationID, unsigned int step)
{
  switch (animationID)
  {
  case 0:
    idleMarquee(step);
    break;
  case 1:
    idlePingPong(step);
    break;
  case 2:
    idleGrow(step);
    break;
  case 3:
    idleRandom(step);
    break;
  }
}

void TurnIndicator::showTurn()
{
  setSection(currentSection, true, 0, false);

  if (dealerSection != -1)
  {
    int smallBlindSection = getSmallBlindSection();
    setSection(smallBlindSection, false, 0, false);

    int bigBlindSection = getBigBlindSection();
    setSection(bigBlindSection, false, 0, false);
  }
}

void TurnIndicator::showSetPlayers()
{
  strip.clear();

  setSection(currentSection, true, 0, false);

  for (int i = 0; i < SECTION_COUNT; i++)
  {
    uint32_t color;
    if (sectionEnabled[i])
    {
      color = strip.Color(0, brightness, 0);
    }
    else
    {
      color = strip.Color(brightness, 0, 0);
    }

    for (int j = -PLAYER_ENABLED_SIZE / 2; j < PLAYER_ENABLED_SIZE / 2; j++)
    {
      strip.setPixelColor(i * SECTION_SIZE + j + SECTION_SIZE / 2, color);
    }
  }
  strip.show();
}

void TurnIndicator::toggleSelectedPlayer()
{
  sectionEnabled[currentSection] = !sectionEnabled[currentSection];
  if (enabledPlayerCount() < MIN_PLAYERS)
  {
    sectionEnabled[currentSection] = !sectionEnabled[currentSection];
  }
  showSetPlayers();
}

void TurnIndicator::startSettingPlayers()
{
  settingPlayersStartTime = millis();
}

void TurnIndicator::continueSettingPlayers()
{
  if (settingPlayersStartTime == -1)
  {
    // The player setting process wasn't started! The buttin is probably
    // just still being held down.
    return;
  }
  else if (millis() - settingPlayersStartTime >= DEALER_SET_TIME)
  {
    settingPlayers = true;
    stopSettingPlayers();
  }
  else
  {
    Serial.println("Setting......");
    int factor = (((float)(millis() - settingPlayersStartTime)) / ((float)DEALER_SET_TIME)) * (SECTION_SIZE / 2);
    Serial.println(factor);

    for (int i = 0; i < SECTION_SIZE / 2; i++)
    {
      uint32_t color;
      if (i <= factor)
      {
        color = strip.Color(brightness, 0, 0);
        strip.setPixelColor(currentSection * SECTION_SIZE + i, color);
        strip.setPixelColor((currentSection + 1) * SECTION_SIZE - i - 1,
                            color);
      }
    }

    strip.show();
  }
}

void TurnIndicator::stopSettingPlayers()
{
  settingPlayersStartTime = -1;
  if (settingPlayers)
  {
    showSetPlayers();
  }
  else
  {
    showTurn();
  }
}

void TurnIndicator::finishSettingPlayers()
{
  settingPlayers = false;
  showTurn();
}

void TurnIndicator::startSettingDealer()
{
  Serial.println("initialized setting dealer");
  dealerSettingStartTime = millis();
}

void TurnIndicator::continueSettingDealer()
{
  if (dealerSettingStartTime == -1)
  {
    // The dealer process wasn't started! The button is probably just still
    // being held down.
    return;
  }
  else if (millis() - dealerSettingStartTime >= DEALER_SET_TIME)
  {
    if (dealerSection == currentSection)
    {
      Serial.println("Dealer is unset!");
      dealerSection = -1;
    }
    else
    {
      Serial.println("Dealer is set!");
      dealerSection = currentSection;
    }
    stopSettingDealer();
  }
  else
  {
    Serial.println("Setting......");
    int factor = (((float)(millis() - dealerSettingStartTime)) / ((float)DEALER_SET_TIME)) * (SECTION_SIZE / 2);
    Serial.println(factor);

    for (int i = 0; i < SECTION_SIZE / 2; i++)
    {
      uint32_t color;
      if (i <= factor)
      {
        color = strip.Color(0, brightness, 0);
        strip.setPixelColor(currentSection * SECTION_SIZE + i, color);
        strip.setPixelColor((currentSection + 1) * SECTION_SIZE - i - 1,
                            color);
      }
    }

    strip.show();
  }
}

void TurnIndicator::stopSettingDealer()
{
  dealerSettingStartTime = -1;
  showTurn();
}

#endif // TABLE_MODE