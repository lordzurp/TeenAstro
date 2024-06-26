
#include <TeenAstroMountStatus.h>
#include "XEEPROM.hpp"
#include "SHC_text.h"
#include "SmartController.h"

static char* BreakRC[6] = { ":Qn#" ,":Qs#" ,":Qe#" ,":Qw#", ":Fo#", ":Fi#" };
static char* RC[6] = { ":Mn#" , ":Ms#" ,":Me#" ,":Mw#", ":FO#", ":FI#" };

void SmartHandController::setup(const char version[], const int pin[7], const bool active[7], const int SerialBaud, const OLED model)
{
  if (EEPROM.length() == 0)
    EEPROM.begin(1024);
  if (strlen(version) <= 19) strcpy(_version, version);

  //choose a 128x64 display supported by U8G2lib (if not listed below there are many many others in u8g2 library example Sketches)
  Serial.begin(SerialBaud);
  switch (model)
  {
  case OLED_SH1106:
    display = new U8G2_EXT_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0);
    break;
  case OLED_SSD1306:
    display = new U8G2_EXT_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0);
    break;
  case OLED_SSD1309:
    display = new U8G2_EXT_SSD1309_128X64_NONAME_F_HW_I2C(U8G2_R0);
    break;
  }
  display->begin();
  drawIntro();
  buttonPad.setup(pin, active);
  tickButtons();
  maxContrast = EEPROM.read(EEPROM_Contrast);
  display->setContrast(maxContrast);
  displayT1 = EEPROM.read(EEPROM_T1);
  if (displayT1 < 3)
  {
    displayT1 = 3;
    EEPROM.write(EEPROM_T1, displayT1);
    EEPROM.commit();
  }
  displayT2 = EEPROM.read(EEPROM_T2);
  if (displayT2 < displayT1)
  {
    displayT2 = displayT1;
    EEPROM.write(EEPROM_T2, displayT2);
    EEPROM.commit();
  }

#ifdef DEBUG_ON
  DebugSer.begin(9600);
  delay(1000);
#endif

  drawLoad();
}

void SmartHandController::update()
{
  tickButtons();
  top = millis();
  if (isSleeping())
    return;
  if (powerCycleRequired)
  {
    display->sleepOff();
    DisplayMessage(T_PRESS_KEY, T_TO_REBOOT "...", -1);
    DisplayMessage(T_DEVICE, T_WILL_REBOOT "...", 1000);

#ifdef ARDUINO_D1_MINI32
    ESP.restart();
#endif
#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
    ESP.reset();
#endif

    return;
  }
  if (ta_MountStatus.notResponding())
  {
    display->sleepOff();
    DisplayMessage("!! " T_ERROR " !!", T_NOT_CONNECTED, -1);
    DisplayMessage(T_DEVICE, T_WILL_REBOOT "...", 1000);

#ifdef ARDUINO_D1_MINI32
    ESP.restart();
#endif
#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
    ESP.reset();
#endif

  }
  if (ta_MountStatus.isAlignSelect())
  {
    char message[10] = T_STAR "#?";
    message[6] = '0' + ta_MountStatus.getAlignStar();
    DisplayLongMessage(T_SELECTASTAR, T_FROMFOLLOWINGLIST, "", message, -1);
    if (!SelectStarAlign())
    {
      DisplayMessage(T_SELECTION, T_ABORTED, -1);
      ta_MountStatus.backStepAlign();
      return;
    }
    else
    {
      ta_MountStatus.nextStepAlign();
    }
  }
  else if (top - lastpageupdate > 200)
  {
    updateMainDisplay(pages[current_page].p);
  }
  if (!ta_MountStatus.connected())
    return;
  bool moving = false;
  manualMove(moving);
  if (eventbuttons[0] == E_CLICK && !ta_MountStatus.isAligning())
  {
    for (int k = 1; k < NUMPAGES + 1; k++)
    {
      current_page++;
      if (current_page >= NUMPAGES)
        current_page = 0;
      if (pages[current_page].show)
      {
        if (pages[current_page].p == P_FOCUSER && !ta_MountStatus.hasFocuser())
        {
          pages[current_page].show = false;
          continue;
        }
        break;
      }
    }

    time_last_action = millis();
  }
  else if (moving)
  {
    return;
  }
  else if (eventbuttons[0] == E_LONGPRESS || eventbuttons[0] == E_LONGPRESSTART && !ta_MountStatus.isAligning())
  {
    if (eventbuttons[3] == E_LONGPRESS || eventbuttons[3] == E_CLICK || eventbuttons[3] == E_LONGPRESSTART)
    {
      menuTelAction();
    }
    else if (eventbuttons[1] == E_LONGPRESS || eventbuttons[3] == E_CLICK || eventbuttons[3] == E_LONGPRESSTART)
    {
      menuSpeedRate();
      time_last_action = millis();
    }
    else if (eventbuttons[4] == E_LONGPRESS || eventbuttons[3] == E_CLICK || eventbuttons[3] == E_LONGPRESSTART)
    {
      menuTelSettings();
    }
    else if (eventbuttons[6] == E_LONGPRESS || eventbuttons[3] == E_CLICK || eventbuttons[3] == E_LONGPRESSTART)
    {
      menuFocuserAction();
    }
    else if (eventbuttons[5] == E_LONGPRESS || eventbuttons[3] == E_CLICK || eventbuttons[3] == E_LONGPRESSTART)
    {
      menuFocuserSettings();
    }
    exitMenu = false;
    time_last_action = millis();
  }
  else if (eventbuttons[0] == E_CLICK && ta_MountStatus.isAlignRecenter())
  {
    TeenAstroMountStatus::AlignReply reply = ta_MountStatus.addStar();
    switch (reply)
    {
    case TeenAstroMountStatus::AlignReply::ALIR_FAILED1:
      DisplayMessage(T_ALIGNMENT, T_FAILED"!", -1);
      break;
    case TeenAstroMountStatus::AlignReply::ALIR_FAILED2:
      DisplayMessage(T_ALIGNMENT, T_WRONG"!", -1);
      break;
    case TeenAstroMountStatus::AlignReply::ALIR_DONE:
      DisplayMessage(T_ALIGNMENT, T_SUCESS"!", -1);
      break;
    case TeenAstroMountStatus::AlignReply::ALIR_ADDED:
      DisplayMessage(T_STARADDED, "=>", 1000);
      break;
    }
  }
}

