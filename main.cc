#include "cclipboard.h"
#include "ciconv.h"
#include <io.h>
#include <fcntl.h>
#include <getopt.h>
#include <map>

#define DEFAULT_ENCODING "UTF-8"

const char CR = '\r';
const char LF = '\n';

static std::string progname;

void usage();
void err_exit(std::string to, std::string from);
void crlf2lf(std::string &targetStr);
void lf2crlf(std::string &targetStr);

int main(int argc, char * const argv[])
{
  progname = argv[0];
  size_t pos;
  if ((pos = progname.rfind("\\", progname.length()) + 1) != std::string::npos) {
    progname.erase(0, pos);
  }

  std::map<char, bool> optflag;
  std::string charset;
  optflag['i'] = false;
  optflag['o'] = false;
  optflag['l'] = false;
  optflag['r'] = false;

  struct option longopts[] = {
    {"lf",   no_argument, NULL, 'l'},
    {"crlf", no_argument, NULL, 'r'},
    {"charset", required_argument, NULL, 'c'},
    {0,      0,           0,     0 },
  };

  int opt;
  int longindex;
  while ((opt = getopt_long(argc, argv, "ioc:", longopts, &longindex)) != -1) {
    switch (opt) {
      case 'i':
        optflag['i'] = true;
        break;
      case 'o':
        optflag['o'] = true;
        break;
      case 'l':
        optflag['l'] = true;
        break;
      case 'r':
        optflag['r'] = true;
        break;
      case 'c':
        charset = optarg;
        break;
      case ':':
        usage();
        return 1;
      default:
        usage();
        return 1;
    }
  }

  if ((optflag['i'] && optflag['o'])
      || (!optflag['i'] && !optflag['o'])
      || (optflag['i'] && optflag['l'])
      || (optflag['o'] && optflag['r'])) {
    usage();
    return 1;
  }

  Cclipboard cclipboard;
  std::string str;
  std::stringstream ss;

  if (optflag['o']) {
    ss << cclipboard;
    if (charset.empty()) {
      str = ss.str();
    }
    else {
      try {
        Ciconv ciconv(charset, DEFAULT_ENCODING);
        str = ciconv.convert(ss.str());
      } catch (std::invalid_argument) {
        err_exit(charset, DEFAULT_ENCODING);
      }
    }
    if (optflag['l']) {
      crlf2lf(str);
    }
    _setmode(_fileno(stdout), _O_BINARY);
    std::cout.write(str.c_str(), str.length());
  }
  else {
    std::cin >> std::noskipws;
    std::istream_iterator<char> begin(std::cin);
    std::istream_iterator<char> end;
    str = std::string(begin, end);

    if (optflag['r']) {
      lf2crlf(str);
    }

    if (!charset.empty()) {
      try {
        Ciconv ciconv(DEFAULT_ENCODING, charset);
        str = ciconv.convert(str);
      } catch (std::invalid_argument) {
        err_exit(DEFAULT_ENCODING, charset);
      }
    }
    std::stringstream(str) >> cclipboard;
  }

  return 0;
}

void usage() {
  std::cerr << progname << std::endl << std::endl
    << "Usage:" << std::endl
    << "    " << progname << " -o [--lf] [[-c|--charset=]output charset]" << std::endl
    << "    " << progname << " -i [--crlf] [[-c|--charset=]input charset]" << std::endl << std::endl
    << "Options:" << std::endl
    << "    -o         Print clipboard content to stdout" << std::endl
    << "    -i         Set clipboard from stdin" << std::endl
    << "    -c         Output or input charset" << std::endl
    << "    --lf       Replace CRLF with  LF before printing to stdout" << std::endl
    << "    --crlf     Replace lone LF bytes with CRLF before setting the clipboard" << std::endl
    << "    --charset  Output or input charset (default \""  << DEFAULT_ENCODING << "\")" << std::endl;
}

void err_exit(std::string to, std::string from) {
  std::cerr << "The conversion from "
    << from << " to " << to
    << "is not supported by the implementation.";
  exit(1);
}

void crlf2lf(std::string &targetStr) {
  std::string destStr;
  for (std::string::const_iterator it = targetStr.begin();
      it != targetStr.end(); ++it) {
    try {
      if (*it == CR && *(it + 1) == LF) {
        ++it;
      }
    }
    catch (std::out_of_range) {
    }
    destStr += *it;
  }
  targetStr = destStr;
}

void lf2crlf(std::string &targetStr) {
  std::string destStr;
  for (std::string::const_iterator it = targetStr.begin();
      it != targetStr.end(); ++it) {
    if (*it == LF && *(it - 1) != CR) {
      destStr += CR;
    }
    destStr += *it;
  }
  targetStr = destStr;
}
