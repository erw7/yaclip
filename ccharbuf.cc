#include "ccharbuf.h"

Ccharbuf::Ccharbuf(size_t len) {
  p_char = new char[len];
  m_length = len;
}

Ccharbuf::~Ccharbuf() {
  delete [] p_char;
}

int Ccharbuf::length() {
  return m_length;
}

char* Ccharbuf::c_ptr() {
  return p_char;
}

void Ccharbuf::copy(const char* src, std::size_t len) {
  if (len > m_length || len < 0) {
    throw std::out_of_range("Out of range");
  }
  memcpy(p_char, src, len);
}

char Ccharbuf::operator[](std::size_t index) const {
  if (index > m_length - 1 || index < 0) {
    throw std::out_of_range("Out of range");
  }
  return p_char[index];
}

char & Ccharbuf::operator[](std::size_t index) {
  if (index > m_length - 1 || index < 0) {
    throw std::out_of_range("Out of range");
  }
  return p_char[index];
}
