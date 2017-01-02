#ifndef _CMEMBUF_H
#define _CMEMBUF_H

#include <stdexcept>
#include <cstddef>

template <class T>
class Cmembuf {
  public:
    Cmembuf(size_t len);
    ~Cmembuf();
    int length();
    T* c_ptr();
    void copy(const T* src, std::size_t len);
    T operator[](std::size_t index) const;
    T & operator[](std::size_t index);
  private:
    T* m_ptr;
    size_t m_length;
};

#include "cmembuf_private.h"

#endif
