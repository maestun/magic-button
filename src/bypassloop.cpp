#include "bypassloop.h"
#include "debug.h"

BypassLoop::BypassLoop(uint8_t aButtonPin, uint8_t aRelayPin, uint8_t aTempLEDPin, uint8_t aLatchLEDPin, uint16_t aLongpressDelayMS) {
    _button = new Button(aButtonPin, aLongpressDelayMS, this);
    _relay_pin = aRelayPin;
    _temp_led_pin = aTempLEDPin;
    _latch_led_pin = aLatchLEDPin;
    
    pinMode(_relay_pin, OUTPUT);
    pinMode(_temp_led_pin, OUTPUT);
    pinMode(_latch_led_pin, OUTPUT);
    
    digitalWrite(_relay_pin, LOW);
    digitalWrite(_temp_led_pin, LOW);
    digitalWrite(_latch_led_pin, LOW);
}


void BypassLoop::updateDisplay(bool aON, bool aTemp) {
    // manage display
    digitalWrite(_temp_led_pin, aON && aTemp);
    digitalWrite(_latch_led_pin, aON && !aTemp);

    // if(aON) {
    //     digitalWrite(_temp_led_pin, aTemp);
    //     digitalWrite(_latch_led_pin, !aTemp);
    // }
    // else {
    //     // turn LEDs off
    //     digitalWrite(_temp_led_pin, false);
    //     digitalWrite(_latch_led_pin, false);
    // }
}

void BypassLoop::updateBypass(bool aON) {
    // manage effect
    digitalWrite(_relay_pin, aON);
}


void BypassLoop::onButtonEvent(uint8_t aPin, EButtonScanResult aResult) {
    if(aResult == EButtonDown) {
        // button down: toggle fx on if it wasn't
        dprintln(F("DOWN"));
        if(!_fx_on) {
            _fx_on = true;
            _fx_disable = false;
            updateBypass(true);
            updateDisplay(true, false);
        }
        else {
            _fx_disable = true;   
        }
    }
    else if(aResult == EButtonLongpress) {
        // button longpressed: switch to temporary mode
        dprintln(F("LONG"));
        updateBypass(true);
        updateDisplay(true, true);
    }
    else if(aResult == EButtonUnlongpress) {
        // button released from longpress, turn fx off
        _fx_on = false;
        dprintln(F("UNLONG"));
        updateBypass(false);
        updateDisplay(false, false);

    }
    else if(aResult == EButtonClick) {
        // button clicked: turn fx off if it was on
        dprintln(F("CLICK"));
        if(_fx_on && _fx_disable) {
            _fx_on = false;
            updateBypass(false);
            updateDisplay(false, false);

        }
    }
    else if(aResult == EButtonUp) {
        // button released from shortpress: ignore
        dprintln(F("UP"));  
    }
}


void BypassLoop::scan() {
    _button->scan();
}
