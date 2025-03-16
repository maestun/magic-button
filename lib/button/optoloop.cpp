#include "optoloop.h"

Optoloop::Optoloop(uint8_t aButtonPin, uint8_t aOctocouplerPin, uint8_t aRelayPin, uint8_t aTempLEDPin, uint8_t aLatchLEDPin, uint16_t aLongpressDelayMS, uint8_t aAudioCutMS) {
    button = new Button(aButtonPin, aLongpressDelayMS, this);
    octocoupler_pin = aOctocouplerPin;
    relay_pin = aRelayPin;
    temp_led_pin = aTempLEDPin;
    latch_led_pin = aLatchLEDPin;
    audio_cut_ms = aAudioCutMS;
    
    pinMode(octocoupler_pin, OUTPUT);
    pinMode(relay_pin, OUTPUT);
    pinMode(temp_led_pin, OUTPUT);
    pinMode(latch_led_pin, OUTPUT);
    
    digitalWrite(octocoupler_pin, LOW);
    digitalWrite(relay_pin, LOW);
    digitalWrite(temp_led_pin, LOW);
    digitalWrite(latch_led_pin, LOW);
}


void Optoloop::toggle(bool aON, bool aTemp) {
    // manage effect
    digitalWrite(octocoupler_pin, HIGH);
    digitalWrite(relay_pin, aON);
    delay(audio_cut_ms);
    digitalWrite(octocoupler_pin, LOW);
    
    // manage display
    if(aON) {
        digitalWrite(temp_led_pin, aTemp);
        digitalWrite(latch_led_pin, !aTemp);
    }
    else {
        // turn LEDs off
        digitalWrite(temp_led_pin, false);
        digitalWrite(latch_led_pin, false);
    }
}


void Optoloop::onButtonEvent(uint8_t aPin, EButtonScanResult aResult) {
    if(aResult == EButtonDown) {
        // button down: toggle fx on if it wasn't
        // Serial.println(F("DOWN"));
        if(!fx_on) {
            fx_on = true;
            fx_disable = false;
            toggle(true, false);
        }
        else {
            fx_disable = true;   
        }
    }
    else if(aResult == EButtonLongpress) {
        // button longpressed: switch to temporary mode
        // Serial.println(F("LONG"));
        toggle(true, true);
    }
    else if(aResult == EButtonUnlongpress) {
        // button released from longpress, turn fx off
        fx_on = false;
        // Serial.println(F("UNLONG"));
        toggle(false, false);
    }
    else if(aResult == EButtonClick) {
        // button clicked: turn fx off if it was on
        // Serial.println(F("CLICK"));
        if(fx_on && fx_disable) {
            fx_on = false;
            toggle(false, false);
        }
    }
    else if(aResult == EButtonUp) {
        // button released from shortpress: ignore
        // Serial.println(F("UP"));  
    }
}


void Optoloop::scan() {
    button->scan();
}
