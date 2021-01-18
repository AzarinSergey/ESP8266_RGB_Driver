#include "src/WifiConfigurer.h"
#include "src/RgbDriver.h"
#include <ESP8266WebServer.h>

#define STRING_EMPTY(c) ((c != NULL) && (c[0] == '\0'))

using namespace std;

WifiConfigurer wifiConfig;
RgbDriver RgbDriver;
std::unique_ptr<ESP8266WebServer> Server;

void ping() {
    Server->send(200, "text/plain", "pong!");
}

void setRgbPwmValue()
{
    Server->send(200, "text/plain", "Ok");
    int red = atoi(Server->arg("red").c_str());
    int green = atoi(Server->arg("green").c_str());
    int blue = atoi(Server->arg("blue").c_str());

    RgbDriver.SetPwmValue(Red, red);
    RgbDriver.SetPwmValue(Green, green);
    RgbDriver.SetPwmValue(Blue, blue);
}

vector<uint32_t> Split(const char* str, char separator = ',')
{
    vector<uint32_t> result;

    do
    {
        const char* begin = str;

        while (*str != separator && *str)
            str++;

        result.push_back(atol(string(begin, str).c_str()));

    } while (*str++ != 0);

    return result;
}

//http://192.168.0.xxx/rgb/pwm/set?red=1020&green=500&blue=310
//http://192.168.0.xxx/rgb/pwm/auto?red=40,1020,40&green=40,1020,40&blue=40,1020,40
//http://192.168.0.xxx/rgb/switch/off
void setRgbPwmAuto()
{
    Server->send(200, "text/plain", "Ok");

    const char separator = ',';

    const char* r = Server->arg("red").c_str();
    const char* g = Server->arg("green").c_str();
    const char* b = Server->arg("blue").c_str();

    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

    if (!STRING_EMPTY(r))
    {
        vector<uint32_t> rValues = Split(r);
        RgbDriver.SetPwmRange(Red, rValues[0], rValues[1], rValues[2]);
    }

    if (!STRING_EMPTY(g))
    {
        vector<uint32_t> gValues = Split(g);
        RgbDriver.SetPwmRange(Green, gValues[0], gValues[1], gValues[2]);
    }

    if (!STRING_EMPTY(b))
    {
        vector<uint32_t> bValues = Split(b);
        RgbDriver.SetPwmRange(Blue, bValues[0], bValues[1], bValues[2]);
    }
}

void setup() {
    Serial.begin(74880);
    Serial.println("Driver firmware setup running...");
   
    wifiConfig.Invoke();

    Server.reset(new ESP8266WebServer(WiFi.localIP(), 80));
    Server->on("/ping", ping);

    Server->on("/rgb/switch/on", []() { RgbDriver.Switch(1); Server->send(200, "text/plain", "Ok"); });
    Server->on("/rgb/switch/off", []() { RgbDriver.Switch(0); Server->send(200, "text/plain", "Ok"); });
    
    Server->on("/rgb/pwm/set", setRgbPwmValue);
    Server->on("/rgb/pwm/auto", setRgbPwmAuto);

    Server->begin();

    Serial.println("HTTP server started");
    Serial.println(WiFi.localIP());
}

void loop() {
    Server->handleClient();
}