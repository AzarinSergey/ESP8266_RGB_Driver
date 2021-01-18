#include "RgbDriver.h"

Ticker rTicker, gTicker, bTicker;

static PwmChannelState 
rChannel { RGB_R_LED, 0, &rTicker, 0, 0, 0 },
gChannel { RGB_G_LED, 0, &gTicker, 0, 0, 0 },
bChannel { RGB_B_LED, 0, &bTicker, 0, 0, 0 };

RgbDriver::RgbDriver()
{
	Serial.println("RgbDriver()");
	pinMode(RGB_R_LED, OUTPUT);
	pinMode(RGB_G_LED, OUTPUT);
	pinMode(RGB_B_LED, OUTPUT);
}

void RgbDriver::Switch(uint8_t value)
{
	Serial.printf("Switch all to %d \n", value);
	Serial.println("Disable pwm...");
	DisablePwm(&rChannel);
	DisablePwm(&gChannel);
	DisablePwm(&bChannel);

	Serial.println("Set digitalWrite...");
	digitalWrite(rChannel.Pin, value);
	digitalWrite(gChannel.Pin, value);
	digitalWrite(bChannel.Pin, value);

	rChannel.Value = value;
	gChannel.Value = value;
	bChannel.Value = value;
}

void RgbDriver::Switch(RgbLedPin pin, uint8_t value)
{
	PwmChannelState* channel;
	switch (pin)
	{
		case Green: channel = &gChannel; break;
		case Red:	channel = &rChannel; break;
		case Blue:	channel = &bChannel; break;
		default: return;
	}
	
	DisablePwm(channel);
	digitalWrite(channel->Pin, value);
	channel->Value = value;
}

void RgbDriver::SetPwmRange(RgbLedPin pin, uint32_t min, uint32_t max, uint32_t speed)
{
	PwmChannelState* channel;
	switch (pin)
	{
		case Green: channel = &gChannel; break;
		case Red:	channel = &rChannel; break;
		case Blue:	channel = &bChannel; break;
		default: return;
	}

	channel->PwmMin = min;
	channel->PwmMax = max;
	channel->PwmStep = 16;
	channel->PwmTicker->attach_ms(speed, UpdatePwm, channel);
}

void RgbDriver::SetPwmValue(RgbLedPin pin, uint32_t value)
{
	PwmChannelState* channel;
	switch (pin)
	{
		case Green: channel = &gChannel; break;
		case Red:	channel = &rChannel; break;
		case Blue:	channel = &bChannel; break;
		default: return;
	}

	DisablePwm(channel);
	analogWrite(channel->Pin, value);
	channel->Value = value;
}

void RgbDriver::DisablePwm(PwmChannelState* channel)
{
	if (channel->PwmTicker->active())
		channel->PwmTicker->detach();
}

void RgbDriver::UpdatePwm(PwmChannelState* channel)
{
	if (channel->PwmUpDirection && channel->Value >= channel->PwmMax)
	{
		channel->PwmUpDirection = false;
	}

	if (!channel->PwmUpDirection && channel->Value <= channel->PwmMin)
	{
		channel->PwmUpDirection = true;
	}

	if (channel->PwmUpDirection)
	{
		channel->Value = channel->Value + channel->PwmStep;
	}
	else
	{
		channel->Value = channel->Value - channel->PwmStep;
	}

	analogWrite(channel->Pin, channel->Value);
}