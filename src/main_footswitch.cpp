#include "footswitch.h"
#include "debug.h"

// -------------------------------------------------------------
// HARDWARE CONFIG
// -------------------------------------------------------------
#define PIN_BUTTON_CHANNEL      2
#define PIN_LED_CHANNEL         3
#define PIN_BUTTON_BOOST        4
#define PIN_LED_BOOST           5
#define PIN_BUTTON_FXLOOP       6
#define PIN_LED_FXLOOP          7


void on_footswitch_event(uint8_t id, bool state);


// -------------------------------------------------------------
// GLOBALS
// -------------------------------------------------------------
Footswitch      _channel_button(PIN_BUTTON_CHANNEL, PIN_LED_CHANNEL, on_footswitch_event);
Footswitch      _boost_button(PIN_BUTTON_BOOST, PIN_LED_BOOST, on_footswitch_event);
Footswitch      _fxloop_button(PIN_BUTTON_FXLOOP, PIN_LED_FXLOOP, on_footswitch_event);



// -------------------------------------------------------------
// CODE
// -------------------------------------------------------------
void on_footswitch_event(uint8_t id, bool state) {
    if (id == PIN_BUTTON_CHANNEL) {
        dprint(F("CHANNEL "));
        dprintln(state ? F("ON") : F("OFF"));
    }
    else if (id == PIN_BUTTON_BOOST) {
        dprint(F("BOOST "));
        dprintln(state ? F("ON") : F("OFF"));
    }
    else if (id == PIN_BUTTON_FXLOOP) {
        dprint(F("FXLOOP "));
        dprintln(state ? F("ON") : F("OFF"));
    }
}

void setup() {
    dprintinit(9600);
    dprintln("start");
}

void loop() {
    _channel_button.scan();
    _boost_button.scan();
    _fxloop_button.scan();
}
