#ifndef _RGBDRIVER_h
#define _RGBDRIVER_h

#ifndef RGB_R_LED
#define RGB_R_LED 13
#endif 

#ifndef RGB_G_LED
#define RGB_G_LED 12
#endif

#ifndef RGB_B_LED
#define RGB_B_LED 14
#endif

#include <c_types.h>
#include <Arduino.h>
#include <Ticker.h>

struct PwmChannelState
{
	uint8 Pin;
	uint32_t Value;
	Ticker* PwmTicker;
	uint32_t PwmMin;
	uint32_t PwmMax;
	uint32_t PwmStep;
	bool PwmUpDirection;
	//mode: 0 - degital out, 1 - pwm out, 2 - pwm out auto
};

enum RgbLedPin { Green = RGB_G_LED, Red = RGB_R_LED, Blue = RGB_B_LED };

class RgbDriver
{
 public:
	 RgbDriver();
	 
	 void Switch(uint8_t value);

	 void Switch(RgbLedPin pin, uint8_t value);

	 void SetPwmRange(RgbLedPin pin, uint32_t min, uint32_t max, uint32_t speed = 50);

	 void SetPwmValue(RgbLedPin pin, uint32_t value);

 private:
	void DisablePwm(PwmChannelState* channel);
	static void UpdatePwm(PwmChannelState* channel);
};

#endif