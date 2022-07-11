/*
 * User event => callback event
 * ----------------------------
 * Press button => EButtonDown
 * Release button (before longpress time) => EButtonUp then EButtonClick
 * Keep button pressed for lonpress time => EButtonLongpress
 * Release button (after longpress time) => EButtonUnlongpress
 *
 */
#include <stdint.h>
#include <Arduino.h>

static constexpr uint8_t kPinNull = 0xff;

typedef enum EButtonScanResult {
    EButtonUp = 0,      // off, idle state
    EButtonDown,        // button is pressed
    EButtonClick,       // down then up events happened < longpress time
    EButtonLongpress,   // button help down for > longpress time
    EButtonHold,        // button is still held after longpress
    EButtonUnlongpress  // button up from longpress
} EButtonScanResult;


class IButtonListener {
public:
    virtual void onButtonEvent(uint8_t aPin, EButtonScanResult aResult) = 0;
};

class Button {
private:
    bool                _pressed;
    uint8_t             _pin;
    uint8_t             _prevState;
    bool                _longpressed;
    uint32_t            _longpressTS;
    uint32_t            _debounceTS;
    uint16_t            _longpressMS;
    IButtonListener *   _listener;
    void                onButtonReleased();
    void                onButtonPressed();
    
public:
    Button(uint8_t aPin, uint16_t aLongpressDelayMS, IButtonListener * aListener, uint8_t aPinMode);
    void tick();
};
