# Controlling ITHO Mechanical Ventilation

:alert: This repository was updated after too many years of being stale :alert:

I've removed everything that wasn't working any more and provided a more clear way how to add this.

For previous notes, please dive into the older commits :)

## Requirements

- An ESP8266 (or compatible)
- A C1101 board (including coil)
- Some tinkering [see Mechanics](#mechanics)
- Manually placing some files inside of Home Assistants `/config/esphome` directory


## How-to using Home Assistant

Ensure you have the hardware ready ([Mechanics](#mechanics)) and the `esphome` add-on installed 

- Copy the directory structure of `config` into HA's `/config` (or manually ensure you have the dir `ITHO` and it contents present in `/config/esphome` and start configuring from the UI
- See the sample contents in `/config/esphome/itho.yaml` for some idea of what is neccesary (obviously needs keys and secrets changed)
- Build and install on the ESP of you choice from Home Assistant
- Go into HA, choose `integrations` and add your ITHO (it should auto-discover it).
- If everything goes well, you'll see `switch.fansend...` and quiet a few others popping up.
- Enjoy!


## Current status:

 - Sending High/Medium/Low and the Timers work
 - Reading state and indicative time (not counting down (yet) - should HA or ESPhome do that?)

## ESPHome ITHO control
Trying to get ESPHome to mimic what is comprised in
 
 - https://github.com/jodur/ESPEASY_Plugin_ITHO/blob/master/_P145_Itho.ino
 - https://github.com/adri/IthoEcoFanRFT / https://github.com/supersjimmie/IthoEcoFanRFT


# Mechanics

## Wiring schema used

```
Connections between the CC1101 and the ESP8266 or Arduino:
CC11xx pins    ESP pins Arduino pins  Description
*  1 - VCC        VCC      VCC           3v3
*  2 - GND        GND      GND           Ground
*  3 - MOSI       13=D7    Pin 11        Data input to CC11xx
*  4 - SCK        14=D5    Pin 13        Clock pin
*  5 - MISO/GDO1  12=D6    Pin 12        Data output from CC11xx / serial clock from CC11xx
*  6 - GDO2       04=D2    Pin  2        Programmable output
*  7 - GDO0       ?        Pin  ?        Programmable output
*  8 - CSN        15=D8    Pin 10        Chip select / (SPI_SS)
```

# Dependencies

 - Home Assistant
 - ESPHome
 - https://github.com/CoMPaTech/esphome_itho

