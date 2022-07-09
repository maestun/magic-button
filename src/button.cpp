/*
 * Pullup button wiring : Vcc => button => pin
 *                                      => 10k => GND
 */


#include "button.h"


static constexpr uint8_t    kDebounceMS = 50;
static int8_t               gPrevButton = kPinNull;
static int8_t               gState = kPinNull;


Button::Button(uint8_t aPin, uint16_t aLongpressDelayMS, IButtonListener * aListener, uint8_t aPinMode = INPUT) {

    pinMode(aPin, aPinMode);
    _pin = aPin;
    _longpressed = false;
    _longpressMS= aLongpressDelayMS;
    _longpressTS = 0;
    _debounceTS = 0;
    _listener = aListener;
    _prevState = LOW;
    gPrevButton = kPinNull;
}


void Button::onButtonReleased() {

    if(_pin == gPrevButton) {
        // unclick
        if(_longpressed == false) {
            _listener->onButtonEvent(_pin, EButtonUp);
            _listener->onButtonEvent(_pin, EButtonClick);
        }
        else {
            // unlongpress
            _listener->onButtonEvent(_pin, EButtonUnlongpress);
            _longpressed = false;
        }
        gPrevButton = kPinNull;
    }
}


void Button::onButtonPressed() {

    // previous code w/ longpress detection
    if(_pin == gPrevButton) {
        // same pin still pressed
        if(_longpressed == false && (millis() - _longpressTS) >= _longpressMS) {
            _longpressed = true;
            _listener->onButtonEvent(_pin, EButtonLongpress);
        }
        if(_longpressed == true) {
            _listener->onButtonEvent(_pin, EButtonHold);
        }
    }
    else {
        // new button pressed
        _listener->onButtonEvent(_pin, EButtonDown);
        _longpressTS = millis();
        gPrevButton = _pin;
    }
}


void Button::tick() {
    
    gState = digitalRead(_pin);
    if (gState != _prevState) {
        // reset the debouncing timer
        _debounceTS = millis();
    }

    if ((millis() - _debounceTS) > kDebounceMS) {
        // check state only if debounced
        if(gState == true) {
            // pressed
            onButtonPressed();
        }
        else {
            // released
            onButtonReleased();
        }
    }
    _prevState = gState;
}
