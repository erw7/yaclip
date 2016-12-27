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

std::ostream &operator<<(std::ostream &out, const Clipboard &tgt) {
  if (IsClipboardFormatAvailable(CF_UNICODETEXT) &&
      OpenClipboard(NULL)) {
    HANDLE hData;
    PTSTR strText, strClip;

    hData = GetClipboardData(CF_UNICODETEXT);
    strText = (PTSTR)malloc(GlobalSize(hData));
    strClip = (PTSTR)GlobalLock(hData);
    lstrcpy(strText, strClip);

    GlobalUnlock(hData);
    CloseClipboard();

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cv;
    try {
      out << cv.to_bytes(strText);
    }
    catch (std::range_error) {
      std::cerr << "Failed wstring_convert." << std::endl;
      exit(1);
    }
  }
  out << "";

  return out;
}

std::istream &operator>>(std::istream &in, Clipboard &tgt) {
  HANDLE hData;
  PTSTR strMem, strText;
  std::string str;
  std::u16string str16;
  std::stringstream ss;
  bool init = true;

  while (std::getline(in, str)) {
    if (!init) {
      ss << tgt.getNewLine();
    }
    ss << str;
    init = false;
  }
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cv;
  try {
    str16 = cv.from_bytes(ss.str());
  }
  catch (std::range_error) {
    std::cerr << "Failed wstring_convert." << std::endl;
    exit(1);
  }
  strText = (PTSTR)str16.c_str();

  if (OpenClipboard(NULL)) {
    EmptyClipboard();

    hData = GlobalAlloc(GHND | GMEM_SHARE, (str16.length() + 1) * sizeof(char16_t));
    // hData = GlobalAlloc(GHND | GMEM_SHARE, (wcslen(strText) + 1) * sizeof(wchar_t));
    strMem = (PTSTR)GlobalLock(hData);
    lstrcpy(strMem, strText);
    GlobalUnlock(hData);

    SetClipboardData(CF_UNICODETEXT, hData);

    CloseClipboard();
  }

  return in;
}

#endif
