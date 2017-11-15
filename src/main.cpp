#include <Arduino.h>
#include "button.h"

#define     PIN_BYPASS_IN           (1)
#define     PIN_OCTOCOUPLER         (2)
#define     PIN_LED_TOGGLE          (3)
#define     PIN_LED_TEMP            (4)
#define     PIN_RELAY               (5)
#define     LONGPRESS_THRESHOLD_MS  (500)
#define     AUDIO_CUT_MS            (20)

SButtonData gButton;
bool        gEffectON;
bool        gLongpress;


// ============================================================================
void FX_Toggle(bool aON) {
    gEffectON = aON;
    digitalWrite(PIN_OCTOCOUPLER, HIGH);
    digitalWrite(PIN_RELAY, gEffectON);
    delay(AUDIO_CUT_MS);
    digitalWrite(PIN_OCTOCOUPLER, LOW);
}


// ============================================================================
void onButtonEvent(uint8_t aPin, EButtonScanResult aResult) {
    if(aResult == EButtonDown) {
        // button down: toggle
        gEffectON = !gEffectON;
        gLongpress = false;
        digitalWrite(PIN_LED_TOGGLE, gEffectON);
        FX_Toggle(gEffectON);
    }
    else if(aResult == EButtonClick) {
        
    }
    else if(aResult == EButtonLongpress) {
        gLongpress = true;
        digitalWrite(PIN_LED_TEMP, HIGH);
        digitalWrite(PIN_LED_TOGGLE, LOW);
    }
    else if(aResult == EButtonUp) {
        if(gLongpress) {
            // button released from longpress, turn fx off
            FX_Toggle(false);
            digitalWrite(PIN_LED_TEMP, LOW);
        }
        else {
            // released normally, do nothing
        }
    }
}


// ============================================================================
void setup() {
    gEffectON = false;
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