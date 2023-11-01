# Energy monitor

A simple device created from an [ESP32 with 3.5" capacitive touch LCD](https://www.aliexpress.com/item/1005004632953455.html?spm=a2g0o.order_list.order_list_main.66.858e1802IwIcM3) using [Shelly EM](https://www.shelly.com/en/products/shop/shelly-em-120a/shelly-em-50a) as a power meter. 

The device subscribes to MQTT topic where Shelly publishes the current power consumption. It then displays the total consumption together with the remaining power available. If there are appliances defined with their power consumption then the device is able to display warning whenever the current consumption is approaching the limit and some of the appliances should not be turned on.

It automatically turns the display (backlight) off after 30 seconds but can be turned on/off anytime by just touching it. In case of any warnings it turns on automatically. There is also a hysteresis implemented for the 10-seconds maximum of power consumption so that the warnings won't be flickering and constantly changing.

# Compiling

1. Create include/secrets.h (see the `client/include/secrets.example.h` for an example).
1. Check the defines in platformio.ini and update them if needed (e.g. if you are using a different board or display).
1. Build and upload the firmware to the board.
1. Publish the configuration JSON to a retained MQTT topic defined in `secrets.h` (see above). I have used MQTT Explorer to do this. An example config can be found in `client/config.example.json`.
1. Enjoy

# Some details

Having the config in MQTT means that any change is applied immediately. This is nice because it allows easy configuration changes without a need for restart.

The configuration topic should always be published as "retained" so that the configuration is applied after a restart.

# Caveats

`MQTT_MAX_PACKET_SIZE` must be defined in platformio.ini `build_flags` to be able to receive the configuration JSON. I have used the following value: `-D MQTT_MAX_PACKET_SIZE=8192`

This value should account for a largest JSON config you're going to use. 

> ⚠️ If the device gets seemingly stuck at the "Loading config" screen then this value is probably set too small and the PubSubClient library is skipping the MQTT message silently as it's too large. 

# Examples

![IMG_20231101_100221](https://github.com/misch2/Energy-monitor/assets/16558674/a893d8ec-f5e6-4d91-8adb-cf10c0dc8703)
![IMG_20231101_100241](https://github.com/misch2/Energy-monitor/assets/16558674/406ddcb1-7218-4f55-a8d9-5e6aeceaf95d)
