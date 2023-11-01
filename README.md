# Energy monitor

Configuration 

1. Create include/secrets.h (see `examples/secrets.h` for an example).
1. Check the defines in platformio.ini and update them if needed (e.g. if you are using a different board or display).
1. Build and upload the firmware to the board.
1. Publish the configuration JSON to MQTT topic defined in `secrets.h` (see above). I have used MQTT Explorer to achieve this. An example config can be found in `examples/config.json`.

