#ifndef FirmataNeopixels_h
#define FirmataNeopixels_h

#include <ConfigurableFirmata.h>
#include <FirmataFeature.h>
#include <WS2812FX.h>

#define NEOPIXEL_CMD            (0x51)
#define PIXEL_SET_BRIGHTNESS    (0x00)
#define PIXEL_SET_COLOR         (0x01)
#define PIXEL_SET_PIXEL         (0x02) // set the color value of pixel n using 32bit packed color value
#define PIXEL_SET_STRIP         (0x03) // set color of whole strip
#define PIXEL_SHIFT             (0x04) // shift all the pixels n places along the strip
#define PIXEL_SET_MODE          (0x05)
#define PIXEL_MODE_CYCLE        (0x06)
#define PIXEL_START             (0x07) // turn on the strip
#define PIXEL_STOP              (0x08) // turn off the strip
#define PIXEL_CONFIG            (0x09) // configure the strip

class FirmataNeopixels :public FirmataFeature
{
private:
    WS2812FX* ws2812fx;
public:
    FirmataNeopixels();
    void service();

    // FirmataFeature interface functions. Required for any Firmata plugin.
    boolean handlePinMode(byte pin, int mode);
    void handleCapability(byte pin);
    boolean handleSysex(byte command, byte argc, byte *argv);
    void reset();

    // FirmataNeopixels interface functions
    void setConfig(uint8_t pin, uint16_t len);
    void setPixel(uint16_t n, uint32_t color);
    void setColor(uint32_t color);
    void setBrightness(uint8_t);
    void stop();
    void start();
    void setMode(uint8_t mode);
};

#endif
