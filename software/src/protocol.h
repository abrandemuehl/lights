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

constexpr int kMaxMessageSize = 100;

enum MessageType {
  GetLocation = 0,
  SendLocation = 1,
  SendPattern = 2,
};

enum ControlChar {
  DLE = 0x10,
  SOH = 0x01,
  ETX = 0x02,
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
  Neighbor(stm32::Usart *usart);
  Neighbor() {}

  // Note: Disables the Usart on the pin. Should only be called at startup before communication begins
  bool Online();

  std::tuple<int8_t, int8_t> GetLocation();
  bool GetPattern(SendPatternMessage &message);
  bool SendLocation(SendLocationMessage message);

  bool ProcessBuffer();


 private:
  bool isControlChar(uint8_t data);
  void HandleMessage();
  template <typename T>
  void SendMessage(T &message);
  bool online_;
  stm32::Usart *usart_;
  friend class Comm;
};

class Comm {
 public:
  Comm(stm32::Usart *n, stm32::Usart *w, stm32::Usart *s, stm32::Usart *e);

 private:
  Neighbor n_;
  Neighbor w_;
  Neighbor s_;
  Neighbor e_;

  Pattern pattern_;

  friend class Neighbor;
};

template <typename T>
void Neighbor::SendMessage(T &message) {
  uint8_t buf[kMaxMessageSize];
  uint8_t *ptr = buf;

  // Insert header
  MessageHeader *header = reinterpret_cast<MessageHeader *>(ptr);
  header->soh = SOH;
  header->type = T::Type;
  ptr += sizeof(*header);

  // Insert real message
  T *msg = reinterpret_cast<T *>(ptr);
  msg = *message;
  ptr += sizeof(*message);

  // Insert trailer
  MessageTrailer *trailer = reinterpret_cast<MessageTrailer *>(ptr);
  trailer->etx = ETX;

  int size = sizeof(*header) + sizeof(*message) + sizeof(*trailer);

  // Handle delimiting
  uint8_t data[kMaxMessageSize];
  data[0] = SOH;
  int idx = 1;
  for (int i = 1; i < size - 1; i++) {
    if (isControlChar(buf[i])) {
      // Insert a DLE and mask the byte
      data[idx++] = DLE;
      data[idx++] = buf[i] | 0x80;
    } else {
      data[idx++] = buf[i];
    }
  }
  data[idx++] = ETX;

  // Send delimited data
  usart_->Write(data, idx);
}
