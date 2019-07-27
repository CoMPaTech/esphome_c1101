#include "esphome.h"
#include "IthoCC1101.h"
#include "Ticker.h"

IthoCC1101 rf;
void ITHOinterrupt() ICACHE_RAM_ATTR;
void ITHOcheck();

// extra for interrupt handling
bool ITHOhasPacket = false;
Ticker ITHOticker;
String State="Low"; // after startup it is assumed that the fan is running low
String OldState="Low";
int Timer=0;
int LastIDindex = 0;
int OldLastIDindex = 0;
long LastPublish=0; 
bool InitRunned = false;

// Timer values for hardware timer in Fan
#define Time1      10*60
#define Time2      20*60
#define Time3      30*60


class FanRecv : public PollingComponent {
  public:

    // Publish two sensors
    // Speed: the speed the fan is running at (depending on your model 1-2-3 or 1-2-3-4
    TextSensor *fanspeed = new TextSensor();
    // Timer left (though this is indicative) when pressing the timer button once, twice or three times
    TextSensor *fantimer = new TextSensor();

    // For now poll every 15 seconds
    FanRecv() : PollingComponent(15000) { }

    void setup() {
      rf.init();
      // Followin wiring schema, change PIN if you wire differently
      pinMode(D1, INPUT);
      attachInterrupt(D1, ITHOinterrupt, RISING);
      //attachInterrupt(D1, ITHOcheck, RISING);
      rf.initReceive();
    }

    void update() override {
        fanspeed->publish_state(State.c_str());
        fantimer->publish_state(String(Timer).c_str());
    }


};

// Figure out how to do multiple switches instead of duplicating them
// we need
// send: low, medium, high, full
//       timer 1 (10 minutes), 2 (20), 3 (30)
// To optimize testing, reset published state immediately so you can retrigger (i.e. momentarily button press)
class FanSendFull : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoFull);
        State = "High";
        Timer = 0;
        publish_state(!state);
      }
    }
};

class FanSendHigh : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoHigh);
        State = "High";
        Timer = 0;
        publish_state(!state);
      }
    }
};

class FanSendMedium : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoMedium);
        State = "Medium";
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
        State = "Low";
        Timer = 0;
        publish_state(!state);
      }
    }
};

class FanSendIthoTimer1 : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoTimer1);
        State = "High";
        Timer = Time1;
        publish_state(!state);
      }
    }
};

class FanSendIthoTimer2 : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoTimer2);
        State = "High";
        Timer = Time2;
        publish_state(!state);
      }
    }
};

class FanSendIthoTimer3 : public Component, public Switch {
  public:

    void write_state(bool state) override {
      if ( state ) {
        rf.sendCommand(IthoTimer3);
        State = "High";
        Timer = Time3;
        publish_state(!state);
      }
    }
};

void ITHOinterrupt() {
	ITHOticker.once_ms(10, ITHOcheck);
}

void ITHOcheck() {
  noInterrupts();
  if (rf.checkForNewPacket()) {
    IthoCommand cmd = rf.getLastCommand();
    switch (cmd) {
      case IthoUnknown:
        ESP_LOGD("custom", "Unknown state");
        break;
      case IthoLow:
        ESP_LOGD("custom", "IthoLow");
        State = "Low";
        Timer = 0;
        break;
      case IthoMedium:
        ESP_LOGD("custom", "Medium");
        State = "Medium";
        Timer = 0;
        break;
      case IthoHigh:
        ESP_LOGD("custom", "High");
        State = "High";
        Timer = 0;
        break;
      case IthoFull:
        ESP_LOGD("custom", "Full");
        State = "Full";
        Timer = 0;
        break;
      case IthoTimer1:
        ESP_LOGD("custom", "Timer1");
        State = "High";
        Timer = Time1;
        break;
      case IthoTimer2:
        ESP_LOGD("custom", "Timer2");
        State = "High";
        Timer = Time2;
        break;
      case IthoTimer3:
        ESP_LOGD("custom", "Timer3");
        State = "High 30";
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
