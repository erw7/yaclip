#include "cnlconv.h"

std::string Cnlconv::convert(const std::string &src) {
  std::string dest;
  for (std::string::const_iterator it = src.begin();
      it != src.end(); ++it) {
    if (*it == CR[0]) {
      dest += m_to;
      if (*(it + 1) == LF[0]) {
        ++it;
      }
    }
    else if (*it == LF[0]) {
      dest += m_to;
    }
    else {
      dest += *it;
    }
  }
  return dest;
}
