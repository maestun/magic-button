#include <Arduino.h>
#include "button.h"

#define     PIN_BYPASS_IN           (1)
#define     PIN_OCTOCOUPLER         (2)
#define     PIN_LED_TOGGLE          (3)
#define     PIN_LED_TEMP            (4)
#define     PIN_RELAY               (5)
#define     LONGPRESS_THRESHOLD_MS  (1000)
#define     AUDIO_CUT_MS            (20)

SButtonData gButton;
bool        gEffectON;


// ============================================================================
void FX_Toggle(bool aON, bool aTemp) {

    // manage effect
    digitalWrite(PIN_OCTOCOUPLER, HIGH);
    digitalWrite(PIN_RELAY, gEffectON);
    delay(AUDIO_CUT_MS);
    digitalWrite(PIN_OCTOCOUPLER, LOW);
    
    // manage display
    if(aON) {
        digitalWrite(PIN_LED_TEMP, aTemp);
        digitalWrite(PIN_LED_TOGGLE, !aTemp);
    }
    else {
        // turn LEDs off
        digitalWrite(PIN_LED_TEMP, false);
        digitalWrite(PIN_LED_TOGGLE, false);
    }
}


// ============================================================================
void onButtonEvent(uint8_t aPin, EButtonScanResult aResult) {
    static bool fx_on = false;
    if(aResult == EButtonDown && !fx_on) {
        // button down: toggle fx on if it wasn't
        fx_on = true;
        FX_Toggle(true, false);
    }
    else if(aResult == EButtonLongpress) {
        // button longpressed: switch to temporary mode
        FX_Toggle(true, true);
    }
    else if(aResult == EButtonUnlongpress) {
        // button released from longpress, turn fx off
        FX_Toggle(false, false);
    }
    else if(aResult == EButtonClick) {
        // button clicked: ignore
    }
    else if(aResult == EButtonUp && fx_on) {
        // button released from shortpress: turn fx off if it was on
        fx_on = false;
        FX_Toggle(false, false);
    }
}


// ============================================================================
void setup() {
    gLongpress = false;

    pinMode(PIN_OCTOCOUPLER, OUTPUT);
    pinMode(PIN_RELAY, OUTPUT);
    pinMode(PIN_LED_TEMP, OUTPUT);
    pinMode(PIN_LED_TOGGLE, OUTPUT);
    
    digitalWrite(PIN_OCTOCOUPLER, LOW);
    digitalWrite(PIN_RELAY, LOW);

    HW_SetupButton(&gButton, PIN_BYPASS_IN, LONGPRESS_THRESHOLD_MS, &onButtonEvent);
}

void loop() {
    HW_ScanButton(&gButton);
}
