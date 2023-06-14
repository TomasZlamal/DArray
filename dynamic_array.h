#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <initializer_list>
#include <iostream>
#include <optional>

/*
 * @author: Tomáš Zlámal
 * @description: This class is supposed to be a clean implementation of a
 * dynamic array, with its main goal of being easy to expand on
 */
template <typename T> class DArray {
private:
  //
  //  PRIVATE VARIABLES
  //

  T *m_data;
  size_t m_length;
  size_t m_reserve;

  //
  //  PRIVATE FUNCTIONS
  //

  void cleanUp() {
    if (m_data) {
      delete[] m_data;
      m_data = nullptr;
      m_reserve = 0;
      m_length = 0;
    }
  }
  void resizeCheck(size_t elements_to_add_count) {
    if (m_length + elements_to_add_count <= m_reserve) {
      return;
    }
    size_t new_length = m_length + elements_to_add_count;
    T *temp = new T[new_length * 2]{};
    for (size_t i = 0; i < m_length; i++) {
      temp[i] = m_data[i];
    }
    cleanUp();
    m_reserve = new_length * 2;
    m_length = new_length - elements_to_add_count;
    m_data = temp;
  }

  //
  //  UTIL FUNCTIONS
  //

  void moveElementsToTheRightFrom(size_t index) {
    if (index < 0 || index >= m_length) {
      return;
    }
    resizeCheck(1);
    ++m_length;
    T carry;
    for (size_t i = index; i < m_length; i++) {
      if (i == index) {
        carry = m_data[i];
        continue;
      }
      if (i == m_length) {
        m_data[i] = carry;
        continue;
      }
      T temp = carry;
      carry = m_data[i];
      m_data[i] = temp;
    }
  }

  void init(int len, int max) {
    if (max < len) {
      max = len * 2;
    }
    m_length = len;
    m_reserve = max;
    if (!max) {
      m_data = nullptr;
    } else {
      m_data = new T[max]{};
    }
  }

  //
  //  DEBUGGING FUNCTIONS
  //

  void printArray() const {
    for (size_t i = 0; i < m_length; i++) {
      std::cout << "Element at " << i << " is " << m_data[i] << "\n";
    }
  }

  void operationFailed(const char *what) {
    std::cerr << "OPERATION " << what << " FAILED!" << std::endl;
  }

public:
  //
  //  CONSTRUCTORS
  //

  DArray() { init(0, 0); }
  DArray(size_t len) { init(0, len); }
  DArray(std::initializer_list<T> items) {
    init(items.size(), items.size() * 2);
    int i = 0;
    for (T elem : items) {
      m_data[i] = elem;
      i++;
    }
  }

  //
  //  ACCESS METHODS
  //

  std::optional<T> getAt(size_t index) const {
    return (index >= 0 && index < m_length) ? m_data[index] : {};
  }

  //
  //  INSERT METHODS
  //

  void append(T data) {
    resizeCheck(1);
    m_data[m_length] = data;
    ++m_length;
  }
  void append(T data, size_t index) {
    if (index < 0 || index >= m_length) {
      return;
    }
    moveElementsToTheRightFrom(index);
    m_data[index] = data;
  }

  //
  // INFORMATIONAL FUNCTIONS
  //

  size_t getSize() const { return m_length; }
  size_t getMemoryAllocated() const { return m_reserve; }
  bool empty() const { return m_length; }
  size_t getSpaceUntilResize() const { return (m_reserve - m_length); }
  bool dataExists() const { return m_data; }

  //
  //  DESTRUCTOR
  //

  ~DArray() { cleanUp(); }
};
#endif
