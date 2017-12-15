/*
 * User event => callback event
 * ----------------------------
 * Press button => EButtonDown
 * Release button (before longpress time) => EButtonUp, then EButtonClick
 * Keep button pressed for lonpress time => EButtonLongpress
 * Release button (after longpress time) => EButtonUp, then EButtonUnlongpress
 *
 */
#include <stdint.h>

typedef enum EButtonScanResult {
    EButtonUp = 0,      // off, idle state
    EButtonDown,        // button is pressed
    EButtonClick,       // down then up events happened < longpress time
    EButtonLongpress    // button help down for > longpress time
    EButtonUnlongpress  // button up from longpress
} EButtonScanResult;

// event_callback(pin, event)
typedef void (*button_cb_t)(uint8_t, EButtonScanResult);

typedef struct SButtonData {
    uint8_t     pin;
    bool        longpress;
    uint32_t    longpressTS;
    uint16_t    longpressDelay;
    button_cb_t callback;
} SButtonData;

void HW_SetupButton(SButtonData * aButton, uint8_t aPin, uint16_t aLongpressDelay, button_cb_t aCallback);
void HW_ScanButton(SButtonData * aButton);
