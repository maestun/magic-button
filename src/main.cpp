#include <Arduino.h>
#include "button.h"
#include "debug.h"

#define     PIN_BYPASS_IN           (6)
#define     PIN_OCTOCOUPLER         (2)
#define     PIN_LED_TOGGLE          (13)
#define     PIN_LED_TEMP            (12)
#define     PIN_RELAY               (5)
#define     LONGPRESS_THRESHOLD_MS  (1000)
#define     AUDIO_CUT_MS            (20)

SButtonData gButton;


// ============================================================================
void FX_Toggle(bool aON, bool aTemp) {

    // manage effect
    digitalWrite(PIN_OCTOCOUPLER, HIGH);
    digitalWrite(PIN_RELAY, aON);
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
    static bool fx_disable = false;
    if(aResult == EButtonDown) {
        // button down: toggle fx on if it wasn't
        dprintln(F("DOWN"));
        if(!fx_on) {
            fx_on = true;
            fx_disable = false;
            FX_Toggle(true, false);
        }
        else {
            fx_disable = true;   
        }
    }
    else if(aResult == EButtonLongpress) {
        // button longpressed: switch to temporary mode
        dprintln(F("LONG"));
        FX_Toggle(true, true);
    }
    else if(aResult == EButtonUnlongpress) {
        // button released from longpress, turn fx off
        fx_on = false;
        dprintln(F("UNLONG"));
        FX_Toggle(false, false);
    }
    else if(aResult == EButtonClick) {
        // button clicked: ignore
        dprintln(F("CLICK"));
        if(fx_on && fx_disable) {
            fx_on = false;
            FX_Toggle(false, false);
        }
    }
    else if(aResult == EButtonUp) {
        // button released from shortpress: turn fx off if it was on
        dprintln(F("UP"));
        
    }
}


// ============================================================================
void setup() {
    dprintinit(9600);
    dprintln(F("BEGIN"));

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
    delay(1);
}
