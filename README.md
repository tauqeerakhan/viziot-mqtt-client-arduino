# Arduino Client for VizIoT MQTT

This library provides a client for send JSON data and get command from a VizIoT.com.
This library extended PubSubClient https://github.com/knolleary/pubsubclient .

## Examples

The library comes with a number of example sketches. See File > Examples > VizIoTMqttClient
within the Arduino application.

Full API documentation is available here: http://viziot.com/pages/docs/

## Compatible Hardware

The library uses the Arduino Ethernet Client api for interacting with the
underlying network hardware. This means it Just Works with a growing number of
boards and shields, including:

 - Arduino Ethernet
 - Arduino Ethernet Shield
 - Arduino YUN – use the included `YunClient` in place of `EthernetClient`, and
   be sure to do a `Bridge.begin()` first
 - Arduino WiFi Shield - if you want to send packets > 90 bytes with this shield,
   enable the `MQTT_MAX_TRANSFER_SIZE` define in `PubSubClient.h`.
 - Sparkfun WiFly Shield – [library](https://github.com/dpslwk/WiFly)
 - TI CC3000 WiFi - [library](https://github.com/sparkfun/SFE_CC3000_Library)
 - Intel Galileo/Edison
 - ESP8266

The library cannot currently be used with hardware based on the ENC28J60 chip –
such as the Nanode or the Nuelectronics Ethernet Shield. For those, there is an
[alternative library](https://github.com/njh/NanodeMQTT) available.

## License

This code is released under the MIT License.
