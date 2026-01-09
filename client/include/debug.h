#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <Syslog.h>
#include <WiFi.h>

#include "secrets.h"

// other modules
extern WiFiUDP udpClient;
extern Syslog syslog;

class Logger {
 public:
  Logger();

  void debug(const char* format, ...);

  void setEnabled(bool enable);
  bool isEnabled() const;

 private:
  bool debugEnabled;
  bool syslogEnabled;
};

extern Logger logger;

#endif  // DEBUG_H