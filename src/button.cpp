
#include "button.h"
#define BUTTON_NULL (-1)

/*
 Pullup button wiring :
 GND => button => pin
 */
static int8_t gPrevButton = BUTTON_NULL;

// ====================================================================================
#pragma mark - HARDWARE BUTTON SCAN
// ====================================================================================

void HW_SetupButton(SButtonData * aButton, uint8_t aPin, uint16_t aLongpressDelay, button_cb_t aCallback) {
    pinMode(aPin, INPUT_PULLUP);
    aButton->pin = aPin;
    aButton->callback = aCallback;
    aButton->longpress = false;
    aButton->longpressDelay = aLongpressDelay;
    aButton->longpressTS = 0;
    gPrevButton = BUTTON_NULL;
}


static void HW_OnButtonReleased(SButtonData * aButton) {
    if(aButton->pin == gPrevButton) {
        aButton->callback(aButton->pin, EButtonUp);
        // unclick
        if(aButton->longpress == false) {
            aButton->callback(aButton->pin, EButtonClick);
        }
        else {
            // unlongpress
            aButton->longpress = false;
        }
        gPrevButton = BUTTON_NULL;
    }
}

// TODO: implement debounce filter !!
static void HW_OnButtonPressed(SButtonData * aButton) {
    // previous code w/ longpress detection
    if(aButton->pin == gPrevButton) {
        // same pin still pressed
        if(aButton->longpress == false && (millis() - aButton->longpressTS) >= aButton->longpressDelay) {
            aButton->longpress = true;
            aButton->callback(aButton->pin, EButtonLongpress);
        }
    }
    else {
        // new button pressed
        aButton->callback(aButton->pin, EButtonDown);
        aButton->longpressTS = millis();
        gPrevButton = aButton->pin;
    }
}


void HW_ScanButton(SButtonData * aButton) {
    if(digitalRead(aButton->pin) == LOW) {
        // pressed
        HW_OnButtonPressed(aButton);
    }
    else {
        // released
        HW_OnButtonReleased(aButton);
    }
}
