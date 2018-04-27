#include "protocol.h"
#include "usart.h"
#include <string.h>
#include "console.h"
#include "rtc.h"
#include "timer.h"
#include "bitmask.h"
#include "pattern.h"

#define USART_N (USART3)
#define USART_W (USART6)
#define USART_S (USART4)
#define USART_E (USART5)

// Frame count must be a divisor of 256 because the counter is one byte.
// Wrapping around will cause a glitch if you don't
#define FRAME_COUNT (3)

// Declare if this device is the leader
#ifndef LEADER
#define LEADER (0)
#endif

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
  uint8_t shouldReset;
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

union {
  struct {
    Channel N;
    Channel W;
    Channel S;
    Channel E;
  };
  Channel array[4];
} channels;




void processLight();
void protocolProcessMessages();
void processChannel(Direction dir);
int buildLocationResponseMessage(Channel *channel, uint8_t *buf);
int buildLocationRequestMessage(uint8_t *buf);
int buildSynchronizationMessage(uint8_t *buf);
int buildResetMessage(uint8_t *buf);

void sendLocationResponseMessage(Channel *channel);
void sendLocationRequestMessage(Channel *channel);
void sendSynchronizationMessage(Channel *channel);
void sendResetMessage(Channel *channel);

void sendMessageOutward(uint8_t *data, int n);
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
 *
 * ---------Reset--------
 * uint8_t TYPE = ResetType
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
  ResetType=0x23,
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

void sendMessageOutward(uint8_t *msg, int n) {
  // Send message away from (0,0)
  if(state.x < 0) {
    // Send message to the right but not the left
    sendMessage(&channels.array[rotatedDirection(EAST)], msg, n);
  } else if (state.x > 0) {
    // Send message to the left but not the right
    sendMessage(&channels.array[rotatedDirection(WEST)], msg, n);
  } else {
    // Send message left and right
    sendMessage(&channels.array[rotatedDirection(WEST)], msg, n);
    sendMessage(&channels.array[rotatedDirection(EAST)], msg, n);
  }
  if(state.y < 0) {
    // Send message down
    sendMessage(&channels.array[rotatedDirection(SOUTH)], msg, n);
  } else if(state.y > 0) {
    // Send message up
    sendMessage(&channels.array[rotatedDirection(NORTH)], msg, n);
  } else {
    // Send message up and down
    sendMessage(&channels.array[rotatedDirection(NORTH)], msg, n);
    sendMessage(&channels.array[rotatedDirection(SOUTH)], msg, n);
  }
}

// Creates a location message in a buffer
int buildLocationRequestMessage(uint8_t *buf) {
  buf[0] = SOH;
  buf[1] = LocationRequestType;
  buf[2] = ETX;
  return 3;
}

void sendLocationRequestMessage(Channel *channel) {
  uint8_t buf[100];
  int len = buildLocationRequestMessage(buf);
  sendMessage(channel, buf, len);
}

int buildLocationResponseMessage(Channel *channel, uint8_t *buf) {
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

  return idx;
}

// Creates a location message in a buffer
void sendLocationResponseMessage(Channel *channel) {
  PRINT("Sending LocationResponseMessage\n");
  uint8_t buf[256];
  int len = buildLocationResponseMessage(channel, buf);
  sendMessage(channel, buf, len);
  return;
}

int buildSynchronizationMessage(uint8_t *buf) {
  buf[0] = SOH;
  buf[1] = SynchronizationType;
  buf[2] = state.counter;
  buf[3] = ETX;
  return 4;
}

void sendSynchronizationMessage(Channel *channel) {
  uint8_t buf[100];
  int len = buildSynchronizationMessage(buf);
  sendMessage(channel, buf, len);
}

int buildResetMessage(uint8_t *buf) {
  buf[0] = SOH;
  buf[1] = ResetType;
  buf[2] = ETX;
  return 3;
}

void sendResetMessage(Channel *channel) {
  uint8_t buf[100];
  int len = buildResetMessage(buf);
  sendMessage(channel, buf, len);
}

