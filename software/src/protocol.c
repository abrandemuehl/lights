#include "protocol.h"
#include "usart.h"
#include <string.h>
#include "console.h"
#include "rtc.h"
#include "timer.h"
#include "bitmask.h"

#define USART_N (USART3)
#define USART_W (USART4)
#define USART_S (USART5)
#define USART_E (USART6)

// Frame count must be a divisor of 256 because the counter is one byte
#define FRAME_COUNT (8)


// Represents which way this board is oriented. 
typedef enum {
  NORTH=0,
  WEST=1,
  SOUTH=2,
  EAST=3,
} Direction;

typedef struct {
  int8_t x;
  int8_t y;
  Direction orientation;
  uint8_t initialized;
  Bitmask pattern[FRAME_COUNT];
  uint8_t rotation;
  uint8_t counter;
} State;


State state;

typedef struct {
  USART_TypeDef *usart;
  // Absolute "direction" of this usart channel
  Direction direction;
  int delimited;
  int step;

  // Message data
  int idx;
  uint8_t type;

  int8_t location_x;
  int8_t location_y;
  // Received orientation from neighbors
  uint8_t orientation;
  uint8_t counter;
  Bitmask pattern[FRAME_COUNT];
  int pattern_idx;
  uint8_t complete_pattern;
} Channel;

Channel channel_N;
Channel channel_W;
Channel channel_S;
Channel channel_E;


void processLight();
void protocolProcessMessages();
void processChannel(Direction dir);
void sendSynchronizationMessage(Channel *channel);
void sendLocationResponseMessage(Channel *channel);
void sendLocationRequestMessage(Channel *channel);
void sendMessage(Channel *channel, uint8_t *data, int n);
int isControlChar(uint8_t ch);
Direction rotatedDirection(Direction dir);


Direction rotatedDirection(Direction dir) {
  return (dir + state.rotation) % 4;
}


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
 * uint32_t frame
 * Bitmask Pattern[FRAME_COUNT]
 *
 * ---------Synchronization-------
 * uint8_t TYPE = SynchronizationType
 * uint8_t COUNTER
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
  packetized[idx++] = data[0];

  for(int i=1; i < n-1; i++) {
    if(isControlChar(data[i])) {
      packetized[idx++] = DLE;

      packetized[idx++] = data[i] | DLE_MASK;
    } else {
      packetized[idx++] = data[i];
    }
  }
  packetized[idx++] = data[n-1];

  // for(int i=0 ; i < idx; i++) {
  //   printByte(packetized[i]);
  //   PRINT(" ");
  // }
  // PRINT("\n");

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
  uint8_t buf[256];
  int8_t x = state.x;
  int8_t y = state.y;
  switch(rotatedDirection(channel->direction)) {
    case NORTH:
      y = state.y + 1;
      break;
    case WEST:
      x = state.x + 1;
      break;
    case SOUTH:
      y = state.y - 1;
      break;
    case EAST:
      x = state.x - 1;
      break;
  }
  int idx = 0;
  buf[idx++] = SOH;
  buf[idx++] = LocationResponseType;
  buf[idx++] = (uint8_t)x;
  buf[idx++] = (uint8_t)y;
  buf[idx++] = rotatedDirection(channel->direction);

  for(int i=0; i < FRAME_COUNT; i++) {
    idx += bitmaskSerialize(state.pattern[i], &buf[idx]);
  }

  buf[idx++] = ETX;

  sendMessage(channel, buf, idx);
  return;
}

// Creates a location message in a buffer
void sendSynchronizationMessage(Channel *channel) {
  uint8_t buf[100];
  buf[0] = SOH;
  buf[1] = SynchronizationType;
  buf[2] = state.counter;
  buf[3] = ETX;

  sendMessage(channel, buf, 4);
}


