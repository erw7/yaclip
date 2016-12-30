#ifndef _CLIPBOARD_H
#define _CLIPBOARD_H

#define UNICODE
#define _UNICODE

#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <windows.h>

class Clipboard
{
  public:
    setNewLine(std::string str) {
      newline = str;
    }
    const std::string getNewLine() {
      return newline;
    }
  private:
    std::string newline;
};

std::ostream &operator<<(std::ostream &out, const Clipboard &tgt);
std::istream &operator>>(std::istream &in, Clipboard &tgt);

#endif
