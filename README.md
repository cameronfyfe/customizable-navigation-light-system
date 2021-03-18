
# TODO: Title/Board/Product Name

## Overview
The board has 4 output pins for external LEDs that can be configured in various modes/flashing sequences.
| LED    | Description                                                                                            |
| ------ | ------------------------------------------------------------------------------------------------------ |
| Strobe | 3 selectable flashing modes with adjustable timing via button menu<br>Enabled/Disabled via RC switches |
| Beacon | Adjustable flashing timing via button menu<br>Enabled/Disabled via RC switches                         |
| Aux 1  | On/Off via RC switches                                                                                 |
| Aux 2  | On/Off via RC switches                                                                                 |

The board has 3 buttons for selecting modes and 1 potentiometer/knob for adjusting parameters.
| Input | Description                                                            |
| ----- | ---------------------------------------------------------------------- |
| Btn 1 | Switches between Strobe and Beacon config modes                        |
| Btn 2 | Controls adjustment of parameters for Strobe and Beacon configurations |
| Btn 3 | Saves current configuration to EEPROM while in config mode             |
| Pot   | Adjusts currently selected parameter in config mode                    |

## Operation
### Modes
The board has 4 modes:
| Mode                  | Description |
| --------------------- | ----------- |
| 1. Normal Mode        | Beacon, Strobe, Aux1, and Aux2 outputs are controlled according the board's configuration and RC switch signals. |
| 2. Manual Mode        | Beacon and Strobe are controlled according to the board's configuration.  Aux1 is ON. Aux2 is OFF. |
| 3. Strobe Config Mode | Strobe configuration is adjustable from this mode.<br>While in this mode Strobe flashes according to config as it's adjusted.  All other outputs are OFF. |
| 4. Beacon Config Mode | Beacon configuration is adjustable form this mode.<br>While in this mode Beacon flashes according to config as it's adjusted.  All other outputs are OFF. |

### Normal Mode
In Normal Mode, the LED outputs will be controlled by switch signals wired to the board.

`TODO, table mapping switch positions to signal enable/disables as described in video or with other options for Aux2`

- A short press (< 1 second) of Btn 1 will switch to Strobe Config Mode
- A long press (>= second) of Btn 1 will switch to Manual Mode

### Manual Mode
In Manual Mode, the Strobe, Beacon, and Aux 1 signals are all enabled and Aux 2 is disabled.  In this mode any RC switch signals wired to the board are ignored.
- A short press (< 1 second) of Btn 1 will switch to Strobe Config Mode
- A long press (>= second) of Btn 1 will switch back to Normal Mode

### Strobe Config Mode
While in Strobe Config Mode you can use Btn 2 and the Pot to select from the 3 Strobe modes and adjust the timing of each.

At any point in Strobe Config mode you can press Btn 3 to save any changes you've made in Strobe Config Mode or press Btn 1 to discard changes and switch to Beacon Config Mode.

After 60 seconds of inactivity in Strobe Config Mode, the board will switch to Normal Mode without saving any unsaved changes.

 - A short press (< 1 second) of Btn 2 will switch between the 3 strobe flashing pattern selections.  
 - A long press (>= 1 second) of Btn 2 will enter spacing adjustment mode for the currently selected flashing pattern.  While in spacing adjustment mode, the potentiometer can be turned to adjust the spacing time between it's min and max values.  While in spacing adjustment mode, pressing Btn 2 again (short or long will return user back to the base Strobe Config Mode).

### Beacon Config Mode
While in Beacon Config Mode you can use Btn 2 and the Pot to adjust the pulse timing for the beacon LED.

At any point in Beacon Config mode you can press Btn 3 to save any changes you've made in Beacon Config Mode or press Btn 1 to discard changes and switch to Strobe Config Mode.

After 60 seconds of inactivity in Beacon Config Mode, the board will switch to Normal Mode without saving any unsaved changes.

 - A short press (< 1 second) of Btn 2 will switch between selection of ON and OFF timing.
 - A long press (>= 1 second) of Btn 2 will enter timing adjustment mode for the currently selected time (ON or OFF).  While in timing adjustment mode, pressing Btn 2 again (short or long will return user back to the base Beacon Config Mode).

### Hardcoded Config Parameters
| Parameter                 | Value | Unit |
| ------------------------- | ----- | ---- |
| STROBE_SPACE_TIME_MIN     | 100   | ms   |
| STROBE_SPACE_TIME_MAX     | 5000  | ms   |
| STROBE_SPACE_TIME_DEFAULT | 500   | ms   |
| BEACON_ON_TIME_MIN        | 10    | ms   |
| BEACON_ON_TIME_MAX        | 2000  | ms   |
| BEACON_ON_TIME_DEFAULT    | 30    | ms   |
| BEACON_OFF_TIME_MIN       | 10    | ms   |
| BEACON_OFF_TIME_MAX       | 5000  | ms   |
| BEACON_OFF_TIME_DEFAULT   | 1000  | ms   |
