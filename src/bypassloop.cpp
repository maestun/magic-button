#include "bypassloop.h"
#include "debug.h"

FootswitchBypass::FootswitchBypass(uint8_t aButtonPin, 
                                   uint8_t aRelayPin, 
                                   uint8_t aTempLEDPin, 
                                   uint8_t aLatchLEDPin, 
                                   uint16_t aLongpressDelayMS, 
                                   uint8_t aPinMode) {
    _button = new Button(aButtonPin, aLongpressDelayMS, this, aPinMode);
    _relay_pin = aRelayPin;
    _temp_led_pin = aTempLEDPin;
    _latch_led_pin = aLatchLEDPin;
    _blink_led_on_temp = (aTempLEDPin == kPinNull) || (aTempLEDPin == aLatchLEDPin);
    
    pinMode(_relay_pin, OUTPUT);
    digitalWrite(_relay_pin, LOW);

    pinMode(_latch_led_pin, OUTPUT);
    digitalWrite(_latch_led_pin, LOW);

    if(_blink_led_on_temp == false) {
        pinMode(_temp_led_pin, OUTPUT);
        digitalWrite(_temp_led_pin, LOW);
    }
}


void FootswitchBypass::updateDisplay(bool aON, bool aTemp) {
    // manage display
    if(_blink_led_on_temp == false) {
        digitalWrite(_temp_led_pin, aON && aTemp);
    }
    digitalWrite(_latch_led_pin, aON && !aTemp);
}

void FootswitchBypass::updateBypass(bool aON) {
    // manage effect
    digitalWrite(_relay_pin, aON);
}


void FootswitchBypass::onButtonEvent(uint8_t aPin, EButtonScanResult aResult) {
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
        _fx_temp = true;
        updateBypass(true);
        updateDisplay(true, true);
    }
    else if(aResult == EButtonUnlongpress) {
        // button released from longpress, turn fx off
        _fx_on = false;
        _fx_temp = false;
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


void FootswitchBypass::tick() {
    _button->tick();
    if(_blink_led_on_temp && _fx_temp) {
        if(millis() - _blinkTS > kBlinkDurationMS) {
            _blinkTS = millis();
            _blink_state = !_blink_state;
            digitalWrite(_latch_led_pin, _blink_state);
        }
    }
}
