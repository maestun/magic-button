#include "button.h"

class BypassLoop : public IButtonListener {

private:
    Button *    _button;
    bool        _fx_on = false;
    bool        _fx_disable = false;

protected:
    uint8_t     _relay_pin;
    uint8_t     _temp_led_pin;
    uint8_t     _latch_led_pin;

    // IButtonListener
private:
    void        onButtonEvent(uint8_t aPin, EButtonScanResult aResult);

protected:
    void        updateBypass(bool aON);
    void        updateDisplay(bool aON, bool aTemp);


public:
    void scan();
    BypassLoop(uint8_t aButtonPin, uint8_t aRelayPin, uint8_t aTempLEDPin, uint8_t aLatchLEDPin, uint16_t aLongpressDelayMS);
};