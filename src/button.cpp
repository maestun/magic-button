/*
 * Pullup button wiring : Vcc => button => pin
 *                                      => 10k => GND
 */

#include "button.h"


static const int8_t BUTTON_NULL = -1;
static int8_t gPrevButton       = BUTTON_NULL;


Button::Button(uint8_t aPin, uint16_t aLongpressDelay, ButtonListener * aListener) {
    pinMode(aPin, INPUT);
    pin = aPin;
    longpress = false;
    longpressDelay = aLongpressDelay;
    longpressTS = 0;
    listener = aListener;
    gPrevButton = BUTTON_NULL;
}


void Button::onButtonReleased() {

    if(pin == gPrevButton) {
        // unclick
        if(longpress == false) {
            listener->onButtonEvent(pin, EButtonUp);
            listener->onButtonEvent(pin, EButtonClick);
        }
        else {
            // unlongpress
            listener->onButtonEvent(pin, EButtonUnlongpress);
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
            listener->onButtonEvent(pin, EButtonLongpress);
        }
    }
    else {
        // new button pressed
        listener->onButtonEvent(pin, EButtonDown);
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