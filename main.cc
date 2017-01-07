#include "cclipboard.h"
#include "ciconv.h"
#include "cnlconv.h"
#include <io.h>
#include <fcntl.h>
#include <getopt.h>
#include <map>

#define DEFAULT_ENCODING "UTF-8"
#define VERSION "0.1.0"

static std::string progname;

void usage();
void version();
void err_exit(std::string to, std::string from);

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
  optflag['h'] = false;
  optflag['v'] = false;

  struct option longopts[] = {
    {"lf",   no_argument, NULL, 'l'},
    {"crlf", no_argument, NULL, 'r'},
    {"charset", required_argument, NULL, 'c'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {0,      0,           0,     0 },
  };

  int opt;
  int longindex;
  while ((opt = getopt_long(argc, argv, "iohVc:", longopts, &longindex)) != -1) {
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
      case 'h':
        optflag['h'] = true;
        break;
      case 'V':
        optflag['v'] = true;
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
      || (!optflag['i'] && !optflag['o'] && !optflag['v'])
      || (optflag['i'] && optflag['l'])
      || (optflag['o'] && optflag['r'])) {
    usage();
    return 1;
  }

  if (optflag['h']) {
    usage();
    return 0;
  }

  if (optflag['v']) {
    version();
    return 0;
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
      Cnlconv cnlconv(LF);
      str = cnlconv.convert(str);
    }
    _setmode(_fileno(stdout), _O_BINARY);
    std::cout.write(str.c_str(), str.length());
  }
  else {
    _setmode(_fileno(stdin), _O_BINARY);
    std::cin >> std::noskipws;
    std::istream_iterator<char> begin(std::cin);
    std::istream_iterator<char> end;
    str = std::string(begin, end);

    if (optflag['r']) {
      Cnlconv cnlconv(CRLF);
      str = cnlconv.convert(str);
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
    << "    " << progname << " [-h|--help]" << std::endl
    << "    " << progname << " [-V|--version]" << std::endl
    << "    " << progname << " -o [--lf] [[-c|--charset=]output charset]" << std::endl
    << "    " << progname << " -i [--crlf] [[-c|--charset=]input charset]" << std::endl << std::endl
    << "Options:" << std::endl
    << "    -h, --help       Print this help and exit" << std::endl
    << "    -V, --version    Print version information and exit" << std::endl
    << "    -o               Print clipboard content to stdout" << std::endl
    << "    -i               Set clipboard from stdin" << std::endl
    << "    --lf             Replace CRLF with LF before printing to stdout" << std::endl
    << "    --crlf           Replace lone LF bytes with CRLF before setting the clipboard" << std::endl
    << "    -c, --charset    Output or input charset (default \""  << DEFAULT_ENCODING << "\")" << std::endl;
}

void version() {
  std::cout << progname << " " << VERSION << std::endl
    << "Copyright (C) 2017 erw7@gmail.com" << std::endl
    << "This is a free software; see the LICENSE for copy conditions.  There is NO" << std::endl
    << "warranty; not even for MERCHANTABILIRY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
}

void err_exit(std::string to, std::string from) {
  std::cerr << "The conversion from "
    << from << " to " << to
    << "is not supported by the implementation.";
  exit(1);
}
