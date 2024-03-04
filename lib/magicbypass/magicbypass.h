#include "Arduino.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

void mbp_init(uint8_t aPinSwitch, 
              uint8_t aPinLED, 
              uint8_t aPinLEDTemp, 
              uint8_t aPinRelay,
              uint16_t aLongpressMS,
              uint16_t aDebounceMS,
              uint16_t aAudioCutMS);
void mbp_tick();