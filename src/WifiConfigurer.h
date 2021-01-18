#ifndef _WIFI_CONNECTION_h
#define _WIFI_CONNECTION_h

#ifndef WIFI_STATE_LED 
#define WIFI_STATE_LED 4
#endif

#ifndef WIFI_RESET_CONFIG
#define WIFI_RESET_CONFIG 5
#endif

#define WM_DEBUG_LEVEL DEBUG_ERROR

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          
#include <Ticker.h>


class WifiConfigurer
{
 public:
	 void Invoke();
};

#endif

