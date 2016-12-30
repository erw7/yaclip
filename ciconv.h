#include <algorithm>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <iconv.h>
#include "ccharbuf.h"

#define BUFLEN 1024

class Ciconv {
  friend void iconvlist(int (*do_one) (unsigned int namescount,
        const char * const * name,
        void* data), void* data);
  public:
  Ciconv(std::string to, std::string from);
  ~Ciconv();
  std::string convert(std::string src);
  std::vector<std::string> get_encodings();
  bool is_enable_encoding(const std::string encoding);
  private:
  iconv_t m_cd;
  std::string m_from;
  std::string m_to;
  static std::vector<std::string> encodings;
  static int init_encodings(unsigned int namescount,
      const char * const * names,
      void* data) {
    (void)data;
    for (unsigned int i = 0; i < namescount; ++i) {
      std::string encoding = names[i];
      std::transform(encoding.cbegin(), encoding.cend(), encoding.begin(), toupper);
      Ciconv::encodings.push_back(names[i]);
    }

    return 0;
  }
};