void protocolInit() {
  memset(&state, 0, sizeof(state));

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

  channel_N.step = 0;
  channel_W.step = 0;
  channel_S.step = 0;
  channel_E.step = 0;
  sendLocationRequestMessage(&channel_N);
  sendLocationRequestMessage(&channel_E);
  sendLocationRequestMessage(&channel_S);
  sendLocationRequestMessage(&channel_W);


  for(int i=0; i < 5; i++) {
    protocolProcessMessages();
    for (int i = 0; i < 100000; i++);
    if(state.initialized) {
      break;
    }
  }

  if(state.initialized) {
    PRINT("Received location: (");
    printDec(state.x);
    PRINT(",");
    printDec(state.y);
    PRINT(")\n");
  } else {
    PRINT("No neighbors. Assigning (0,0)\n");
    state.initialized = 1;
  }

  bitmaskWrite(state.pattern[0], 0, 0, 0);
  bitmaskWrite(state.pattern[1], 0, 0, 0);
  bitmaskWrite(state.pattern[2], 0, 0, 0);
  bitmaskWrite(state.pattern[3], 0, 0, 0);
  bitmaskWrite(state.pattern[4], 0, 0, 1);
  bitmaskWrite(state.pattern[5], 0, 0, 1);
  bitmaskWrite(state.pattern[6], 0, 0, 1);
  bitmaskWrite(state.pattern[7], 0, 0, 1);

  bitmaskWrite(state.pattern[0], 0, -1, 1);
  bitmaskWrite(state.pattern[1], 0, -1, 1);
  bitmaskWrite(state.pattern[2], 0, -1, 1);
  bitmaskWrite(state.pattern[3], 0, -1, 1);
  bitmaskWrite(state.pattern[4], 0, -1, 0);
  bitmaskWrite(state.pattern[5], 0, -1, 0);
  bitmaskWrite(state.pattern[6], 0, -1, 0);
  bitmaskWrite(state.pattern[7], 0, -1, 0);

}

