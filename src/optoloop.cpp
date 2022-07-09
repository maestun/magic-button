#include "optoloop.h"
#include "debug.h"

OptoFootswitchBypass::OptoFootswitchBypass(uint8_t aButtonPin, 
                   uint8_t aOctocouplerPin, 
                   uint8_t aRelayPin, 
                   uint8_t aTempLEDPin, 
                   uint8_t aLatchLEDPin, 
                   uint16_t aLongpressDelayMS, 
                   uint8_t aAudioCutMS,
                   uint8_t aPinMode) :
                   FootswitchBypass(aButtonPin, 
                                    aRelayPin, 
                                    aTempLEDPin, 
                                    aLatchLEDPin,
                                    aLongpressDelayMS,
                                    aPinMode) {

    _octocoupler_pin = aOctocouplerPin;
    _audio_cut_ms = aAudioCutMS;
    
    pinMode(_octocoupler_pin, OUTPUT);
    digitalWrite(_octocoupler_pin, LOW);
}


void OptoFootswitchBypass::updateBypass(bool aON) {
    // manage effect
    digitalWrite(_octocoupler_pin, HIGH);
    digitalWrite(_relay_pin, aON);
    delay(_audio_cut_ms);
    digitalWrite(_octocoupler_pin, LOW);
}
