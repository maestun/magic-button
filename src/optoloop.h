#include "button.h"

class Optoloop {
private:
    Button *    button;
    uint8_t     octocoupler_pin;
    uint8_t     relay_pin;
    uint8_t     temp_led_pin;
    uint8_t     latch_led_pin;
    uint8_t     audio_cut_ms;
    bool        fx_on = false;
    bool        fx_disable = false;
    void        onButtonEvent(uint8_t aPin, EButtonScanResult aResult);
    void        toggle(bool aON, bool aTemp);
public:
    void scan();
    Optoloop(uint8_t aButtonPin, uint8_t aOctocouplerPin, uint8_t aRelayPin, uint8_t aTempLEDPin, uint8_t aLatchLEDPin, uint16_t aLongpressDelayMS, uint8_t aAudioCutMS);
};