void processChannel(Direction dir) {
  Channel *channel;
  dir = rotatedDirection(dir);

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

  // int y;
  // int x;
  // int frame;
  while(usartRead(channel->usart, &data, 1) > 0) {
    PRINT("Received Data ");
    printByte(data);
    PRINT("\n");
    // PRINT("Available: ");
    // printDec(usartAvailable(channel->usart));
    // PRINT("\n");

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
          case SynchronizationType:
            PRINT("SynchronizationType\n");
            channel->step = 25;
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
          if(!state.initialized) {
            // Disregard message
            PRINT("Disregarding. Not initialized yet\n");
          } else {
            // Send a position response message
            PRINT("Responding to locationRequest\n");
            sendLocationResponseMessage(channel);
            PRINT("Done sending location\n");
          }
        } else {
          PRINT("Not ETX\n");
        }
        channel->step = 0;
        break;
      case 25: // Synchronization message type
        // Pull the counter
        channel->counter = data;
        channel->step++;
        break;
      case 26:
        // Receive an ETX
        if(data == ETX) {
          rtcReset();
          state.counter = channel->counter;
          processLight();
          PRINT("Clock synchronized\n");
          PRINT("Counter: ");
          printDec(state.counter);
          PRINT("\n");
        }
        channel->step = 0;
        break;
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
        break;
      case 53:
        channel->counter = data;
        channel->step++;
      case 54:
        ((uint8_t *)channel->pattern)[channel->pattern_idx] = data;
        // y = (channel->pattern_idx % (FRAME_COUNT*PATTERN_DIAMETER)) / PATTERN_DIAMETER;
        // x = channel->pattern_idx % PATTERN_DIAMETER;
        // frame = channel->pattern_idx / (FRAME_COUNT*PATTERN_DIAMETER);
        // channel->pattern[frame][y][x] = data;
        channel->pattern_idx++;
        if(channel->pattern_idx >= sizeof(channel->pattern)) {
          channel->pattern_idx = 0;
          channel->step++;
        }
        break;
      case 55:
        if(data == ETX) {
          PRINT("Got ETX\n");
          channel->complete_pattern = 1;
        } else {
          PRINT("Missed ETX\n");
        }
        channel->step = 0;
        break;
      default:
        channel->step = 0;
        break;
    }

    // If we get a complete update, and we aren't already initialized
    if(channel->complete_pattern && !state.initialized) {
      PRINT("Got a location\n");
      // Copy data into location
      state.x = channel->location_x;
      state.y = channel->location_y;
      state.counter = channel->counter;

      memcpy(state.pattern, channel->pattern, sizeof(channel->pattern));


      // Need to rotate the device based on which direction the data came from



      /* Final configuration
       *        N                N                       
       *    ---------        ---------                               
       *    |       |        |       |                               
       *   W|       |E ---- W|       |E                        
       *    |       |        |       |                               
       *    ---------        ---------                               
       *        S                S                       
       *
       * Messages should come from direction "opposite" of the channel it arrived on
       * opposite(received_direction) = ((received_direction + 2) % 4);
       *
       * Distance from opposite(received_direction) to the actual direction is the amount to rotate by
       */
      switch(channel->orientation) {
        case NORTH:
          PRINT("Other boards NORTH\n");
          break;
        case WEST:
          PRINT("Other boards WEST\n");
          break;
        case SOUTH:
          PRINT("Other boards SOUTH\n");
          break;
        case EAST:
          PRINT("Other boards EAST\n");
          break;
      }

      switch(channel->direction) {
        case NORTH:
          PRINT("This boards NORTH\n");
          break;
        case WEST:
          PRINT("This boards WEST\n");
          break;
        case SOUTH:
          PRINT("This boards SOUTH\n");
          break;
        case EAST:
          PRINT("This boards EAST\n");
          break;
      }


      Direction desired = ((channel->orientation + 2) % 4);

      switch(desired) {
        case NORTH:
          PRINT("Should be NORTH\n");
          break;
        case WEST:
          PRINT("Should be WEST\n");
          break;
        case SOUTH:
          PRINT("Should be SOUTH\n");
          break;
        case EAST:
          PRINT("Should be EAST\n");
          break;
      }
      state.rotation = (channel->direction - desired);
      state.rotation = state.rotation % 4;
      PRINT("Rotation: ");
      printDec(state.rotation);
      PRINT("\n");

      for(int i=0; i < FRAME_COUNT; i++) {
        // PRINT("Frame ");
        // printDec(i);
        // PRINT("\n");
        // bitmaskPrint(state.pattern[i]);
      }

      state.initialized = 1;

      channel->complete_pattern = 0;
    }
  }
}


void protocolProcessMessages() {
  // Process channels
  processChannel(NORTH);
  processChannel(WEST);
  processChannel(SOUTH);
  processChannel(EAST);
}

void processLight() {
  int frame = state.counter % FRAME_COUNT;
  uint8_t on = bitmaskGet(state.pattern[frame], state.x, state.y);

  // bitmaskPrint(state.pattern[frame]);
  if(on) {
    // PRINT("LIGHT ON\n");
    timerSetDuty(90.0);
    GPIOB->BRR = 0x0080;
  } else {
    // PRINT("LIGHT OFF\n");
    timerSetDuty(0.0);
    GPIOB->BSRR = 0x0080;
  }
  // Synchronize once per 5 second
  // PRINT("counter ");
  // printDec(state.counter);
  // PRINT("\n");
  if(state.x == 0 && state.y == 0) {
    if(state.counter % (1) == 0) {
      PRINT("Sending synchronization message\n");
      sendSynchronizationMessage(&channel_N);
      sendSynchronizationMessage(&channel_W);
      sendSynchronizationMessage(&channel_S);
      sendSynchronizationMessage(&channel_E);
    }
  }
}

void protocolStep() {
  protocolProcessMessages();
  if(rtcGetAndClear()) {
    processLight();
    state.counter++;
  }
}
