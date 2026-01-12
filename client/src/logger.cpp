#include "logger.h"

Logger::Logger(WiFiUDP& udpClient, Syslog& syslog) : udpClient(udpClient), syslog(syslog), debugEnabled(false), syslogEnabled(false) {
#ifdef DEBUG
  debugEnabled = true;
  #ifdef SYSLOG_SERVER
  syslogEnabled = true;
  #endif
#endif
}

void Logger::debug(const char* format, ...) {
  if (!debugEnabled) {
    return;
  }

  // Format the string first
  va_list args;
  va_start(args, format);
  char buffer[256];
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  // Print to Serial
  Serial.print(buffer);
  Serial.print('\n');
  Serial.flush();

  // Send to syslog if enabled and WiFi connected
  if (syslogEnabled && WiFi.status() == WL_CONNECTED) {
    va_start(args, format);
    for (int i = 0; i < 3; i++) {
      if (syslog.log(LOG_INFO, buffer)) {
        break;
      } else {
        delay(100);
      }
    }
    va_end(args);
  } else if (syslogEnabled) {
    Serial.println(" (no syslog, WiFi not connected)");
  }
}

void Logger::setEnabled(bool enable) {
  debugEnabled = enable;
}

bool Logger::isEnabled() const {
  return debugEnabled;
}

// WiFi is connected now => all messages go to syslog too.
// But FIXME, first few UDP packets are not sent:
// [1113][E][WiFiUdp.cpp:183] endPacket(): could not send data: 12
