
#include <Arduino.h>

#define DEFAULT_LONGPRESS_DELAY  (1000)

typedef enum EButtonScanResult {
    EButtonNone = 0,
    EButtonDown,
    EButtonUp,
    EButtonClick,
    EButtonLongpress
} EButtonScanResult;

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