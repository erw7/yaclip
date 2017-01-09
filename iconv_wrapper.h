#ifndef _CICONV_H
#define _CICONV_H

#include <algorithm>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <boost/scoped_array.hpp>
#include <iconv.h>

#define BUFLEN 1024

class iconv_wrapper {
  friend void iconvlist(int (*do_one) (unsigned int namescount,
        const char * const * name,
        void* data), void* data);
  public:
  iconv_wrapper(std::string to, std::string from);
  iconv_wrapper(const iconv_wrapper &ci);
  ~iconv_wrapper();
  iconv_wrapper &operator=(const iconv_wrapper &ci);
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
      iconv_wrapper::encodings.push_back(names[i]);
    }

    return 0;
  }
};

#endif
