#ifndef _CMEMBUF_PRIVATTE_H
#define _CMEMBUF_PRIVATTE_H

#include "cmembuf.h"

template <class T>
Cmembuf<T>::Cmembuf(size_t len) {
  m_ptr = new T[len];
  m_length = len;
}

template <class T>
Cmembuf<T>::~Cmembuf() {
  delete [] m_ptr;
}

template <class T>
int Cmembuf<T>::length() {
  return m_length;
}

template <class T>
T* Cmembuf<T>::c_ptr() {
  return m_ptr;
}

template <class T>
void Cmembuf<T>::copy(const T* src, std::size_t len) {
  if (len > m_length || len < 0) {
    throw std::out_of_range("Out of range");
  }
  memcpy(m_ptr, src, len * sizeof(T));
}

template <class T>
T Cmembuf<T>::operator[](std::size_t index) const {
  if (index > m_length - 1 || index < 0) {
    throw std::out_of_range("Out of range");
  }
  return m_ptr[index];
}

template <class T>
T & Cmembuf<T>::operator[](std::size_t index) {
  if (index > m_length - 1 || index < 0) {
    throw std::out_of_range("Out of range");
  }
  return m_ptr[index];
}

#endif
