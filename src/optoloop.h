#include "bypassloop.h"

class Optoloop : public BypassLoop {
private:
    uint8_t     _octocoupler_pin;
    uint8_t     _audio_cut_ms;

protected:
    void        updateBypass(bool aON);

public:
    // void scan();
    Optoloop(uint8_t aButtonPin, uint8_t aOctocouplerPin, uint8_t aRelayPin, uint8_t aTempLEDPin, uint8_t aLatchLEDPin, uint16_t aLongpressDelayMS, uint8_t aAudioCutMS);
};