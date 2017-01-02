#include "ciconv.h"

std::vector<std::string> Ciconv::encodings;

Ciconv::Ciconv(std::string to, std::string from) {
  if (Ciconv::encodings.empty()) {
    iconvlist(Ciconv::init_encodings, NULL);
  }
  m_from = from;
  m_to = to;
  m_cd = iconv_open(m_to.c_str(), m_from.c_str());
  if (m_cd == (iconv_t) - 1 && errno == EINVAL) {
    std::stringstream ss;
    ss << "The conversion from "
       << from << " to " << to
       << "is not supported by the implementation.";
    throw std::invalid_argument(ss.str());
  }
}

Ciconv::~Ciconv() {
  iconv_close(m_cd);
}

std::string Ciconv::convert(std::string src) {
  std::string result;
  Cmembuf<char> ccharbuf(src.length());
  char *p_src;
  char dest[BUFLEN];
  char* p_dest;
  size_t inbyteleft, outbytesleft;
  ccharbuf.copy(src.c_str(), src.length());
  p_src = ccharbuf.c_ptr();
  p_dest =  dest;
  inbyteleft = src.length();
  outbytesleft = BUFLEN;
  while (1) {
    size_t ret = iconv(m_cd, &p_src, &inbyteleft, &p_dest, &outbytesleft);
    if (ret == (size_t)(-1)) {
      if (errno == EILSEQ || errno == EINVAL) {
        throw std::invalid_argument("Failed convert.");
      }
      else if (errno == E2BIG) {
        result.append(dest, BUFLEN - outbytesleft);
        p_dest = dest;
        outbytesleft = BUFLEN;
      }
      else {
        assert(false);
      }
    }
    else {
      result.append(dest, BUFLEN - outbytesleft);
      break;
    }
  }
  return result;
}

std::vector<std::string> Ciconv::get_encodings(){
  return Ciconv::encodings;
}

bool Ciconv::is_enable_encoding(const std::string encoding) {
  std::string target = encoding;
  std::transform(target.cbegin(), target.cend(), target.begin(), toupper);
  for (std::vector<std::string>::const_iterator it = encodings.begin(); it != encodings.end(); ++it) {
    if (*it == target) {
      return true;
    }
  }
  return false;
}
