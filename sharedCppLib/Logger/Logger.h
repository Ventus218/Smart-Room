#ifndef __LOGGER__
#define __LOGGER__

#include <Arduino.h>

enum class LogLevel { INFO = 0, NOTICE = 1, WARNING = 2, ERROR = 3 };

class Logger {
private:
  static LogLevel logLevel;
  static bool hasBeenInitialized;

  Logger();
  static String logLevelToString(LogLevel logLevel);

public:
  static void init(unsigned long baudRate, LogLevel logLevel);
  static void log(String message);
  static void log(String message, LogLevel logLevel);
};

#endif