void SmartHandController::tickButtons()
{
  buttonPad.tickButtons();
}

bool SmartHandController::buttonPressed()
{
  return buttonPad.buttonPressed();
}

bool SmartHandController::isSleeping()
{
  if (forceDisplayoff)
  {
    if (!buttonPad.shiftPressed())
    {
      bool moving = false;
      manualMove(moving);
      return true;
    }
    else
      forceDisplayoff = false;
  }
  if (buttonPressed())
  {
    time_last_action = millis();
    if (sleepDisplay)
    {
      display->setContrast(maxContrast);
      display->sleepOff();
      sleepDisplay = false;
      lowContrast = false;
      buttonPad.setControlerMode();
      return true;
    }
    if (lowContrast)
    {
      lowContrast = false;
      display->setContrast(maxContrast);
      buttonPad.setControlerMode();
      return true;
    }
  }
  else if (sleepDisplay)
  {
    return true;
  }
  else if ((top - time_last_action) / 10000 > displayT2)
  {
    display->sleepOn();
    sleepDisplay = true;
    buttonPad.setMenuMode();
    return false;
  }
  else if ((top - time_last_action) / 10000 > displayT1 && !lowContrast)
  {
    display->setContrast(0);
    lowContrast = true;
    buttonPad.setMenuMode();
    return true;
  }
  return false;
}

void SmartHandController::manualMove(bool &moving)
{
  moving = ta_MountStatus.getTrackingState() == TeenAstroMountStatus::TRK_SLEWING ||
    ta_MountStatus.getParkState() == TeenAstroMountStatus::PRK_PARKING ||
    ta_MountStatus.isSpiralRunning();
  if (moving)
  {
    bool stop = (eventbuttons[0] == E_LONGPRESS || eventbuttons[0] == E_LONGPRESSTART || eventbuttons[0] == E_DOUBLECLICK) ? true : false;
    int it = 1;
    while (!stop && it < 5)
    {
      stop = (eventbuttons[it] == E_LONGPRESS || eventbuttons[it] == E_CLICK || eventbuttons[it] == E_LONGPRESSTART);
      it++;
    }
    if (stop)
    {
      Ser.print(":Q#");
      Ser.flush();
      time_last_action = millis();
      display->sleepOff();
      ta_MountStatus.backStepAlign();
      return;
    }
  }
  else
  {
    buttonCommand = false;
    for (int k = 1; k < 7; k++)
    {
      if (Move[k - 1] && (eventbuttons[k] == E_LONGPRESSSTOP || eventbuttons[k] == E_NONE))
      {
        buttonCommand = true;
        Move[k - 1] = false;
        if (k < 5)
          SetBoolLX200(BreakRC[k - 1]);
        else
          Move[k - 1] = !(SetBoolLX200(BreakRC[k - 1]) == LX200VALUESET);
        continue;
      }
      else if (eventbuttons[0] == E_NONE && !Move[k - 1] && (eventbuttons[k] == E_LONGPRESS || eventbuttons[k] == E_CLICK || eventbuttons[k] == E_LONGPRESSTART))
      {
        buttonCommand = true;

        if (k < 5)
        {
          if (!telescoplocked)
          {
            Move[k - 1] = true;
            SetBoolLX200(RC[k - 1]);
          }
        }
        else if (!focuserlocked)
          Move[k - 1] = (SetBoolLX200(RC[k - 1]) == LX200VALUESET);
        continue;
      }
      moving = moving || Move[k - 1];
    }
    if (buttonCommand)
    {
      time_last_action = millis();
      return;
    }
  }
}
