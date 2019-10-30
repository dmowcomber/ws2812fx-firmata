#include <ConfigurableFirmata.h>
#include "WS2812FXFirmata.h"

#define SERIAL_DEBUG
#include "utility/firmataDebug.h"

// TODO: make this a field on the FirmataNeopixels class
WS2812FX ws2812fx = WS2812FX(DEFAULT_NUMPIXELS, DEFAULT_PIN, NEO_GRB + NEO_KHZ800);

// TODO: rename class to Ws2812fxFirmata
FirmataNeopixels::FirmataNeopixels()
{
   pixelCount = DEFAULT_NUMPIXELS;
   pixelPin = DEFAULT_PIN;
   colorDepth = 3;
}

// FirmataFeature interface functions. Required for any Firmata plugin.
boolean FirmataNeopixels::handlePinMode(byte pin, int mode)
{
    return false;
}

void FirmataNeopixels::handleCapability(byte pin)
{
}

void FirmataNeopixels::reset()
{
}

void FirmataNeopixels::service()
{
   ws2812fx.service();
}

boolean FirmataNeopixels::handleSysex(byte command, byte argc, byte *argv)
{
    if (command == NEOPIXEL_CMD)
    {
        DEBUG_PRINT("Got command: ");

        byte pixelCommand;
        pixelCommand = argv[0];

        if (pixelCommand == PIXEL_CONFIG)
        {
           DEBUG_PRINTLN("PIXEL_CONFIG");
           uint8_t pin = (uint8_t)argv[1];
           uint16_t len = (uint16_t)argv[2] + ((uint16_t)argv[3]<<7);

           setConfig(pin, len);
           DEBUG_PRINTLN("returning true!");
           return true;
        }

        if (pixelCommand == PIXEL_SET_PIXEL)
        {
            DEBUG_PRINTLN("PIXEL_SET_PIXEL");
            uint16_t i = (uint16_t)argv[1] + ((uint16_t)argv[2]<<7);
              uint32_t color = (uint32_t)argv[3] + ((uint32_t)argv[4]<<7) +
                ((uint32_t)argv[5]<<14) + ((uint32_t)argv[6] << 21);

            DEBUG_PRINT("pin, color: ");
            DEBUG_PRINT(i);
            DEBUG_PRINT(" - ");
            DEBUG_PRINTLN(color);
            setPixel(i, color);

            return true;
        }

        if (pixelCommand == PIXEL_SET_COLOR)
        {
            DEBUG_PRINTLN("PIXEL_SET_COLOR");
            uint32_t color = (uint32_t)argv[3] + ((uint32_t)argv[4]<<7) +
              ((uint32_t)argv[5]<<14) + ((uint32_t)argv[6] << 21);

            DEBUG_PRINT("color: ");
            DEBUG_PRINTLN(color);
            setColor(color);

            return true;
        }

        if (pixelCommand == PIXEL_RANDOM_MODE)
        {
          DEBUG_PRINTLN("PIXEL_RANDOM_MODE");
          ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
          ws2812fx.setColor((uint32_t)0x00FF00);
        }
        if (pixelCommand == PIXEL_SET_MODE)
        {
          DEBUG_PRINTLN("PIXEL_SET_MODE");
          uint16_t mode = (uint8_t)argv[1];
          ws2812fx.setMode(mode);
        }
    }
    return false;
}

// a list of modes can be found in the WS2812FX repo:
// https://github.com/kitesurfer1404/WS2812FX/blob/837b7dd843f0e0c38347c13e4b4594d6dc603162/src/WS2812FX.h#L120-L180
void FirmataNeopixels::setMode(uint8_t mode)
{
  DEBUG_PRINTLN("setMode");
  ws2812fx.setMode(mode);
  ws2812fx.service();
}

void FirmataNeopixels::setPixel(uint16_t i, uint32_t color)
{
  DEBUG_PRINT("setPixel: ");
  DEBUG_PRINTLN(i);

  ws2812fx.setMode(FX_MODE_CUSTOM);
  ws2812fx.setPixelColor(i, color);
  ws2812fx.service();
}

void FirmataNeopixels::setColor(uint32_t color)
{
  DEBUG_PRINTLN("setColor");

  ws2812fx.setMode(FX_MODE_CUSTOM);
  ws2812fx.setColor(color);
  ws2812fx.service();
}

// TODO: make this actually configure the ws2812fx variable!
void FirmataNeopixels::setConfig(uint8_t pin, uint16_t len) {
  DEBUG_PRINT("setConfig: ");
  DEBUG_PRINT(pin);
  DEBUG_PRINT(" - ");
  DEBUG_PRINTLN(len);

  pixelPin = pin;
  pixelCount = len;

  ws2812fx.init();
  ws2812fx.setSpeed(DEFAULT_SPEED);
  ws2812fx.setBrightness(5);
  ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  ws2812fx.start();
  ws2812fx.show();
  ws2812fx.service();
}
