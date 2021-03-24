
#include <Arduino.h>
#include "buttons.h"


// Pin Definitions
// button pins need to stay in D8-D13 otherwise pci scheme also needs to be updated (see PCI setup code and ISRs)
#define BTN_1_PIN     8
#define BTN_1_REG_BIT 0

#define BTN_2_PIN     9
#define BTN_2_REG_BIT 1

#define BTN_3_PIN     10
#define BTN_3_REG_BIT 2


// Config
#define BTN1_LONG_HOLD 1000 // ms
#define BTN2_LONG_HOLD 1000 // ms
#define BTN3_LONG_HOLD 1000 // ms


// Button event buffer
static ButtonEvent _btn_event = NO_EVENT;


/* ---------- Private Function Declarations ---------- */
void Buttons_SendEvent(ButtonEvent evt);


/* ---------- Button Class ---------- */
// handles debouncing and short/long press identification
class Button {
  public:
  
    // Constructor
    Button(byte pin, ButtonEvent short_evt, ButtonEvent long_evt, uint32_t hold_time=1000, uint32_t t_debounce=10) :
    _pin(pin),
    _short_evt(short_evt),
    _long_evt(long_evt),
    _hold_time(hold_time)
    {
      _last_fall_time = 0;
    }

    // Initialize button pin config
    void Init()
    {
      pinMode(_pin, INPUT_PULLUP);
      _pin_state = digitalRead(_pin);
      
      *digitalPinToPCMSK(_pin) |= bit(digitalPinToPCMSKbit(_pin)); // enable pin
      PCIFR |= bit(digitalPinToPCICRbit(_pin)); // clear any outstanding interrupt
      PCICR |= bit(digitalPinToPCICRbit(_pin)); // enable interrupt for the group
    }

    // Update button state
    // (can call from polling loop or from interrupt)
    void Update()
    {
      uint32_t ms = millis();

      bool new_pin_state = digitalRead(_pin);
      
      if (new_pin_state != _pin_state)
      {
        // Debounce
        if (ms - _last_pin_change < _t_debounce)
        {
          return;
        }
        // Rising Edge (RELEASE)
        if (new_pin_state)
        {
          uint32_t hold_time = ms - _last_fall_time;
          Buttons_SendEvent(hold_time < _hold_time ? _short_evt : _long_evt);
        }
        // Falling Edge (PRESS)
        else
        {
          _last_fall_time = ms;
        }
        // Update pin state
        _pin_state = new_pin_state;
        _last_pin_change = ms;
      }      
    }
    
  private:
    const byte _pin;              // pin number
    const ButtonEvent _short_evt; // short button press event enum
    const ButtonEvent _long_evt;  // long button press event enum
    const uint32_t _hold_time;    // hold time for identifying short vs long hold
    const uint32_t _t_debounce;   // debounce time
    
    bool _pin_state;           // hold pin state
    uint32_t _last_fall_time;  // hold time of last falling edge (button press)
    uint32_t _last_pin_change; // hold time of last pin change
};


// Buttons Array
#define NUM_BTNS (sizeof(btns)/sizeof(Button))
Button btns[] = {
  Button(BTN_1_PIN, BTN_1_SHORT, BTN_1_LONG, BTN1_LONG_HOLD),
  Button(BTN_2_PIN, BTN_2_SHORT, BTN_2_LONG, BTN2_LONG_HOLD),
  Button(BTN_3_PIN, BTN_3_SHORT, BTN_3_LONG, BTN3_LONG_HOLD)
};


/* ---------- Interrupts ---------- */

// Port B (D8-D13) Pin Change Interrupt
ISR (PCINT0_vect)
{
  for (uint8_t i=0; i<NUM_BTNS; i++)
  {
    btns[i].Update();
  }
}


/* ---------- Public Function Definitions ---------- */

// Initalize Button pins
void Buttons_Init()
{
  // Initialize button pin inputs
  for (uint8_t i=0; i<NUM_BTNS; i++)
  {
    btns[i].Init();
  }
}


// Sent Button Event
void Buttons_SendEvent(ButtonEvent evt)
{  
  _btn_event = evt;
}


// Get Button Event
ButtonEvent Buttons_GetEvent()
{  
  ButtonEvent evt = _btn_event;

  // Clear event
  _btn_event = NO_EVENT;
  
  return evt;
}
