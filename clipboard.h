#ifndef _CLIPBOARD_H
#define _CLIPBOARD_H

#define UNICODE
#define _UNICODE

#include <codecvt>
#include <boost/iostreams/filter/stdio.hpp>
#include <windows.h>
#include "iconv_wrapper.h"

namespace io = boost::iostreams;

std::string getall(std::istream& in);

class iconv_stdio_filter : public io::stdio_filter {
  public:
    iconv_stdio_filter(std::string to, std::string from)
      : m_ciconv(to, from) {};
    iconv_stdio_filter(const iconv_stdio_filter &isf)
      : m_ciconv(isf.m_ciconv) {};
    iconv_stdio_filter &operator=(const iconv_stdio_filter &isf);
  private:
    void do_filter ();
    iconv_wrapper m_ciconv;
};

class clipboard_source {
  public:
    typedef char            char_type;
    typedef io::source_tag  category;
    clipboard_source();
    std::streamsize read(char* s, std::streamsize n);
  private:
    std::streamsize m_pos;
    std::string m_data;
};

class clipboard_sink {
  public:
    typedef char char_type;
    struct category
      : public io::sink_tag,
      public io::closable_tag
  {};
    ~clipboard_sink();
    std::streamsize write(const char * s, std::streamsize n);
    void close();
  private:
    void save_clipboard(std::string src);
    std::string m_data;
};

#endif
