#include "protocol.h"
#include "usart.h"
#include <string.h>
#include "console.h"

#define USART_N (USART3)
#define USART_W (USART4)
#define USART_S (USART5)
#define USART_E (USART6)

#define PATTERN_DIAMETER (5)

// Represents which way this board is oriented. 
typedef enum {
  NORTH,
  SOUTH,
  EAST,
  WEST,
} Direction;

typedef struct {
  int8_t x;
  int8_t y;
  Direction orientation;
  uint8_t initialized;
  uint8_t pattern[5][5];
} Location;


Location location;

typedef struct {
  USART_TypeDef *usart;
  Direction direction;
  int delimited;
  int step;

  // Message data
  int idx;
  uint8_t type;

  int8_t location_x;
  int8_t location_y;
  uint8_t orientation;
  uint8_t pattern[5][5];
  int pattern_idx;
  uint8_t complete_pattern;
} Channel;

Channel channel_N;
Channel channel_W;
Channel channel_S;
Channel channel_E;


/* Protocol definition
 *
 * ----------Packet Header------------
 * uint8_t SOH;
 * uint8_t TYPE;
 *
 * ----------Packet Trailer------------
 * uint8_t ETX;
 *
 * ---------Location Request-------
 * uint8_t TYPE = LocationRequestType
 *
 * ---------Location Response-------
 * uint8_t TYPE = LocationResponseType
 * int8_t X
 * int8_t Y
 * int8_t Orientation
 * uint8_t Pattern[5][5]
 *
 * ---------Synchronization-------
 * uint8_t TYPE = SynchronizationType
 */

#define DLE_MASK (0x80)
enum {
  SOH = 0x1,
  ETX = 0x3,
  DLE = 0x10,
};

typedef enum {
  LocationRequestType=0x20,
  LocationResponseType=0x21,
  SynchronizationType=0x22,
} PacketType;

int isControlChar(uint8_t ch) {
  switch(ch) {
    case SOH:
    case ETX:
    case DLE:
      return 1;
  }
  return 0;
}

// Handle delimiting then send message
void sendMessage(Channel *channel, uint8_t *data, int n) {
  uint8_t packetized[100];

  int idx = 0;
  PRINT("Adding ");
  printByte(data[0]);
  PRINT("\n");
  packetized[idx++] = data[0];

  PRINT("Sending packet: ");
  for(int i=0 ; i < n; i++) {
    printByte(data[i]);
    PRINT(" ");
  }
  PRINT("\n");

  for(int i=1; i < n-1; i++) {
    if(isControlChar(data[i])) {
      PRINT("Adding ");
      printByte(DLE);
      PRINT("\n");
      packetized[idx++] = DLE;

      PRINT("Adding ");
      printByte(data[i] | DLE_MASK);
      PRINT("\n");
      packetized[idx++] = data[i] | DLE_MASK;
    } else {
      PRINT("Adding ");
      printByte(data[i]);
      PRINT("\n");
      packetized[idx++] = data[i];
    }
  }
  PRINT("Adding ");
  printByte(data[n-1]);
  PRINT("\n");
  packetized[idx++] = data[n-1];
  PRINT("Sending message of ");
  printDec(idx);
  PRINT(" bytes\n");

  for(int i=0 ; i < idx; i++) {
    printByte(packetized[i]);
    PRINT(" ");
  }
  PRINT("\n");

  usartWrite(channel->usart, packetized, idx);
}

// Creates a location message in a buffer
void sendLocationRequestMessage(Channel *channel) {
  uint8_t buf[100];
  buf[0] = SOH;
  buf[1] = LocationRequestType;
  buf[2] = ETX;

  sendMessage(channel, buf, 3);
}

// Creates a location message in a buffer
void sendLocationResponseMessage(Channel *channel) {
  PRINT("Sending LocationResponseMessage\n");
  uint8_t buf[100];
  int8_t x = location.x;
  int8_t y = location.y;
  switch(channel->direction) {
    case NORTH:
      y = location.y + 1;
      break;
    case WEST:
      x = location.x + 1;
      break;
    case SOUTH:
      y = location.y - 1;
      break;
    case EAST:
      x = location.x - 1;
      break;
  }
  buf[0] = SOH;
  buf[1] = LocationResponseType;
  buf[2] = (uint8_t)x;
  buf[3] = (uint8_t)y;
  memcpy(&buf[4], location.pattern, sizeof(location.pattern));

  sendMessage(channel, buf, 4 + sizeof(location.pattern));
}

