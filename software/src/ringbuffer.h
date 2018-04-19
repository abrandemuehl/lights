#pragma once

template <class T, int S>
class RingBuffer {
 public:
  RingBuffer() : start_(0), end_(1) {}

  bool Get(T *result) {
    // Check for empty
    if (end_ == indexInc(start_)) {
      return false;
    }
    // Start points to the location where the oldest message is
    // End points to where the new message should be placed

    *result = data_[start_];
    start_ = indexInc(start_);
  }

  bool Put(T data, bool force=true) {
    if (!force) {
      // Check for full
      if (start_ == end_) {
        return false;
      }
    }
    // Buffer items get placed in end
    data_[end_] = data;
    end_ = indexInc(end_);
    return true;
  }

 private:
  // Start+1 == end -> Empty
  // Start == end -> Full
  int start_;
  int end_;
  T data_[S + 1];

  inline int indexInc(int idx) { return (idx + 1) % S; }

  inline int space() {
    if (start_ > end_) {
      return S - (start_ - end_);
    }
    return end_ - start_;
  }
};
