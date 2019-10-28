`!!Work in progress!!`

## NOTE - this repo uses custom-HomeAssistant scripting

`If you like native ESPhome scripting and 'just having a fan' in HA, use the alternative repo`

As per request by @jodur there is also is https://github.com/CoMPaTech/esphome_c1101_alt which is a quick (but working!) demo of (ab)using the ESPhome fan & output 

# ESPHome ITHO control
Trying to get ESPHome to mimic what is comprised in
 
 - https://github.com/jodur/ESPEASY_Plugin_ITHO/blob/master/_P145_Itho.ino
 - https://github.com/adri/IthoEcoFanRFT / https://github.com/supersjimmie/IthoEcoFanRFT


# How-to

Not tested from HA yet, just from CLI. Assumption: you have a machine ready with the ESP8266 and C1101 connected to it.

That machine is also ready to go as instructed per https://esphome.io/guides/getting_started_command_line.html (`pip install`)

- Copy `itho.yaml` and `c1101h` to a directory of choice
- Run `esphome itho.yaml run` and watch the magic happen
- Go into HA, choose `integrations` and add your ITHO
- If everything goes well, you'll see `sensor.fanspeed`, `sensor.fantimer` and a couple of `switch.fansend***` popping up.
- Now include the contents of `HA_configuration.yaml` in your Home-Assitant configuration and reload HA
- Enjoy!


# Current status:

 - Sending High/Medium/Low and the Timers work
 - Reading state and indicative time (not counting down (yet) - should HA or ESPhome do that?)

# Current issues:

 - How to have 'multiple' switches like we have multiple sensors -> for now comprised using template fan in HA

# Fixed issue

 - Adding join command switch as requested by @danieldotnl and as well by @tomduijf
 - Adding 'fan' control in HA

# Wiring schema used:

```
Connections between the CC1101 and the ESP8266 or Arduino:
CC11xx pins    ESP pins Arduino pins  Description
*  1 - VCC        VCC      VCC           3v3
*  2 - GND        GND      GND           Ground
*  3 - MOSI       13=D7    Pin 11        Data input to CC11xx
*  4 - SCK        14=D5    Pin 13        Clock pin
*  5 - MISO/GDO1  12=D6    Pin 12        Data output from CC11xx / serial clock from CC11xx
*  6 - GDO2       04=D1    Pin  2        Programmable output
*  7 - GDO0       ?        Pin  ?        Programmable output
*  8 - CSN        15=D8    Pin 10        Chip select / (SPI_SS)
```

# Dependencies

 - ESPHome
 - https://github.com/CoMPaTech/esphome_itho

