#define MQTT_SERVER "192.168.1.2"
#define MQTT_PORT 1883
#define MQTT_USER "username"
#define MQTT_PASSWORD "password12345"
#define MQTT_CONFIGURATION_TOPIC "energy_monitor/config"

// #define USE_WIFI_MANAGER

#ifndef USE_WIFI_MANAGER
#define WIFI_SSID "<your wifi ssid>"
#define WIFI_PASSWORD "<your wifi password"
#define NETWORK_HOSTNAME "energymonitor"
#define NETWORK_IP_ADDRESS IPAddress(192, 168, 1, 99)
#define NETWORK_IP_SUBNET IPAddress(255, 255, 255, 0)
#define NETWORK_IP_GATEWAY IPAddress(192, 168, 1, 1)
#define NETWORK_IP_DNS IPAddress(192, 168, 1, 1)
#endif

#define DEBUG
#define SYSLOG_SERVER "192.168.1.2"
#define SYSLOG_PORT 514
#define SYSLOG_MYHOSTNAME "esp01"
#define SYSLOG_MYAPPNAME "energymonitor"
