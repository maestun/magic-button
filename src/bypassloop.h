#include "button.h"

static constexpr uint32_t kBlinkDurationMS = 50;

class FootswitchBypass : public IButtonListener {

private:
    Button *    _button;
    bool        _fx_on = false;
    bool        _fx_temp = false;
    bool        _fx_disable = false;
    bool        _blink_led_on_temp = false;
    uint32_t    _blinkTS = 0;
    bool        _blink_state = false;

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

    /**
     * @brief Detect the state of the footswitch: this should be called in the loop() routine.
     * 
     */
    void tick();

    /**
     * @brief Construct a new Footswitch Bypass object
     * 
     * @param aButtonPin Pin to connect the bypass footswitch.
     * @param aRelayPin Pin to connect the bypass relay's coil.
     * @param aTempLEDPin 'Temp Mode' LED pin. If value is kNullPin or same as aLatchLEDPin, then the latch LED blinks in 'temp mode'.
     * @param aLatchLEDPin 'Latch Mode' LED pin.
     * @param aLongpressDelayMS Delay in milliseconds from which the 'temp mode' is toggled when the footswitch is held down.
     * @param aPinMode Set to INPUT or INPUT_PULLUP depending on the hardware setup.
     */
    FootswitchBypass(uint8_t aButtonPin, 
                     uint8_t aRelayPin, 
                     uint8_t aTempLEDPin, 
                     uint8_t aLatchLEDPin, 
                     uint16_t aLongpressDelayMS,
                     uint8_t aPinMode = INPUT);
};