#include "cclipboard.h"

std::ostream &operator<<(std::ostream &out, const Cclipboard &tgt) {
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

std::istream &operator>>(std::istream &in, Cclipboard &tgt) {
  HANDLE hData;
  PTSTR strMem, strText;
  std::string str;
  std::u16string str16;

  std::noskipws(in);
  std::istream_iterator<char> begin(in);
  std::istream_iterator<char> end;
  str = std::string(begin, end);

  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cv;
  try {
    str16 = cv.from_bytes(str);
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
