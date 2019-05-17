#include <Arduino.h>
#include "optoloop.h"
#include "debug.h"

#define     PIN_BYPASS_IN           (6)
#define     PIN_OCTOCOUPLER         (2)
#define     PIN_LED_TOGGLE          (11)
#define     PIN_LED_TEMP            (12)
#define     PIN_RELAY               (5)
#define     LONGPRESS_THRESHOLD_MS  (1000)
#define     AUDIO_CUT_MS            (20)


Button *    gButton;
Optoloop *  gLoopA;
Optoloop *  gLoopB;



// ============================================================================
void setup() {
    dprintinit(9600);
    dprintln(F("BEGIN"));


    gButton = new Button(PIN_BYPASS_IN, LONGPRESS_THRESHOLD_MS, NULL);
    gLoopA = new Optoloop(PIN_BYPASS_IN, PIN_OCTOCOUPLER, PIN_RELAY, PIN_LED_TEMP, PIN_LED_TOGGLE, LONGPRESS_THRESHOLD_MS, AUDIO_CUT_MS);
    gLoopB = new Optoloop(PIN_BYPASS_IN, PIN_OCTOCOUPLER, PIN_RELAY, PIN_LED_TEMP, PIN_LED_TOGGLE, LONGPRESS_THRESHOLD_MS, AUDIO_CUT_MS);
}


void loop() {
    gButton->scan();
    gLoopA->scan();
    gLoopB->scan();
    delay(1);
}
