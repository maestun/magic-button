#include "magicbypass.h"

// TODO:
// save last bypass state to eeprom:
// #define ON_BOOT_ENGAGE   0x3
// #define ON_BOOT_BYPASS   0x4
// #define ON_BOOT_REMEMBER 0x5


#ifdef _DEBUG
#  define     dprintinit(x)           Serial.begin(x)
#  define     dprint(x)               Serial.print(x)
#  define     dprintln(x)             Serial.println(x)
#else
#  define     dprintinit(x)
#  define     dprint(x)
#  define     dprintln(x)
#endif

typedef enum eButtonScanResult {
    eButtonUp = 0,      // off, idle state
    eButtonDown,        // button is pressed
    eButtonClick,       // down then up events happened < longpress time
    eButtonLongpress,   // button help down for > longpress time
    eButtonHold,        // button is still held after longpress
    eButtonUnlongpress  // button up from longpress
} eButtonScanResult;

static uint8_t          _pin_switch;
static uint8_t          _pin_led;
static uint8_t          _pin_led_temp;
static uint8_t          _pin_relay;

static bool             _prev_state = false;
static bool             _longpressed = false;
static uint32_t         _longpress_ts = 0;
static uint32_t         _debounce_ts = 0;
static uint16_t         _longpress_ms = 0;
static uint16_t         _debounce_ms = 0;
static uint16_t         _audio_cut_ms = 0;

static bool             _fx_on = false;
static bool             _fx_disable = false;

// ----------------------------------------------------------------------------
// HARDWARE DEPS
// ----------------------------------------------------------------------------
#if TARGET_ATTINY13 || TARGET_ATTINY85
static void _init() {
    DDRB &= ~(1 << _pin_switch); // input
    PORTB |= (1 << _pin_switch); // activate pull-up resistor
    DDRB |= (1 << _pin_relay); // output
    // DDRB |= (1 << PIN_ENGAGE); // output
    DDRB |= (1 << _pin_led); // output
    DDRB |= (1 << _pin_led_temp); // output
}

inline static uint8_t _read() {
    return PINB & (1 << _pin_switch);
}

static void _write(uint8_t aPin, uint8_t aON) {
    if (aON) {
        PORTB |= (1 << aPin);
    }
    else {
        PORTB &= ~(1 << aPin);
    }
}
#endif


// ----------------------------------------------------------------------------
// PRIVATE API
// ----------------------------------------------------------------------------
void _toggle_bypass(bool aON, bool aTemp) {
    // manage effect
    // _write(octocoupler_pin, HIGH);
    _write(_pin_relay, aON);
    delay(_audio_cut_ms);
    // _write(octocoupler_pin, LOW);
    
    // manage display
    if(aON) {
        _write(_pin_led_temp, aTemp);
        _write(_pin_led, !aTemp);
    }
    else {
        // turn LEDs off
        _write(_pin_led_temp, 0);
        _write(_pin_led, 0);
    }
}


void _handle_button(eButtonScanResult aResult) {
    if(aResult == eButtonDown) {
        // button down: _toggle_bypass fx on if it wasn't
        dprintln(F("DN"));
        if(!_fx_on) {
            _fx_on = true;
            _fx_disable = false;
            _toggle_bypass(true, false);
        }
        else {
            _fx_disable = true;   
        }
    }
    else if(aResult == eButtonLongpress) {
        // button longpressed: switch to temporary mode
        dprintln(F("L"));
        _toggle_bypass(true, true);
    }
    else if(aResult == eButtonUnlongpress) {
        // button released from longpress, turn fx off
        _fx_on = false;
        dprintln(F("UL"));
        _toggle_bypass(false, false);
    }
    else if(aResult == eButtonClick) {
        // button clicked: turn fx off if it was on
        dprintln(F("CK"));
        if(_fx_on && _fx_disable) {
            _fx_on = false;
            _toggle_bypass(false, false);
        }
    }
    else if(aResult == eButtonUp) {
        // button released from shortpress: ignore
        dprintln(F("UP"));  
    }
}


// ----------------------------------------------------------------------------
// BUTTON
// ----------------------------------------------------------------------------

void _button_released() {
    // unclick
    if(_longpressed == false) {
        _handle_button(eButtonUp);
        _handle_button(eButtonClick);
    }
    else {
        // unlongpress
        _handle_button(eButtonUnlongpress);
        _longpressed = false;
    }
}


void _button_pressed() {

    // previous code w/ longpress detection
    if(_longpressed == false && (millis() - _longpress_ts) >= _longpress_ms) {
        _longpressed = true;
        _handle_button(eButtonLongpress);
    }
    if(_longpressed == true) {
        _handle_button(eButtonHold);
    }
}


// ----------------------------------------------------------------------------
// API
// ----------------------------------------------------------------------------
void mbp_tick() {
    bool _switch_state = _read();
    if (_switch_state != _prev_state) {
        // reset the debouncing timer
        _debounce_ts = millis();
    }

    if ((millis() - _debounce_ts) > _debounce_ms) {
        // check state only if debounced
        if(_switch_state == true) {
            // pressed
            _button_pressed();
        }
        else {
            // released
            _button_released();
        }
    }
    _prev_state = _switch_state;
}


void mbp_init(uint8_t aPinSwitch, 
              uint8_t aPinLED, 
              uint8_t aPinLEDTemp, 
              uint8_t aPinRelay,
              uint16_t aLongpressMS,
              uint16_t aDebounceMS,
              uint16_t aAudioCutMS) {

    _pin_switch = aPinSwitch;
    _pin_led = aPinLED;
    _pin_led_temp = aPinLEDTemp;
    _pin_relay = aPinRelay;
    _longpress_ms = aLongpressMS;
    _debounce_ms = aDebounceMS;
    _audio_cut_ms = aAudioCutMS;
    _init();
}
