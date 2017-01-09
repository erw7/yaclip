#include "iconv_wrapper.h"

std::vector<std::string> iconv_wrapper::encodings;

iconv_wrapper::iconv_wrapper(const iconv_wrapper &ci) {
  m_cd = iconv_open(ci.m_to.c_str(), ci.m_from.c_str());
  m_from = ci.m_from;
  m_to = ci.m_to;
}

iconv_wrapper &iconv_wrapper::operator=(const iconv_wrapper &ci) {
  m_cd = iconv_open(ci.m_to.c_str(), ci.m_from.c_str());
  m_from = ci.m_from;
  m_to = ci.m_to;
  return (*this);
}

iconv_wrapper::iconv_wrapper(std::string to, std::string from) {
  if (iconv_wrapper::encodings.empty()) {
    iconvlist(iconv_wrapper::init_encodings, NULL);
  }
  m_from = from;
  m_to = to;
  m_cd = iconv_open(m_to.c_str(), m_from.c_str());
  if (m_cd == (iconv_t) - 1 && errno == EINVAL) {
    std::stringstream ss;
    ss << "The conversion from "
       << from << " to " << to
       << " is not supported by the implementation.";
    throw std::invalid_argument(ss.str());
  }
}

iconv_wrapper::~iconv_wrapper() {
  int ret = iconv_close(m_cd);
  if (ret == -1) {
    std::cerr << "An attempt be made to close an invalid conversion descriptor" << std::endl;
  }
}

std::string iconv_wrapper::convert(std::string src) {
  std::string result;
  boost::scoped_array<char> sp_src(new char[src.length()]);
  char* p_src;
  char dest[BUFLEN];
  char* p_dest;
  size_t inbyteleft, outbytesleft;
  p_src = sp_src.get();
  std::copy(src.begin(), src.end(), p_src);
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

std::vector<std::string> iconv_wrapper::get_encodings(){
  return iconv_wrapper::encodings;
}

bool iconv_wrapper::is_enable_encoding(const std::string encoding) {
  std::string target = encoding;
  std::transform(target.cbegin(), target.cend(), target.begin(), toupper);
  for (std::vector<std::string>::const_iterator it = encodings.begin(); it != encodings.end(); ++it) {
    if (*it == target) {
      return true;
    }
  }
  return false;
}
