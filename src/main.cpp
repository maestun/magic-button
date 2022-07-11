#include <Arduino.h>
#include "optoloop.h"
#include "debug.h"

#define     PIN_FOOTSWITCH              (10)
#define     PIN_RELAY                   (11)
#define     PIN_LED_TOGGLE              (13)
#define     PIN_LED_TEMP                (kPinNull)
#define     PIN_TEMP_THRESHOLD          (A0)

#define     LONGPRESS_THRESHOLD_MS_DEF  (1000)
#define     LONGPRESS_THRESHOLD_MS_MIN  (300)
#define     LONGPRESS_THRESHOLD_MS_MAX  (2000)

FootswitchBypass *    gBypass;

// ============================================================================
void setup() {
    dprintinit(9600);
    dprintln(F("BEGIN"));

    uint16_t longpress_threshold_ms = LONGPRESS_THRESHOLD_MS_DEF;
    if(PIN_TEMP_THRESHOLD != kPinNull) {
        longpress_threshold_ms = (uint16_t)map(analogRead(PIN_TEMP_THRESHOLD), 0, 1024, LONGPRESS_THRESHOLD_MS_MIN, LONGPRESS_THRESHOLD_MS_MAX);
        dprint(F("TEMP THRES: "));
        dprintln(longpress_threshold_ms);
    }

    gBypass = new FootswitchBypass(PIN_FOOTSWITCH, PIN_RELAY, PIN_LED_TEMP, PIN_LED_TOGGLE, longpress_threshold_ms, INPUT_PULLUP);
}


void loop() {
    gBypass->tick();
    delay(1);
}
