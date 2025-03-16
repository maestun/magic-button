
#include "button.h"

static const uint8_t  DEBOUNCE_MS = 20;
static const int8_t   BUTTON_NULL = -1;

Button::Button(uint8_t aPin, uint16_t aLongpressDelayMS, ButtonListener * aListener, uint8_t aMode) {
    pinMode(aPin, aMode);
    _inputMode = aMode;
    _id = aPin;
    _longpressed = false;
    _longpressMS = aLongpressDelayMS;
    _longpressTS = 0;
    _debounceTS = 0;
    _listener = aListener;
    _prevState = LOW;
    _prevButton = BUTTON_NULL;
}

Button::Button(uint8_t aPin, uint16_t aLongpressDelayMS, button_cb_t aCallback, uint8_t aMode) : 
    Button(aPin, aLongpressDelayMS, (ButtonListener *)NULL, aMode) {
    _fptr = aCallback;
}


void Button::onButtonReleased() {
    if(_id == _prevButton) {
        // unclick
        if(_longpressed == false) {
            if (_listener != NULL) {
                // Serial.println("clic");
                _listener->onButtonEvent(_id, EButtonUp);
                _listener->onButtonEvent(_id, EButtonClick);
            }
            if (_fptr != NULL) {
                _fptr(_id, EButtonUp);
                _fptr(_id, EButtonClick);
            }
        }
        else {
            // unlongpress
            // Serial.println("unlong");
            if (_listener != NULL) {
                _listener->onButtonEvent(_id, EButtonUnlongpress);
            }
            if (_fptr != NULL) {
                _fptr(_id, EButtonUnlongpress);
            }
            _longpressed = false;
        }
        _prevButton = BUTTON_NULL;
    }
}


// TODO: implement debounce filter !!
void Button::onButtonPressed() {

    // previous code w/ longpress detection
    if(_id == _prevButton) {
        // same pin still pressed
        if(_longpressed == false && (millis() - _longpressTS) >= _longpressMS) {
            _longpressed = true;
            // Serial.println("longpress");
            if (_listener != NULL) {
                _listener->onButtonEvent(_id, EButtonLongpress);
            }
            if (_fptr != NULL) {
                _fptr(_id, EButtonLongpress);
            }
        }
        if(_longpressed == true) {
            // Serial.println("hold");
            if (_listener != NULL) {
               _listener->onButtonEvent(_id, EButtonHold);
            }
            if (_fptr != NULL) {
                _fptr(_id, EButtonHold);
            }
        }
    }
    else {
        // new button pressed
        if (_listener != NULL) {
            _listener->onButtonEvent(_id, EButtonDown);
        }
        if (_fptr != NULL) {
            _fptr(_id, EButtonDown);
        }
        _longpressTS = millis();
        _prevButton = _id;
    }
}


void Button::scanLogic(int8_t aState) {
    if (aState != _prevState) {
        // reset the debouncing timer
        _debounceTS = millis();
    }

    if ((millis() - _debounceTS) > DEBOUNCE_MS) {
        // check state only if debounced
        if(aState == true) {
            // pressed
            // Serial.print(_id);
            // Serial.println(" press");
            onButtonPressed();
        }
        else {
            // released
            // Serial.print(_id);
            // Serial.println(" rel");
            onButtonReleased();
        }
    }
    _prevState = aState;
}


void Button::scan() {
    bool state = !!digitalRead(_id);
    // Serial.println(state);
    if (_inputMode == INPUT_PULLUP) {
        state = !state;
    }
    scanLogic(state);
}

// =============================================================================

void AnalogButton::scan() {
    uint16_t val = analogRead(_analogPin);
    // Serial.println(val);
    uint8_t state = ((val - _deltaValue < _analogValue) && (val + _deltaValue > _analogValue));
    scanLogic(state);
}