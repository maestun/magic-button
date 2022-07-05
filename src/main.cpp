#include <Arduino.h>
#include "optoloop.h"
#include "debug.h"

#define     PIN_NULL                (-1)

#define     PIN_BYPASS_IN           (6)
#define     PIN_OCTOCOUPLER         (2)
#define     PIN_LED_TOGGLE          (11)
#define     PIN_LED_TEMP            (12)
#define     PIN_RELAY               (5)
#define     PIN_TEMP_THRESHOLD      (A0)
#define     LONGPRESS_THRESHOLD_MS  (1000)
#define     AUDIO_CUT_MS            (20)


#define     TEMP_THRESHOLD_MS_MIN   (200)
#define     TEMP_THRESHOLD_MS_MAX   (2000)


Button *        gButton;
Optoloop *      gLoopA;
BypassLoop *    gBypass;


// ============================================================================
void setup() {
    dprintinit(9600);
    dprintln(F("BEGIN"));

    uint16_t longpress_threshold_ms = LONGPRESS_THRESHOLD_MS;
    if(PIN_TEMP_THRESHOLD != PIN_NULL) {
        longpress_threshold_ms = (uint16_t)map(analogRead(PIN_TEMP_THRESHOLD), 0, 1024, TEMP_THRESHOLD_MS_MIN, TEMP_THRESHOLD_MS_MAX);
        dprint(F("TEMP THRES: "));
        dprintln(longpress_threshold_ms);
    }

    gButton = new Button(PIN_BYPASS_IN, longpress_threshold_ms, NULL);
    gBypass = new BypassLoop(PIN_BYPASS_IN, PIN_RELAY, PIN_LED_TEMP, PIN_LED_TOGGLE, longpress_threshold_ms);
    gLoopA = new Optoloop(PIN_BYPASS_IN, PIN_OCTOCOUPLER, PIN_RELAY, PIN_LED_TEMP, PIN_LED_TOGGLE, longpress_threshold_ms, AUDIO_CUT_MS);
}


void loop() {
    gButton->scan();
    gBypass->scan();
    gLoopA->scan();
    delay(1);
}
