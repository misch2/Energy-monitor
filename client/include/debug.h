#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <Syslog.h>
#include <WiFi.h>

#include "secrets.h"

class Logger {
 private:
  WiFiUDP& udpClient;
  Syslog& syslog;

 public:
  Logger(WiFiUDP& udpClient, Syslog& syslog);

  void debug(const char* format, ...);

  void setEnabled(bool enable);
  bool isEnabled() const;

 private:
  bool debugEnabled;
  bool syslogEnabled;
};

extern Logger logger;

#endif  // DEBUG_H