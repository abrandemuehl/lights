#pragma once


template <class T, int S>
class RingBuffer {
  public:
    RingBuffer() : start(0), end(0) {}

    bool Get(T *result) {
      if(start == end) {
        return false;
      }

      *result = data[start];
      start = indexInc(start);
    }
    bool Put(T data) {
      if(start ==)
    }

  private:
    // Start == end -> Empty
    // Start == end-1 -> Full
    int start;
    int end;
    T data[S+1];

    inline int indexInc(int idx) {
      int res = idx + 1;
      if(res > S) {
        res = 0; 
      }
      return res;
    }

    inline int size(int start, int end) {
      if(start > end) {
        return S - (start - end);
      }
    }
};
