# Energy monitor

A simple device created from an [ESP32 with 3.5" capacitive touch LCD](https://www.aliexpress.com/item/1005004632953455.html?spm=a2g0o.order_list.order_list_main.66.858e1802IwIcM3) using [Shelly EM](https://www.shelly.com/en/products/shop/shelly-em-120a/shelly-em-50a) as a power meter. 

The device subscribes to MQTT topic where Shelly publishes the current power consumption. It then displays the total consumption together with the remaining power available. If there are appliances defined with their power consumption then the device is able to display warning whenever the current consumption is approaching the limit and some of the appliances should not be turned on.

# Compiling

1. Create include/secrets.h (see `examples/secrets.h` for an example).
1. Check the defines in platformio.ini and update them if needed (e.g. if you are using a different board or display).
1. Build and upload the firmware to the board.
1. Publish the configuration JSON to a retained MQTT topic defined in `secrets.h` (see above). I have used MQTT Explorer to do this. An example config can be found in `examples/config.json`.
1. Enjoy

# Some details

Having the config in MQTT means that any change is applied immediately. This is nice because it allows easy configuration changes without a need for restart.

The configuration topic should always be published as "retained" so that the configuration is applied after a restart.

# Caveats

`MQTT_MAX_PACKET_SIZE` must be defined in platformio.ini `build_flags` to be able to receive the configuration JSON. I have used the following value: `-D MQTT_MAX_PACKET_SIZE=8192`

This value should account for a largest JSON config you're going to use. 

> ⚠️ If the device gets seemingly stuck at the "Loading config" screen then this value is probably set too small and the PubSubClient library is skipping the MQTT message silently as it's too large. 

# Examples

<img src="https://github.com/misch2/Energy-monitor/assets/16558674/bc73a087-413b-44f0-8ffa-ebcfaee5c718" width="250">
<img src="https://github.com/misch2/Energy-monitor/assets/16558674/aca041fa-b3dc-45d0-a086-815e9758d6d3" width="250">
