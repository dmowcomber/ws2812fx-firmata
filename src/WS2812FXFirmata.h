#ifndef FirmataNeopixels_h
#define FirmataNeopixels_h

#include <ConfigurableFirmata.h>
#include <FirmataFeature.h>
#include <WS2812FX.h>

#define NEOPIXEL_CMD            (0x51)
#define PIXEL_OFF               (0x00) // set strip to be off
#define PIXEL_CONFIG            (0x01) // configure the strip
#define PIXEL_SHOW              (0x02) // latch the pixels and show them
#define PIXEL_SET_PIXEL         (0x03) // set the color value of pixel n using 32bit packed color value
#define PIXEL_SET_STRIP         (0x04) // set color of whole strip
#define PIXEL_SHIFT             (0x05) // shift all the pixels n places along the strip
#define PIXEL_RANDOM_MODE       (0x06)
#define PIXEL_SET_MODE          (0x07)
// TODO: reorder these! so off and on are next to each other
#define PIXEL_ON                (0x08) // set strip to be on
#define PIXEL_SET_COLOR         (0x09)

#define DEFAULT_PIN            5
#define DEFAULT_NUMPIXELS      7

class FirmataNeopixels :public FirmataFeature
{
public:
    uint16_t pixelCount;
    uint8_t pixelPin;
    uint8_t *px;
    uint8_t colorDepth;


    FirmataNeopixels();
    void service();

    // FirmataFeature interface functions. Required for any Firmata plugin.
    boolean handlePinMode(byte pin, int mode);
    void handleCapability(byte pin);
    boolean handleSysex(byte command, byte argc, byte *argv);
    void reset();

    // FirmataNeopixels interface functions
    void setConfig(uint8_t pin, uint16_t len);
    void setPin(uint8_t p);
    void setPixel(uint16_t n, uint32_t color);
    void setColor(uint32_t color);
    void setMode(uint8_t mode);
};

#endif
