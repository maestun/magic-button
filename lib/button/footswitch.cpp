#include "footswitch.h"

void Footswitch::onButtonEvent(uint8_t aID, EButtonScanResult aResult) {
    if (_mechanical)
        simulate_mechanical(aID, aResult);
    else
        simulate_immediate(aID, aResult);
}


void Footswitch::simulate_immediate(uint8_t aID, EButtonScanResult aResult) {
    switch (aResult) {
        case EButtonDown: {
            // button down: toggle fx on if it wasn't
            // Serial.print(aID);
            // Serial.println(F(" - DOWN"));
            _on = !_on;
            _fptr(aID, _on);
            digitalWrite(_ledPin, _on);
        } break;

        case EButtonUp: {
            // button released from shortpress: ignore
            // Serial.print(aID);
            // Serial.println(F(" - UP"));
        } break;

        case EButtonClick: {
            // button clicked: ignore
        } break;

        case EButtonLongpress: {
            // switch temp mode if fx on
            // Serial.print(aID);
            // Serial.println(" - LP");
            _led = _on;
            _blinkTS = millis();
        } break;
        case EButtonHold: {
            if (millis() - _blinkTS > _blinkIntervalMS) {
                _led = !_led;
                digitalWrite(_ledPin, _led);
                _blinkTS = millis();
            }    
        } break;
        case EButtonUnlongpress: {
            // toggle state
            // Serial.print(aID);
            // Serial.println(" - ULP");
            _on = !_on;
            digitalWrite(_ledPin, _on);
            _fptr(aID, _on);
        } break;
    }
}


void Footswitch::simulate_mechanical(uint8_t aID, EButtonScanResult aResult) {
    switch (aResult) {
        case EButtonDown: {
            // button down: toggle fx on if it wasn't
            // Serial.print(aID);
            // Serial.println(F(" - DOWN"));
            if(_on == false) {
                _on = true;
                _disable = false;
                _fptr(aID, true);
                digitalWrite(_ledPin, true);
            }
            else {
                _disable = true;   
            }
        } break;

        case EButtonUp: {
            // button released from shortpress: ignore
            // Serial.print(aID);
            // Serial.println(F(" - UP"));
        } break;

        case EButtonClick: {
            // button clicked: turn fx off if it was on
            // Serial.print(aID);
            // Serial.println(F(" - CLICK"));
            if(_on == true && _disable) {
                _on = false;
                _fptr(aID, false);
                digitalWrite(_ledPin, false);
            }        
        } break;

        case EButtonLongpress: {
            // switch temp mode
            // Serial.print(aID);
            // Serial.println(" - LP");
            _led = true;
            _blinkTS = millis();
        } break;
        case EButtonHold: {
            if (millis() - _blinkTS > _blinkIntervalMS) {
                _led = !_led;
                digitalWrite(_ledPin, _led);
                _blinkTS = millis();
            }    
        } break;
        case EButtonUnlongpress: {
            // Serial.print(aID);
            // Serial.println(" - ULP");
            _on = false;
            digitalWrite(_ledPin, false);
            _fptr(aID, false);
        } break;
    }
}

void Footswitch::enable(bool aEnable) {
    // Serial.print(_id);
    // Serial.print(F(" enable: "));
    // Serial.println(aEnable);
    _on = aEnable;
    digitalWrite(_ledPin, aEnable);
    _fptr(_id, aEnable);
}