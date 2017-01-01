#ifndef _CCHARBUF_H
#define _CCHARBUF_H

#include <stdexcept>
#include <cstddef>

class Ccharbuf {
  public:
    Ccharbuf(size_t len);
    ~Ccharbuf();
    int length();
    char* c_ptr();
    void copy(const char* src, std::size_t len);
    char operator[](std::size_t index) const;
    char & operator[](std::size_t index);
  private:
    char* p_char;
    size_t m_length;
};

#endif
