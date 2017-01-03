#ifndef _CNLCONV_H
#define _CNLCONV_H

#include <string>

const char CR[] = "\r";
const char LF[] = "\n";
const char CRLF[] = "\r\n";

class Cnlconv {
  public:
    Cnlconv() : m_to(
#ifdef _WIN32
        CRLF
#elif defined __APPLE__
        CR
#else
        LF
#endif
        ) {};
      Cnlconv(std::string to) : m_to(to) {};
    std::string convert(const std::string &src);
  private:
    std::string m_to;
};

#endif
