//#define DEBUG
#ifdef DEBUG
#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
//#define ENABLE_DEBUG_PING
#endif

#define BLYNK_NO_BUILTIN	//disable built-in analog and digital operations.
//#define BLYNK_NO_INFO		//disable providing info about device to the server. (saving time)

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <ESP8266Ping.h>

//blynk colors
#define BLYNK_GREEN		"#23C48E"
#define BLYNK_BLUE		"#04C0F8"
#define BLYNK_YELLOW	"#ED9D00"
#define BLYNK_RED		"#D3435C"
#define BLYNK_DARK_BLUE	"#5F7CD8"

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

//WOL
#define MAGIC_PACKET_LENGTH 102
#define PORT_WAKEONLAN 9
byte magicPacket[MAGIC_PACKET_LENGTH];
unsigned int localPort = 9;

WiFiUDP udp;

//pins
#define STATE_PIN		V0
#define BUTTON_PIN		V1
#define PING_TIME_PIN	V2
#define RSSI_PIN		V3

//state
struct WOLServerState {
	bool IsOnline;
	uint16_t boot_time;
	bool boot_error;
	uint16_t ping;
	uint32_t previousMillis;
	uint32_t interval;
};
WOLServerState currentState = { false, 0, false, 0, 0, 5000UL };

void setup() {
#ifdef DEBUG
	Serial.begin(115200);
#endif

	connectWiFi();
	connectBlynk();

	//if (Blynk.connected()) {
		if (udp.begin(localPort) == 1) {
			BLYNK_LOG("udp begin OK");
			buildMagicPacket();
		} else {
			delay(500);
			ESP.restart();
		}
	//}
}

void connectWiFi() {
	WiFi.mode(WIFI_STA);
	WiFi.hostname("WOL server");
	WiFi.config(ip, dns, gateway, subnet);
	WiFi.begin(ssid, pass);

	int count = 0;
	while (WiFi.status() != WL_CONNECTED) {
		delay(250);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(250);
		digitalWrite(LED_BUILTIN, LOW);

		count++;
		if (count > 20) {
			delay(500);
			ESP.restart();
		}
	}

	//BLYNK_LOG("WiFi connected, IP: %s", WiFi.localIP().toString());
}

void connectBlynk() {
	Blynk.config(auth);
	Blynk.disconnect();

	int count = 0;
	while (Blynk.connect(10000) == false) {
		delay(250);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(250);
		digitalWrite(LED_BUILTIN, LOW);

		count++;
		if (count > 20) {
			delay(500);
			ESP.restart();
		}
	}

	BLYNK_LOG("Blynk connected");
}

void loop() {
	// Reconnect WiFi
	if (WiFi.status() != WL_CONNECTED) {
		connectWiFi();
		return;
	}

	// Reconnect to Blynk Cloud
	if (!Blynk.connected()) {
		connectBlynk();
		return;
	}

	uint32_t currentMillis = millis();
	if (currentMillis - currentState.previousMillis >= currentState.interval) {
		currentState.previousMillis = currentMillis;

		if (currentState.boot_time == 0) {
			currentState.interval = 5000UL;
		} else {
			currentState.boot_time--;
			if (currentState.boot_time == 0) {
				currentState.boot_error = true;
				Blynk.email(email, "{DEVICE_NAME} : Alert", String(device_name) + " could not be turned on!");
			}
		}

		if (Ping.ping(device_ip, 1)) {
			currentState.IsOnline = true;
			currentState.boot_error = false;
			currentState.boot_time = 0;
			currentState.ping = Ping.averageTime();
		} else {
			currentState.IsOnline = false;
			currentState.ping = 0;
		}
	}

	Blynk.run();
}

// Generate magic packet
void buildMagicPacket() {
	memset(magicPacket, 0xFF, 6);
	for (int i = 0; i < 16; i++) {
		int ofs = i * sizeof(macAddr) + 6;
		memcpy(&magicPacket[ofs], macAddr, sizeof(macAddr));
	}
}

//BLYNK_CONNECTED() {
//	Blynk.syncVirtual(BUTTON_PIN);
//}

// BOOT PC button handler of application
BLYNK_WRITE(BUTTON_PIN) {
	if (!currentState.IsOnline && currentState.boot_time == 0) {
		BLYNK_LOG("AppButtonWakeOnLan: value=%d", param.asInt());
		udp.beginPacket(bcastAddr, PORT_WAKEONLAN);
		udp.write(magicPacket, MAGIC_PACKET_LENGTH);
		udp.endPacket();

		currentState.boot_time = boot_time;
		currentState.interval = 1000UL;
	}
}

BLYNK_READ(STATE_PIN) {
	Blynk.virtualWrite(RSSI_PIN, WiFi.RSSI());
	Blynk.virtualWrite(PING_TIME_PIN, currentState.ping);

	if (currentState.IsOnline) {
		Blynk.setProperty(STATE_PIN, "color", BLYNK_GREEN);
		Blynk.virtualWrite(STATE_PIN, String(device_name) + " is Online");

		Blynk.setProperty(BUTTON_PIN, "color", BLYNK_DARK_BLUE);
		Blynk.setProperty(BUTTON_PIN, "offLabel", String(device_name) + " running...");
		Blynk.setProperty(BUTTON_PIN, "onLabel", String(device_name) + " running...");
	} else if (!currentState.IsOnline && currentState.boot_time > 0) {
		Blynk.setProperty(STATE_PIN, "color", BLYNK_BLUE);
		Blynk.virtualWrite(STATE_PIN, "Waiting for ping...");

		Blynk.setProperty(BUTTON_PIN, "color", BLYNK_YELLOW);
		Blynk.setProperty(BUTTON_PIN, "offLabel", currentState.boot_time);
		Blynk.setProperty(BUTTON_PIN, "onLabel", "Waiting for ping...");
	} else if (!currentState.IsOnline && currentState.boot_time == 0 && currentState.boot_error) {
		Blynk.setProperty(STATE_PIN, "color", BLYNK_RED);
		Blynk.virtualWrite(STATE_PIN, "Oops! Something happened, Try It Again!");

		Blynk.setProperty(BUTTON_PIN, "color", BLYNK_YELLOW);
		Blynk.setProperty(BUTTON_PIN, "offLabel", "Try It Again");
		Blynk.setProperty(BUTTON_PIN, "onLabel", "Magic Packet has been sent");
	} else {
		Blynk.setProperty(STATE_PIN, "color", BLYNK_RED);
		Blynk.virtualWrite(STATE_PIN, String(device_name) + " is Offline");

		Blynk.setProperty(BUTTON_PIN, "color", BLYNK_BLUE);
		Blynk.setProperty(BUTTON_PIN, "offLabel", "Turn On");
		Blynk.setProperty(BUTTON_PIN, "onLabel", "Magic Packet has been sent");
	}
}
