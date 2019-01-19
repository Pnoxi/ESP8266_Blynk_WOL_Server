# ESP8266_Blynk_WOL_Server



This app is written for my NAS server and supports only one device!
It requires a static IP for the WOL server (configured in the source code) and for the device (you need to configure it yourself).

Do you need some device with ESP-8266/ESP-8285 (I use WeMos D1 mini Lite), [Blynk](https://www.blynk.cc/getting-started/) application (Scan the application QR code you will find below!), and device configured for Wake on LAN (I can not help you with that, see bios... use google...)
To upload the software, use [Arduino IDE](https://www.arduino.cc/en/main/software) with the [plug-in](https://github.com/esp8266/Arduino) installed.

--------------------

### The following variables need to be changed:

```C++
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
const uint16_t boot_time = 45;	//seconds
```

Variable name | Default value | Comment
----- | ---------- | -------------------------
auth | Blynk_AuthToken | Blynk Auth Token (You get after the project was created in Blynk)
ssid | WiFi_SSID | Your WiFi network name
pass | WiFi_Password | Your WiFi network password
ip | 192.168.0.123 | Static IP for your WOL Server
gateway | 192.168.0.1 | Your Gateway IP address
bcastAddr | 192.168.0.255 | Your Broadcast IP address
subnet | 255.255.255.0 | Your Subnet Mask IP address
dns | 1.1.1.1 | IP address of your preferred DNS server
device_ip | 192.168.0.234 | Static IP address of device to be turned on (used for ping)
macAddr | aa:bb:cc:dd:ee:ff | Mac address of device to be turned on (important for the magic packet)
email | example@example.com | Email address for notifications (when the device could not be turned on within the set time limit)
device_name | NAS | The short name of your device that turns on
boot_time | 45 | Maximum time to wait for the device to turn on (used for notification)
--------------------

## QR code of the Blynk application (Click for a larger picture):
[![Blynk_QR](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/QR_thumbnail.png)](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/QR.png)


## App preview (Click for a larger picture):
[![Demo](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/1_thumbnail.png)](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/1.png)
[![Demo](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/2_thumbnail.png)](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/2.png)
[![Demo](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/3_thumbnail.png)](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/3.png)
[![Demo](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/4_thumbnail.png)](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/4.png)
[![Demo](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/5_thumbnail.png)](https://www.emsit.sk/SharedContent/GitHub/ESP8266_Blynk_WOL_Server/5.png)


License Information
--------------------

This product is _**open source**_!

The **code** is released under the MIT license. See the included LICENSE file for more information.

Distributed as-is; no warranty is given.


Donate
--------------------

Do you like my work? [Let me know about it and support my other projects!](https://www.patreon.com/pnoxi)

Or just drink beer on my honor :wink:
