#pragma once

#ifndef VERSION
#define VERSION 240
#endif

// firmware info, these are returned by the ":GV?#" commands
#define FirmwareDate    __DATE__
#define FirmwareNumber  "1.3.3"
#define FirmwareName    "TeenAstro"
#define FirmwareTime    "00:00:00"
// forces initialialization of a host of settings in XEEPROM. OnStep does this automatically, most likely, you will want to leave this alone
#define initKey     000000001                       // unique identifier for the current initialization format, do not change
