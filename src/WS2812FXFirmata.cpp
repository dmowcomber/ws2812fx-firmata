#include <ConfigurableFirmata.h>
#include "WS2812FXFirmata.h"

#define SERIAL_DEBUG
#include "utility/firmataDebug.h"

FirmataNeopixels::FirmataNeopixels()
{
   ws2812fx = NULL;
}

void FirmataNeopixels::service()
{
  if (!ws2812fx) {
    // don't do anything until the instance of ws2812fx is initialized
    return;
  }
  (*ws2812fx).service();
}

boolean FirmataNeopixels::handleSysex(byte command, byte argc, byte *argv)
{
    if (command == NEOPIXEL_CMD)
    {
        DEBUG_PRINT("Got command: ");

        byte pixelCommand;
        pixelCommand = argv[0];

        // guard against a NULL ws2812fx
        if (!ws2812fx && pixelCommand != PIXEL_CONFIG)
        {
          DEBUG_PRINTLN("you must run setConfig before doing anything else");
          // don't do anything until the instance of ws2812fx is initialized
          return false;
        }

        if (pixelCommand == PIXEL_STOP)
        {
            DEBUG_PRINTLN("PIXEL_STOP");
            stop();
            return true;
        }

        if (pixelCommand == PIXEL_START)
        {
            DEBUG_PRINTLN("PIXEL_START");
            start();
            return true;
        }

        if (pixelCommand == PIXEL_CONFIG)
        {
           DEBUG_PRINTLN("PIXEL_CONFIG");
           uint8_t pin = (uint8_t)argv[1];
           uint16_t len = (uint16_t)argv[2] + ((uint16_t)argv[3]<<7);

           setConfig(pin, len);
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

        if (pixelCommand == PIXEL_MODE_CYCLE)
        {
          DEBUG_PRINTLN("PIXEL_MODE_CYCLE");
          (*ws2812fx).setMode(((*ws2812fx).getMode() + 1) % (*ws2812fx).getModeCount());
          (*ws2812fx).setColor((uint32_t)0x00FF00);
        }
        if (pixelCommand == PIXEL_SET_MODE)
        {
          DEBUG_PRINTLN("PIXEL_SET_MODE");

          DEBUG_PRINTLN("PIXEL_SET_PIXEL");
          uint16_t mode = (uint8_t)argv[1];
          // uint32_t color = (uint32_t)argv[3] + ((uint32_t)argv[4]<<7) +
          //   ((uint32_t)argv[5]<<14) + ((uint32_t)argv[6] << 21);

          (*ws2812fx).setMode(mode);
          // (*ws2812fx).setColor((uint32_t)0x00FF00);
        }
        if (pixelCommand == PIXEL_SET_BRIGHTNESS)
        {
          DEBUG_PRINT("PIXEL_SET_BRIGHTNESS: ");
          uint16_t brightness = (uint8_t)argv[1];
          DEBUG_PRINTLN(brightness);

          (*ws2812fx).setBrightness(brightness);
        }
    }

    return false;
}

// FirmataNeopixels interface functions
void FirmataNeopixels::stop()
{
  DEBUG_PRINTLN("stop");
  (*ws2812fx).stop();
  (*ws2812fx).service();
}

void FirmataNeopixels::start()
{
  DEBUG_PRINTLN("start");
  (*ws2812fx).start();
  (*ws2812fx).service();
}

// a list of modes can be found in the WS2812FX repo:
// https://github.com/kitesurfer1404/WS2812FX/blob/837b7dd843f0e0c38347c13e4b4594d6dc603162/src/WS2812FX.h#L120-L180
void FirmataNeopixels::setMode(uint8_t mode)
{
  DEBUG_PRINTLN("setMode");
  (*ws2812fx).setMode(mode);
  (*ws2812fx).service();
}
void FirmataNeopixels::setBrightness(uint8_t mode)
{
  DEBUG_PRINTLN("setBrightness");
  (*ws2812fx).setBrightness(mode);
  (*ws2812fx).service();
}

void FirmataNeopixels::setPixel(uint16_t i, uint32_t color)
{
  DEBUG_PRINT("setPixel: ");
  DEBUG_PRINTLN(i);

  (*ws2812fx).setMode(FX_MODE_CUSTOM);
  (*ws2812fx).setPixelColor(i, color);

  (*ws2812fx).service();
}

void FirmataNeopixels::setColor(uint32_t color)
{
  DEBUG_PRINTLN("setColor");

  (*ws2812fx).setMode(FX_MODE_CUSTOM);
  (*ws2812fx).setColor(color);
  (*ws2812fx).service();
}

void FirmataNeopixels::setConfig(uint8_t pixelPin, uint16_t pixelCount) {
  DEBUG_PRINT("setConfig: ");
  DEBUG_PRINT(pixelPin);
  DEBUG_PRINT(" - ");
  DEBUG_PRINTLN(pixelCount);

  DEBUG_PRINTLN("create WS2812FX instance");
  ws2812fx = new WS2812FX(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);

  DEBUG_PRINTLN("ws2812fx2 setup");
  (*ws2812fx).init();
  (*ws2812fx).setSpeed(DEFAULT_SPEED);
  (*ws2812fx).setBrightness(0);
  (*ws2812fx).setMode(FX_MODE_RAINBOW_CYCLE);
  (*ws2812fx).start();
  (*ws2812fx).show();
  (*ws2812fx).service();
}

// FirmataFeature interface functions. Required for any Firmata plugin.
boolean FirmataNeopixels::handlePinMode(byte pin, int mode)
{
    return false;
}
void FirmataNeopixels::handleCapability(byte pin) {}
void FirmataNeopixels::reset() {}
