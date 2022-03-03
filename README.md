# ESP8266_Blynk_WOL_Server

This app is written for my NAS server and supports only one device!
It requires a static IP for the WOL server (configured in the source code) and for the device (you need to configure it yourself).

Do you need some device with ESP-8266/ESP-8285 (I use WeMos D1 mini Lite), [Blynk](https://www.blynk.cc/getting-started/) application (Scan the application QR code you will find below!), and device configured for Wake on LAN (I can not help you with that, see bios... use google...)
To upload the software, use [Arduino IDE](https://www.arduino.cc/en/main/software) with the [ESP8266 plug-in](https://github.com/esp8266/Arduino) and [Ping Library](https://github.com/dancol90/ESP8266Ping) installed.

---

### The following variables need to be changed:

```C++

#define BLYNK_TEMPLATE_ID "" //your blynk template id
#define BLYNK_DEVICE_NAME "" //your blynk device name
#define BLYNK_AUTH_TOKEN "" //your blynk auth token

//WiFi config
const char auth[] = "Blynk_AuthToken";
const char ssid[] = "WiFi_SSID";
const char pass[] = "WiFi_Password";

const IPAddress ip(192, 168, 0, 123);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress bcastAddr(192, 168, 0, 255);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress dns(1, 1, 1, 1);

//WOL device config
const IPAddress device_ip(192, 168, 0, 234);
byte macAddr[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

//Alert config
const char email[] = "example@example.com";
const char device_name[] = "NAS";
const uint16_t boot_time = 45;	//number for countdown (It does not represent seconds, but it's close, read the known issues!)
```

| Variable name     | Default value       | Comment                                                                                            |
| ----------------- | ------------------- | -------------------------------------------------------------------------------------------------- |
| BLYNK_TEMPLATE_ID | ""                  | The template ID can be found when you create a device on Blynk                                     |
| BLYNK_DEVICE_NAME | ""                  | The device name can be found when you create a device on Blynk                                     |
| BLYNK_AUTH_TOKEN  | ""                  | The Blynk Auth Token can be found when you create a device on Blynk                                |
| auth              | Blynk_AuthToken     | Blynk Auth Token (same as the previous one)                                                        |
| ssid              | WiFi_SSID           | Your WiFi network name                                                                             |
| pass              | WiFi_Password       | Your WiFi network password                                                                         |
| ip                | 192.168.0.123       | Static IP for your WOL Server                                                                      |
| gateway           | 192.168.0.1         | Your Gateway IP address                                                                            |
| bcastAddr         | 192.168.0.255       | Your Broadcast IP address                                                                          |
| subnet            | 255.255.255.0       | Your Subnet Mask IP address                                                                        |
| dns               | 1.1.1.1             | IP address of your preferred DNS server                                                            |
| device_ip         | 192.168.0.234       | Static IP address of device to be turned on (used for ping)                                        |
| macAddr           | aa:bb:cc:dd:ee:ff   | Mac address of device to be turned on (important for the magic packet)                             |
| email             | example@example.com | Email address for notifications (when the device could not be turned on within the set time limit) |
| device_name       | NAS                 | The short name of your device that turns on                                                        |
| boot_time         | 45                  | Maximum time to wait for the device to turn on (used for notification, read the known issues!)     |

---

## Blynk device configuration:

I don't think there is yet a way to share templates on the new Blynk,
but you can connect to this account (email: blynktestgithub@gmail.com, password: 123Blynkaccount),
create a new device based on the "WakeOnLan" template,
and then transfer the device to your account.

## Known Issues

variable **boot_time** does not represent seconds, It's about one number for two seconds. It is caused by waiting for Ping.

Your device probably will not respond to the **magic packet** after a power failure. Manual turn-on is required.

## License Information

This product is _**open source**_!

The **code** is released under the MIT license. See the included LICENSE file for more information.

Distributed as-is; no warranty is given.

## Donate

Do you like my work? [Let me know about it and support my other projects!](https://www.patreon.com/pnoxi)

Or just drink beer on my honor :wink:
