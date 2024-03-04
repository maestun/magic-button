#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "magicbypass.h"

#if TARGET_ATTINY13 || TARGET_ATTINY85
    #define PIN_SW          PB2
    #define PIN_LED         PB4
    #define PIN_LED_T       PB5
    #define PIN_MUTE        PB1
    #define PIN_BYPASS      PB0
    #define PIN_ENGAGE      PB3
    #define TEMP_MS         500
    #define DEBOUNCE_MS     50
    #define AUDIO_CUT_MS    20
#endif

void setup() {
    mbp_init(PIN_SW, PIN_LED, PIN_LED_T, PIN_BYPASS, TEMP_MS, DEBOUNCE_MS, AUDIO_CUT_MS);
}


void loop() {
    mbp_tick();
}