/*
 * Pullup button wiring : Vcc => button => pin
 *                                      => 10k => GND
 */

#include "button.h"


static const int8_t BUTTON_NULL = -1;
static int8_t gPrevButton       = BUTTON_NULL;


Button::Button(uint8_t aPin, uint16_t aLongpressDelay, button_cb_t aCallback) {
    pinMode(aPin, INPUT);
    pin = aPin;
    longpress = false;
    longpressDelay = aLongpressDelay;
    longpressTS = 0;
    callback = aCallback;
    gPrevButton = BUTTON_NULL;
}


void Button::onButtonReleased() {

    if(pin == gPrevButton) {
        // unclick
        if(longpress == false) {
            callback(pin, EButtonUp);
            callback(pin, EButtonClick);
        }
        else {
            // unlongpress
            callback(pin, EButtonUnlongpress);
            longpress = false;
        }
        gPrevButton = BUTTON_NULL;
    }
}


// TODO: implement debounce filter !!
void Button::onButtonPressed() {

    // previous code w/ longpress detection
    if(pin == gPrevButton) {
        // same pin still pressed
        if(longpress == false && (millis() - longpressTS) >= longpressDelay) {
            longpress = true;
            callback(pin, EButtonLongpress);
        }
    }
    else {
        // new button pressed
        callback(pin, EButtonDown);
        longpressTS = millis();
        gPrevButton = pin;
    }
}


void Button::scan() {
    if(digitalRead(pin) == true) {
        // pressed
        onButtonPressed();
    }
    else {
        // released
        onButtonReleased();
    }
}