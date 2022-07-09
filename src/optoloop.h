#include "bypassloop.h"


static constexpr uint8_t kDefaultAudioCutMS = 20;

class OptoFootswitchBypass : public FootswitchBypass {
private:
    uint8_t     _octocoupler_pin;
    uint8_t     _audio_cut_ms;

protected:
    void        updateBypass(bool aON);

public:
    OptoFootswitchBypass(uint8_t aButtonPin, 
                         uint8_t aOctocouplerPin, 
                         uint8_t aRelayPin, 
                         uint8_t aTempLEDPin, 
                         uint8_t aLatchLEDPin, 
                         uint16_t aLongpressDelayMS, 
                         uint8_t aAudioCutMS = kDefaultAudioCutMS,
                         uint8_t aPinMode = INPUT_PULLUP);
};