#pragma once
#include "stm32f0xx.h"

namespace stm32 {

class Timer {
 public:
  enum class Channel {
    Chan1 = 1,
    Chan2 = 2,
    Chan3 = 3,
    Chan4 = 4,
  };
  enum class Timer {
    Tim1 = 0,
  };
  Timer(uint8_t timer, uint16_t period_us);
  void EnableOutputChannel(uint8_t channel);

  void SetOutputDuty(uint16_t )
  void SetPeriod(uint16_t us);
  void SetDuty(float duty);
  void SetPulseWidth(uint16_t us);

 private:
  uint8_t timer_;
  uint16_t period_;
  uint16_t channels_[4];
};

}  // namespace stm32
