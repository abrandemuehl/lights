#include "protocol.h"
#include "gpio.h"

bool isControlChar(uint8_t data) {
  switch (data) {
    case DLE:
    case SOH:
    case ETX:
    case ACK:
      return true;
  }
  return false;
}

Neighbor::Neighbor(stm32::USART usart) : dir_(usart), usart_(usart) {}

bool Neighbor::Online() {
  stm32::gpio pin(stm32::kGPIOBase[(int)dir_], stm32::kPinRx[(int)dir_]);
  return pin.Read();
}

template <typename T>
bool SendMessage(T &message) {
  uint8_t buf[256];
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
  uint8_t data[256];
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

  // Send delimited data along uart and wait for ACK
  return true;
}

Comm::Comm(stm32::USART n, stm32::USART w, stm32::USART s, stm32::USART e)
    : n_(n), w_(w), s_(s), e_(e) {}