void protocolInit() {
  memset(&state, 0, sizeof(state));

  // Reset all state machines
  memset(&channels, 0, sizeof(channels));
  // memset(&channel_N, 0, sizeof(channel_N));
  // memset(&channel_W, 0, sizeof(channel_W));
  // memset(&channel_S, 0, sizeof(channel_S));
  // memset(&channel_E, 0, sizeof(channel_E));
  channels.N.usart = USART_N;
  channels.W.usart = USART_W;
  channels.S.usart = USART_S;
  channels.E.usart = USART_E;

  channels.N.direction = NORTH;
  channels.W.direction = WEST;
  channels.S.direction = SOUTH;
  channels.E.direction = EAST;

#if LEADER
  PRINT("Starting up as LEADER\n");
  makeDefaultPattern(state.pattern);
  state.initialized = 1;
#else // LEADER
  PRINT("Starting up as FOLLOWER\n");

  for(int i=0; i < 5; i++) {
    sendLocationRequestMessage(&channels.N);
    sendLocationRequestMessage(&channels.E);
    sendLocationRequestMessage(&channels.S);
    sendLocationRequestMessage(&channels.W);

    for (int i = 0; i < 100000; i++);
    protocolProcessMessages();
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
#endif // LEADER
  // Reset all neighbors outwards
  uint8_t resetMsg[100];
  int len = buildResetMessage(resetMsg);
  sendMessageOutward(resetMsg, len);

}

void processChannel(Direction dir) {
  Channel *channel;
  dir = rotatedDirection(dir);

  switch(dir) {
    case NORTH:
      channel = &channels.N;
      break;
    case WEST:
      channel = &channels.W;
      break;
    case SOUTH:
      channel = &channels.S;
      break;
    case EAST:
      channel = &channels.E;
      break;
  }


  uint8_t data;

  // int y;
  // int x;
  // int frame;
  while(usartRead(channel->usart, &data, 1) > 0) {
    // PRINT("Received Data ");
    // printByte(data);
    // PRINT("\n");
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

    // PRINT("Step: ");
    // printDec(channel->step);
    // PRINT("\n");

    switch (channel->step) {
      case 0: // SOH
        if(data == SOH) {
          channel->step++;
          // PRINT("SOH\n");
        }
        break;
      case 1: // Type
        channel->type = data;
        switch(data) {
          case LocationRequestType:
            // PRINT("LocationRequestType\n");
            channel->step = 2;
            break;
          case LocationResponseType:
            // PRINT("LocationResponseType\n");
            channel->step = 50;
            break;
          case SynchronizationType:
            // PRINT("SynchronizationType\n");
            channel->step = 25;
            break;
          case ResetType:
            channel->step = 10;
            break;
          default:
            channel->step = 0;
            continue;
        }
        break;
      case 2: // Location request message
        // Receive an ETX
        if(data == ETX) {
          if(!state.initialized) {
            // Disregard message
            PRINT("Disregarding. Not initialized yet\n");
          } else {
            // Send a position response message
            PRINT("Responding to locationRequest\n");
            sendLocationResponseMessage(channel);
            // PRINT("Done sending location\n");
          }
        } else {
          PRINT("Not ETX\n");
        }
        channel->step = 0;
        break;
      case 10: // Reset message
        if(data == ETX) {
          // Trigger a protocol reset
          state.shouldReset = 1;
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
          // PRINT("Counter: ");
          // printDec(state.counter);
          // PRINT("\n");
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
#if LEADER
      // Make sure the position is 0
      state.x = 0;
      state.y = 0;
#else
      // Copy data into location
      state.x = channel->location_x;
      state.y = channel->location_y;
#endif
      state.counter = channel->counter;

      // Keep the pattern even if this is the leader
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

      // for(int i=0; i < FRAME_COUNT; i++) {
      //   PRINT("Frame ");
      //   printDec(i);
      //   PRINT("\n");
      //   bitmaskPrint(state.pattern[i]);
      // }

      state.initialized = 1;

      channel->complete_pattern = 0;

      // Reset all neighbors outwards
      uint8_t resetMsg[100];
      int len = buildResetMessage(resetMsg);
      sendMessageOutward(resetMsg, len);
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
  PRINT("counter ");
  printDec(state.counter);
  PRINT("\n");
  if(state.counter % (1) == 0) {
    PRINT("(");
    printDec(state.x);
    PRINT(",");
    printDec(state.y);
    PRINT(")\n");
    PRINT("Sending synchronization message\n");

    // Send synchronization away from (0,0)
    uint8_t buf[10];
    int len = buildSynchronizationMessage(buf);
    sendMessageOutward(buf, len);
  }
}

void protocolStep() {
  if(state.shouldReset) {
    PRINT("Resetting\n");
    state.initialized = 0;
    protocolInit();
  }
  protocolProcessMessages();
  if(rtcGetAndClear()) {
#if !LEADER
    if(state.x == 0 && state.y == 0) {
      PRINT("Not LEADER but location is (0,0)\n");
      return;
    }
#endif // !LEADER
    processLight();
    state.counter++;
  }
}
