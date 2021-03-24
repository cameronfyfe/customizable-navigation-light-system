#include "signal_leds.h"
#include "mode_leds.h"


// Pin Definitions
#define STROBE A1
#define BEACON A2
#define AUX1 A3
#define AUX2 A4


/* ---------- Private Function Declarations ---------- */
void LED_Set(byte pin, bool on);



/* ---------- Private Function Definitions ---------- */

// Set LED output pin
void LED_Set(byte pin, bool on)
{
  digitalWrite(pin, on ? LOW : HIGH);
}


/* ---------- Public Function Definitions ---------- */

// Initialze Signal LEDs
void SignalLEDs_Init()
{
  // Strobe LED
  pinMode(STROBE, OUTPUT);
  LED_Set(STROBE, false);
  // Beacon LED
  pinMode(BEACON, OUTPUT);
  LED_Set(BEACON, false);
  // AUX1 LED
  pinMode(AUX1, OUTPUT);
  LED_Set(AUX1, false);
  // AUX2 LED
  pinMode(AUX2, OUTPUT);
  LED_Set(AUX2, false);
}


// Update LEDs (called on loop)
void SignalLEDs_Update(const SignalLEDsConfig &signal_config)
{
  uint32_t ms = millis();
  
  // Strobe LED
  uint32_t strobe_cycle_time = 0;
  for (byte i=0; i<STROBE_TIMES_ARR_SIZE; i++)
  {
    strobe_cycle_time += signal_config.strobe.times.on[i];
    strobe_cycle_time += signal_config.strobe.times.off[i];
  }
  uint32_t strobe_tc = ms % strobe_cycle_time;

  bool strobe_state = true;
  uint32_t timer = 0;
  for (byte i=0; i<STROBE_TIMES_ARR_SIZE; i++)
  {
    timer += signal_config.strobe.times.on[i];
    if (strobe_tc >= timer) { strobe_state = false; }
    timer += signal_config.strobe.times.off[i];
    if (strobe_tc >= timer) { strobe_state = true; }
  }

  LED_Set(STROBE, strobe_state && signal_config.strobe.enabled);

  // Beacon LED
  uint32_t beacon_tc = ms % (signal_config.beacon.on_time + signal_config.beacon.off_time);
  bool beacon_state = (beacon_tc < signal_config.beacon.on_time);
  
  LED_Set(BEACON, beacon_state && signal_config.beacon.enabled);


  // AUX1 LED
  LED_Set(AUX1, signal_config.aux1_on);
  
  // AUX2 LED
  LED_Set(AUX2, signal_config.aux2_on);
}