// Creates a location message in a buffer
void sendSynchronizationMessage(Channel *channel) {
  uint8_t buf[100];
  buf[0] = SOH;
  buf[1] = SynchronizationType;
  buf[2] = ETX;

  sendMessage(channel, buf, 3);
}


void protocolInit() {
  memset(&location, 0, sizeof(location));

  // Reset all state machines
  memset(&channel_N, 0, sizeof(channel_N));
  memset(&channel_W, 0, sizeof(channel_W));
  memset(&channel_S, 0, sizeof(channel_S));
  memset(&channel_E, 0, sizeof(channel_E));
  channel_N.usart = USART_N;
  channel_W.usart = USART_W;
  channel_S.usart = USART_S;
  channel_E.usart = USART_E;

  channel_N.direction = NORTH;
  channel_W.direction = WEST;
  channel_S.direction = SOUTH;
  channel_E.direction = EAST;

  // sendLocationRequestMessage(&channel_N);
  // sendLocationRequestMessage(&channel_E);
  sendLocationRequestMessage(&channel_S);
  // sendLocationRequestMessage(&channel_W);
  // while(1) {
  //   PRINT("SENDING REQUEST\n");
  //   sendLocationRequestMessage(&channel_S);
  // }

  for(int i=0; i < 5; i++) {
    protocolProcessMessages();
  }

  if(location.initialized) {
    PRINT("Received location\n");
  } else {
    PRINT("No neighbors. Assigning (0,0)\n");
    location.initialized = 1;
  }
}

void processChannel(Direction dir) {
  Channel *channel;

  switch(dir) {
    case NORTH:
      channel = &channel_N;
      break;
    case WEST:
      channel = &channel_W;
      break;
    case SOUTH:
      channel = &channel_S;
      break;
    case EAST:
      channel = &channel_E;
      break;
  }


  uint8_t data;
  while(usartRead(channel->usart, &data, 1) > 0) {
    PRINT("Received Data ");
    printByte(data);
    PRINT("\n");

    if(channel->step > 0 && data == DLE) {
      channel->delimited = 1;
      continue;
    }

    if(channel->delimited) {
      data &= ~DLE_MASK; 
      channel->delimited = 0;
    }

    PRINT("Step: ");
    printDec(channel->step);
    PRINT("\n");

    switch (channel->step) {
      case 0: // SOH
        if(data == SOH) {
          channel->step = 1;
          PRINT("SOH\n");
        }
        break;
      case 1: // Type
        channel->type = data;
        switch(data) {
          case LocationRequestType:
            PRINT("LocationRequestType\n");
            channel->step = 2;
            break;
          case LocationResponseType:
            PRINT("LocationResponseType\n");
            channel->step = 50;
            break;
          default:
            channel->step = 0;
            continue;
        }
        break;
      case 2: // Location request message
        // Receive an ETX
        if(data == ETX) {
          PRINT("ETX\n");
          if(!location.initialized) {
            // Disregard message
            PRINT("Disregarding. Not initialized yet\n");
          } else {
            // Send a position response message
            sendLocationResponseMessage(channel);
          }
        } else {
          PRINT("Not ETX\n");
        }
        channel->step = 0;
        break;
      case 3:

      case 50: // Location response message
        // x
        channel->location_x = data;
        channel->step++;
        break;
      case 51:
        // x
        channel->location_y = data;
        channel->step++;
        break;
      case 52:
        channel->orientation = data;
        channel->step++;
      case 53:
        channel->pattern[channel->pattern_idx / PATTERN_DIAMETER][channel->pattern_idx % PATTERN_DIAMETER] = data;
        if(channel->pattern_idx > sizeof(channel->pattern)) {
          channel->step++;
        }
        break;
      case 54:
        if(data == ETX) {
          channel->complete_pattern = 1;
        }
        channel->step = 0;
        break;
    }

    if(channel->complete_pattern) {
      // Copy data into location
      location.x = channel->location_x;
      location.y = channel->location_y;

      memcpy(location.pattern, channel->pattern, sizeof(channel->pattern));

      location.initialized = 1;

      channel->complete_pattern = 0;
    }
  }
}


void protocolProcessMessages() {
  // Process channels
  // processChannel(NORTH);
  // processChannel(WEST);
  processChannel(SOUTH);
  // processChannel(EAST);
}
