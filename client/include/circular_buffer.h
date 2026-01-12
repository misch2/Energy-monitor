#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <array>

template <typename T, size_t N>
class CircularBuffer {
 public:
  CircularBuffer() : head(0), count(0) {}

  // Add a new element to the buffer and overwrite the oldest if full.
  void push(T value) {
    buffer[head] = value;
    head = (head + 1) % N;
    if (count < N) {
      count++;
    }
  }

  // Get element at index (0 = oldest, count-1 = newest)
  T get(size_t index) const {
    if (index >= count) {
      return T();
    }
    size_t actualIndex = (head + N - count + index) % N;
    return buffer[actualIndex];
  }

  T operator[](size_t index) const {
    return get(index);
  }

  size_t size() const {
    return count;
  }

  bool isEmpty() const {
    return count == 0;
  }

  bool isFull() const {
    return count == N;
  }

  void clear() {
    head = 0;
    count = 0;
  }

  T getLast() const {
    if (count == 0) {
      return T();
    }
    size_t lastIndex = (head + N - 1) % N;
    return buffer[lastIndex];
  }

 private:
  std::array<T, N> buffer;
  size_t head;   // points to the next write position
  size_t count;  // number of elements currently in the buffer (max N)
};

template <typename T, size_t N>
class SmartCircularBuffer : public CircularBuffer<T, N> {
 public:
  SmartCircularBuffer() : CircularBuffer<T, N>() {}

  // Get the maximum of latest numValues elements
  T maxValue(size_t numValues) const {
    if (numValues > this->size()) {
      numValues = this->size();
    }
    T maxVal = T();
    for (size_t i = this->size() - numValues; i < this->size(); i++) {
      T val = this->get(i);
      if (i == this->size() - numValues || val > maxVal) {
        maxVal = val;
      }
    }
    return maxVal;
  }

  // Get the average of latest numValues elements
  T averageValue(size_t numValues) const {
    if (numValues > this->size()) {
      numValues = this->size();
    }
    T sum = T();
    for (size_t i = this->size() - numValues; i < this->size(); i++) {
      sum += this->get(i);
    }
    return sum / static_cast<T>(numValues);
  }
};

#endif  // CIRCULAR_BUFFER_H