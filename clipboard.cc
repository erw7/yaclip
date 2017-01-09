#include "clipboard.h"

std::string getall(std::istream& in) {
  in >> std::noskipws;
  std::istream_iterator<char> begin(in);
  std::istream_iterator<char> end;
  std::string str = std::string(begin, end);
  return str;
}

iconv_stdio_filter &iconv_stdio_filter::operator=(const iconv_stdio_filter &isf) {
  m_ciconv = isf.m_ciconv;
  return (*this);
}

void iconv_stdio_filter::do_filter () {
  std::string str = getall(std::cin);
  str = m_ciconv.convert(str);
  std::cout << str;
}

clipboard_source::clipboard_source()
  : m_pos(0), m_data("") {
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
      m_data = cv.to_bytes(strText);
    }
  }

std::streamsize clipboard_source::read(char* s, std::streamsize n) {
  std::streamsize amt = m_data.length() - m_pos;
  std::streamsize result = (std::min)(n, amt);
  if (result != 0) {
    std::copy(m_data.begin() + m_pos, m_data.begin() + m_pos + result, s);
    m_pos += result;
    return result;
  }
  else {
    return -1;
  }
}

std::streamsize clipboard_sink::write(const char* s, std::streamsize n) {
  m_data.append(s, n);
  return n;
}

clipboard_sink::~clipboard_sink() {
  if (!m_data.empty()) {
    save_clipboard(m_data);
  }
}
void clipboard_sink::close() {
  save_clipboard(m_data);
  m_data = "";
}

void clipboard_sink::save_clipboard(std::string src) {
  HANDLE hData;
  PTSTR strMem, strText;
  std::u16string str16;

  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cv;
  str16 = cv.from_bytes(src);
  strText = (PTSTR)str16.c_str();

  if (OpenClipboard(NULL)) {
    EmptyClipboard();

    hData = GlobalAlloc(GHND | GMEM_SHARE, (str16.length() + 1) * sizeof(char16_t));
    strMem = (PTSTR)GlobalLock(hData);
    lstrcpy(strMem, strText);
    GlobalUnlock(hData);

    SetClipboardData(CF_UNICODETEXT, hData);

    CloseClipboard();
  }
}
