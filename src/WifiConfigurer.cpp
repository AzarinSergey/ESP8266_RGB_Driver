#include "WifiConfigurer.h"

Ticker *ticker;

void WifiConfigurer::Invoke()
{
	//blink 'ON' while not connected to Wifi network
	pinMode(WIFI_STATE_LED, OUTPUT);
	ticker = new Ticker();
	ticker->attach(0.6, []() {
		int state = digitalRead(WIFI_STATE_LED);
		digitalWrite(WIFI_STATE_LED, !state);
	});

	//configure connection in auto or manual throught portal
	WiFiManager wifiManager;

	//reset if requested
	pinMode(WIFI_RESET_CONFIG, INPUT);
	if(digitalRead(WIFI_RESET_CONFIG))
		wifiManager.resetSettings();

	wifiManager.setAPCallback( [] (WiFiManager *m) {
		Serial.println("Entered config mode, wifi config portal started.");
		Serial.println(WiFi.softAPIP());
		Serial.println(m->getConfigPortalSSID());
	});

	if (!wifiManager.autoConnect()) {
		Serial.println("failed to connect and hit timeout");
		ESP.reset();
		delay(1000);
	}

	Serial.println("Connected to endpoint...");

	//stop blink and turn on weak light
	ticker->detach();
	ticker->~Ticker();
	digitalWrite(WIFI_STATE_LED, LOW);
	analogWrite(WIFI_STATE_LED, 150);
}

