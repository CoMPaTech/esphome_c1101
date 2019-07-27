#include "esphome.h"
#include "IthoCC1101.h"
#include "Ticker.h"

IthoCC1101 rf;
void ITHOinterrupt() ICACHE_RAM_ATTR;
void ITHOcheck();

// extra for interrupt handling
bool ITHOhasPacket = false;
Ticker ITHOticker;
int State=1; // after startup it is assumed that the fan is running low
int OldState=1;
int Timer=0;
int LastIDindex = 0;
int OldLastIDindex = 0;
long LastPublish=0; 
bool InitRunned = false;

// Timer values for hardware timer in Fan
#define Time1      10*60
#define Time2      20*60
#define Time3      30*60


class FanRecv : public PollingComponent, public Sensor {
  public:

    // Publish two sensors
    // Speed: the speed the fan is running at (depending on your model 1-2-3 or 1-2-3-4
    Sensor *fanspeed = new Sensor();
    // Timer left (though this is indicative) when pressing the timer button once, twice or three times
    Sensor *fantimer = new Sensor();

    // For now poll every 15 seconds
    FanRecv() : PollingComponent(15000) { }

    void setup() {
      rf.init();
      // Followin wiring schema, change PIN if you wire differently
      pinMode(D1, INPUT);
      //Breaking (reboot loop on ESPs)
      //attachInterrupt(D1, ITHOcheck, RISING);
      rf.initReceive();
    }

    void update() override {
        fanspeed->publish_state(State);
        fantimer->publish_state(Timer);
    }


};

class FanSendHigh : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoHigh);
        State = 3;
        Timer = 0;
        publish_state(!state);
      }
    }
};

class FanSendLow : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoLow);
        State = 1;
        Timer = 0;
        publish_state(!state);
      }
    }
};

void ITHOinterrupt() {
	ITHOticker.once_ms(10, ITHOcheck);
}

void ITHOcheck() {
  noInterrupts();
  ESP_LOGD("custom", "Update called");
  rf.initReceive();
  ESP_LOGD("custom", "Ready to receive");
  if (rf.checkForNewPacket()) {
    ESP_LOGD("custom", "Packet received");
    IthoCommand cmd = rf.getLastCommand();
	ESP_LOGD("custom", "cmd received: %s",cmd);
    switch (cmd) {
      case IthoUnknown:
        State = 0;
        Timer = 0;
        break;
      case IthoLow:
        State = 1;
        Timer = 0;
        break;
      case IthoMedium:
        State = 2;
        Timer = 0;
        break;
      case IthoFull:
        State = 3;
        Timer = 0;
        break;
      case IthoTimer1:
        State = 13;
        Timer = Time1;
        break;
      case IthoTimer2:
        State = 23;
        Timer = Time2;
        break;
      case IthoTimer3:
        State = 33;
        Timer = Time3;
        break;
      case IthoJoin:
        break;
      case IthoLeave:
        break;
    }
  }
  interrupts();
}
