#include "optoloop.h"
#include "debug.h"

Optoloop::Optoloop(uint8_t aButtonPin, 
                   uint8_t aOctocouplerPin, 
                   uint8_t aRelayPin, 
                   uint8_t aTempLEDPin, 
                   uint8_t aLatchLEDPin, 
                   uint16_t aLongpressDelayMS, 
                   uint8_t aAudioCutMS) :
                   BypassLoop(aButtonPin, aRelayPin, aTempLEDPin, aLatchLEDPin, aLongpressDelayMS) {

    _octocoupler_pin = aOctocouplerPin;
    _audio_cut_ms = aAudioCutMS;
    
    pinMode(_octocoupler_pin, OUTPUT);
   
    digitalWrite(_octocoupler_pin, LOW);
}


void Optoloop::updateBypass(bool aON) {
    // manage effect
    digitalWrite(_octocoupler_pin, HIGH);
    digitalWrite(_relay_pin, aON);
    delay(_audio_cut_ms);
    digitalWrite(_octocoupler_pin, LOW);
}
