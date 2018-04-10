#pragma once
#include <utility>
#include <cstdint>
#include "usart.h"


// Communication steps
// 1. Board boots
// 2. Check every UART Rx pin to see if it's pulled up
// 3. If the pin is pulled up, request location from that neighbor
// 4. Receive locations and compare to make sure they match up.
// 5. Receive pattern from neighbor, if one has been uploaded
// 6. Poll every uart to check for messages

enum MessageType {
  GetLocation = 0,
  SendLocation = 1,
  SendPattern = 2,
};

enum ControlChar {
  DLE = 0x10,
  SOH = 0x01,
  ETX = 0x02,
  ACK = 0x03,
  NACK = 0x04,
};

// Basic message
struct MessageHeader {
  uint8_t soh;
  uint8_t type;
};

// Empty Message Request
struct GetLocationMessage {
  constexpr static int Type = GetLocation;
};

struct SendLocationMessage {
  int8_t x;
  int8_t y;
  constexpr static int Type = SendLocation;
};

struct Pattern {
  // [FrameID][X][Y]
  int8_t frames[10][5][5];
};

// TODO: Optimize with a bit mask
struct SendPatternMessage {
  Pattern pattern;
  constexpr static int Type = SendPattern;
};

struct MessageTrailer {
  uint8_t etx;
};

class Comm;



class Neighbor {
 public:
  Neighbor(stm32::USART usart);
  Neighbor() {}

  // Note: Disables the USART on the pin. Should only be called at startup before communication begins
  bool Online();

  std::tuple<int8_t, int8_t> GetLocation();
  bool GetPattern(SendPatternMessage &message);
  bool SendLocation(SendLocationMessage message);


 private:
  void HandleMessage();
  bool SendMessage();
  bool online_;
  stm32::USART dir_;
  stm32::Usart usart_;
  friend class Comm;
};

class Comm {
 public:
  Comm(stm32::USART n, stm32::USART w, stm32::USART s, stm32::USART e);

 private:
  std::tuple<int, int> GetLocationFromNeighbor(stm32::Usart neighbor);
  Neighbor n_;
  Neighbor w_;
  Neighbor s_;
  Neighbor e_;

  Pattern pattern_;

  friend class Neighbor;
};
